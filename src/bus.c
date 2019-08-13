#include "bus.h"

#if defined(_WIN32) && (!defined(__MINGW64__)) && (!defined(__MINGW32__))
#pragma comment (lib, "ws2_32.lib")  
#pragma comment (lib, "visa32.lib")
#pragma comment (lib, "IPHlpApi.lib")
#pragma comment (lib, "pthreadVC2.lib")
#endif

#define MEGA_TCP_SOCKET_PORT (5555)

static pthread_mutex_t _mutex = PTHREAD_MUTEX_INITIALIZER;
#define LOCK()     pthread_mutex_lock(&_mutex)
#define UNLOCK()   pthread_mutex_unlock(&_mutex)

static int g_bus_type = BUS_SOCKET;

static char _g_gateway_ip[512] = "";
static size_t _g_timeout = 500;

#if (!defined(_WIN32)) && (!defined(VXI_NOT_SUPPORT))
VXI11_CLINK *_g_clink = NULL;
#else
void *_g_clink = NULL;
#endif

//! 获取主机的IP地址
static int getHostIpAddr(char strHostIp[][100])
{
#ifdef _WIN32
    //! Windows
    unsigned long stSize = sizeof(IP_ADAPTER_INFO);
    PIP_ADAPTER_INFO pIpAdapterInfo = (PIP_ADAPTER_INFO)malloc(stSize);
    int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
    int ipcount = 0;
    if (ERROR_BUFFER_OVERFLOW == nRel)
    {
        free(pIpAdapterInfo);
        pIpAdapterInfo = (PIP_ADAPTER_INFO)malloc(stSize);
        nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
    }
    if (ERROR_SUCCESS == nRel)
    {
        while (pIpAdapterInfo)
        {
            IP_ADDR_STRING *pIpAddrString = &(pIpAdapterInfo->IpAddressList);
            do
            {
                if (strcmp("0.0.0.0", pIpAddrString->IpAddress.String) != 0)
                {
                    strcpy_s(strHostIp[ipcount],100, pIpAddrString->IpAddress.String);
                    ipcount++;
                }
                pIpAddrString = pIpAddrString->Next;
            } while (pIpAddrString);
            pIpAdapterInfo = pIpAdapterInfo->Next;
        }
    }
    if (pIpAdapterInfo)
    {
        free(pIpAdapterInfo);
    }
    return ipcount;
#else
    //! Linux
    struct ifreq ifr;
    struct ifconf ifc;
    char buf[2048];

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock == -1)
    {
        return -1;
    }

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    if (ioctl(sock, SIOCGIFCONF, &ifc) == -1)
    {
        return -2;
    }

    struct ifreq* it = ifc.ifc_req;
    const struct ifreq* const end = it + (ifc.ifc_len/sizeof(struct ifreq));

    int count = 0;
    for (; it != end; ++it)
    {
        strcpy(ifr.ifr_name, it->ifr_name);
        if (ioctl(sock, SIOCGIFFLAGS, &ifr) != 0)
        {
            return -3;
        }
        else
        {
            if (!(ifr.ifr_flags & IFF_LOOPBACK))
            { // don't count loopback
                if (ioctl(sock, SIOCGIFADDR, &ifr) == 0)
                {
                    //printf("IP address is: %s\n", inet_ntoa(((struct sockaddr_in *)(&ifr.ifr_addr))->sin_addr));
                }
                else
                { continue; }
                sprintf(strHostIp[count],"%s", inet_ntoa(((struct sockaddr_in *)(&ifr.ifr_addr))->sin_addr));
                count++;
            }
        }
    }

    return count;
#endif
}

//! 广播查找设备,返回查找的数量
static int socketFindResources(char ip[][100], size_t timeout_ms)
{
#if _WIN32
    WSADATA ws;
#endif
    int ret;
    int count = 0;
    int hostIpCount = 0;
    int i;
    SOCKET sock[256];
    SOCKADDR_IN localAddr;
    SOCKADDR_IN remoteAddr;
    SOCKADDR_IN servaddr;
    char recvBuf[50];
    char strHostIpAddr[256][100];

#ifdef _WIN32
    int len = sizeof(SOCKADDR);
    char bOpt = 1;
#else
    socklen_t len = sizeof(SOCKADDR);
    int bOpt = 1;
#endif

    hostIpCount = getHostIpAddr(strHostIpAddr);
    if (hostIpCount <= 0)
    {
        return -1;
    }

#ifdef _WIN32
    ret = WSAStartup(MAKEWORD(2, 2), &ws);
    if (0 != ret)
    {
        return -1;
    }
#endif
    for (i = 0; i < hostIpCount; i++)
    {
        sock[i] = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (INVALID_SOCKET == sock[i])
        {
#ifdef _WIN32
            WSACleanup();
#endif
            return -1;
        }
        localAddr.sin_family = AF_INET;
        localAddr.sin_port = htons(0);
#ifdef _WIN32
        localAddr.sin_addr.S_un.S_addr = inet_addr(strHostIpAddr[i]);//htonl(INADDR_ANY);
#else
        localAddr.sin_addr.s_addr  = inet_addr(strHostIpAddr[i]);//htonl(INADDR_ANY);
#endif
        ret = bind(sock[i], (SOCKADDR*)&localAddr, sizeof(SOCKADDR));
        if (SOCKET_ERROR == ret)
        {
            return -1;
        }

#ifdef _WIN32
        ret = setsockopt(sock[i], SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout_ms, sizeof(int));
#else
        struct timeval timeout;
        timeout.tv_sec = timeout_ms/1000;
        timeout.tv_usec = (timeout_ms % 1000) * 1000;
        ret = setsockopt(sock[i], SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(struct timeval));
#endif
        if (ret != 0)
        {
            return -1;
        }

#ifdef _WIN32
        ret = setsockopt(sock[i], SOL_SOCKET, SO_BROADCAST, (char*)&bOpt, sizeof(bOpt));
#else
        ret = setsockopt(sock[i], SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, (char*)&bOpt, sizeof(bOpt));
#endif
        if(ret != 0)
        {
            return -1;
        }
    }
    remoteAddr.sin_family = AF_INET;
    remoteAddr.sin_port = htons(6000);
#ifdef _WIN32
    remoteAddr.sin_addr.S_un.S_addr = inet_addr("255.255.255.255");
#else
    remoteAddr.sin_addr.s_addr = inet_addr("255.255.255.255");//htonl(INADDR_ANY);
#endif

    for (i = 0; i < hostIpCount; i++)
    {
        sendto(sock[i], "*?", 2, 0, (SOCKADDR*)&remoteAddr, sizeof(SOCKADDR));
        while (1)
        {
            memset(recvBuf, 0, 50);
            ret = recvfrom(sock[i], recvBuf, sizeof(recvBuf), 0, (SOCKADDR*)&servaddr, &len);
            if (ret > 0)
            {
                strcpy_s(ip[count], 100, inet_ntoa(servaddr.sin_addr));
                count++;
            }
            else
            {
                break;
            }
        }
    }
    for (i = 0; i < hostIpCount; i++)
    {
#ifdef _WIN32
        shutdown(sock[i], 2);
        closesocket(sock[i]);
#else
        close(sock[i]);
#endif
    }
#ifdef _WIN32
    WSACleanup();
#endif
    return count;
}

static size_t busOpenDevice_socket(const char *ip, size_t timeout_ms)
{
    int status = 0;
    fd_set rfd;
    SOCKADDR_IN     addrSrv;
    SOCKET sockfd = 0;
    int error = 0;
    int nZero = 1024 * 1024;
    struct linger so_linger;

#ifdef _WIN32
    WSADATA Data;
    int len = 0;
#else
    socklen_t len;
#endif

    struct timeval  timeout;  //时间结构体
    timeout.tv_sec = timeout_ms/1000;
    timeout.tv_usec = (timeout_ms % 1000) * 1000;

    memset(&addrSrv, 0, sizeof(SOCKADDR_IN));
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(MEGA_TCP_SOCKET_PORT);
#ifdef _WIN32
    addrSrv.sin_addr.S_un.S_addr = inet_addr(ip);
#else
    addrSrv.sin_addr.s_addr  = inet_addr(ip);
#endif

#ifdef _WIN32
    status = WSAStartup(MAKEWORD(1, 1), &Data);
    if (status != 0)
    {
        return -1;
    }
#endif

    sockfd = socket(AF_INET, SOCK_STREAM, 0);//建立套接字
    if (INVALID_SOCKET == sockfd)
    {
#ifdef _WIN32
        WSACleanup();
#endif
        return -2;
    }

    errno = 0;
    status = connect(sockfd, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
    if(status < 0 /* && errno != EINPROGRESS */ )
    {
        status = -3;
        goto ERR;
    }

    FD_ZERO(&rfd);
    FD_SET(sockfd, &rfd);
    status = select(sockfd+1, 0, &rfd, 0, &timeout);
    if (status <= 0)
    {
        status = -4;
        goto ERR;
    }

    len = sizeof(error);
    status = getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (char *)&error, &len);
    if(status == -1)
    {
        fprintf(stderr, "Error in connection() %d - %s/n", error, strerror(error));
        perror("getsockopt SO_ERROR");
        status = -5;
        goto ERR;
    }

    //在调用closesocket后强制关闭    
    so_linger.l_onoff = 1;
    so_linger.l_linger = 0;
    status = setsockopt(sockfd, SOL_SOCKET, SO_LINGER, (char *)&so_linger, sizeof(so_linger));
    if (status == -1)
    {
        perror("setsockopt SO_LINGER");
        status = -6;
        goto ERR;
    }

    status = setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF,(char*)&nZero, sizeof(int));
    if(status == -1)
    {
        fprintf(stderr, "Error in connection() %d - %s/n", error, strerror(error));
        perror("getsockopt SO_SNDBUF");
        status = -99;
        goto ERR;
    }
    status = setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF,(char*)&nZero, sizeof(int));
    if(status == -1)
    {
        fprintf(stderr, "Error in connection() %d - %s/n", error, strerror(error));
        perror("getsockopt SO_RCVBUF");
        status = -88;
        goto ERR;
    }

    status = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO,
                    #ifdef _WIN32
                        /* Windows */   (char *)&timeout_ms, sizeof(int)
                    #else
                        /* Linux */     &timeout, sizeof(struct timeval)
                    #endif
                        );
    if (status == -1)
    {
        perror("setsockopt SO_RCVTIMEO");
        status = -7;
        goto ERR;
    }

    return sockfd;

ERR:
#ifdef _WIN32
    closesocket(sockfd);
    WSACleanup();
#else
    close(sockfd);
#endif
    return status;
}

static size_t busOpenDevice_vxi(const char *ip, size_t timeout)
{
    unsigned long fd = 0;

    memset(_g_gateway_ip, '\0', sizeof(_g_gateway_ip));
    _g_clink = NULL;
    _g_timeout = timeout;

#if defined(_WIN32)
    int status;
    static unsigned long defaultRM;
    char viSrc[64] = "";

    snprintf(viSrc, sizeof(viSrc), "TCPIP0::%s::inst0::INSTR", ip);

    status = viOpenDefaultRM(&defaultRM);
    if (status < 0)
    {
        return -1;
    }
//    status = viOpen(defaultRM, viSrc, VI_NO_LOCK, VI_TMO_IMMEDIATE, &fd);

    //windows一个主机仅允许打开一个-T
    status = viOpen(defaultRM, viSrc, VI_EXCLUSIVE_LOCK, VI_TMO_IMMEDIATE, &fd);
    if (status < 0)
    {
        return -2;
    }

    viSetAttribute(fd, VI_ATTR_TCPIP_NODELAY, VI_TRUE);
    viSetAttribute(fd, VI_ATTR_TCPIP_KEEPALIVE, VI_TRUE);
    viSetAttribute(fd, VI_ATTR_TMO_VALUE, timeout);

#elif (!defined(VXI_NOT_SUPPORT))
    VXI11_CLINK *clink;
    if(vxi11_open_device(&clink, ip, NULL))
    {
        free(clink);
        return -1;
    }

    _g_clink = clink;

    srand((int)time(0));
    fd = (unsigned long)rand()/10000+1;
#endif

    strcpy(_g_gateway_ip, ip);
    return fd;
}

static int SocketWrite(unsigned long fd, unsigned char *data, size_t dataLen)
{
    int ret = -1;
    ret = send(fd, (char *)data, dataLen,
           #ifdef _WIN32
               0
           #else
               MSG_DONTWAIT
           #endif
               );
    return ret;
}

static int SocketRead(unsigned long fd, unsigned char *data, size_t dataLen)
{
    int ret = -1;
    memset(data, 0, dataLen);
    ret = recv(fd, (char *)data, dataLen, 0);
    return ret;
}

static int VxiWrite(unsigned long fd, unsigned char *data, size_t dataLen)
{
    (void)fd;
    (void)data;
    (void)dataLen;
#ifdef _WIN32
    unsigned long retCount = -1;
    if (viWrite(fd, data, dataLen, &retCount) != 0)
    {
        return -1;
    }
    return (int)retCount;

#elif (!defined(VXI_NOT_SUPPORT))
    time_t tm = time(NULL);
    (void)fd;
    int ret = -1;
    if(_g_clink == NULL)
        return -1;

    ret = vxi11_send(_g_clink, (char *)data, dataLen);
    if(ret > 0)
    {
        MRG_LOG("sendData[len:%d][time:%d]: %s\n", ret, time(NULL) - tm, data);
        return ret;
    }
    if(ret < 0)
    {
        perror("SyncSend error!");
    }
    return -1;
#else
    return -1;
#endif
}

static int VxiRead(unsigned long fd, unsigned char *data, size_t dataLen)
{
    (void)fd;
    (void)data;
    (void)dataLen;
    memset(data, 0, dataLen);
#ifdef _WIN32
    unsigned long retCount = -1;
    viRead(fd, (unsigned char *)data, dataLen, &retCount);
    return (int)retCount;
#elif (!defined(VXI_NOT_SUPPORT))
    (void)fd;
    time_t tm = time(NULL);
    int ret = -1;
    if(_g_clink == NULL || data == NULL)
        return -1;

    // -VXI11_NULL_READ_RESP
    ret = vxi11_receive_timeout(_g_clink, (char *)data, dataLen, _g_timeout);
    if(ret > 0)
    {
        MRG_LOG("recvData[len:%d][time:%ds]: %s\n", ret, time(NULL) - tm, data);
        return ret;
    }
    if(ret < 0 && ret != -VXI11_ERROR_IO_TIMEOUT)
    {
        perror("SyncRead error!");
    }
    return -1;
#else
    return -1;
#endif

}

static int SocketClose(unsigned long fd)
{
    if(fd > 0)
    {
#ifdef _WIN32
        closesocket(fd);
#else
        close(fd);
#endif
    }
    return 0;
}

static int VxiClose(unsigned long fd)
{
    (void)fd;
#ifdef _WIN32
    if(fd > 0)
    {
        viClose(fd);
    }
#elif !defined(VXI_NOT_SUPPORT)
    (void)fd;
    if( (_g_clink != NULL) && (0!=strcmp(_g_gateway_ip,"")) )
    {
        vxi11_close_device(_g_clink, _g_gateway_ip);
    }
#endif
    memset(_g_gateway_ip,'\0',sizeof(_g_gateway_ip));
    _g_clink = NULL;
    return 0;
}


/****************************************************************************/
int busFindDevice(int mode, char *output, size_t len)
{
    //! 通过网络广播查找MRH-T
    char ip_list[256][100];
    int r = 0, i = 0;
    int count = 0;

    if (mode == BUS_USB)
    {
        //! TODO

    }

    if (mode == BUS_SOCKET || mode == BUS_VXI)
    {
        count = socketFindResources(ip_list, _g_timeout);
        if(count > 0)
        {
            for (i = 0; i < count; i++)
            {
                snprintf(&output[r], len - r, "TCPIP0::%s::inst0::INSTR,", ip_list[i]);
                r = strlen(output);
            }
        }
        else
        {
            memset(output, 0, len);
            count = 0;
        }
    }

    return count;
}

int busOpenDevice(int mode, char *devDesc, size_t timeout)
{
    int fd = -1;
    char strVal[16][64];
    splitString(devDesc, "::", strVal);

    g_bus_type = mode;

    if (g_bus_type == BUS_USB)
    {
        //! TODO

    }

    if (g_bus_type == BUS_SOCKET)
    {
        fd = busOpenDevice_socket(strVal[1], timeout);
    }

    if (g_bus_type == BUS_VXI)
    {
        fd = busOpenDevice_vxi(strVal[1], timeout);
    }

    return fd;
}

size_t busWrite(unsigned long fd, char *buf, size_t len)
{
    int retCount = 0;
    LOCK();
    if (g_bus_type == BUS_USB)
    {
        //! TODO
        retCount = -1;
    }

    if (g_bus_type == BUS_SOCKET)
    {
        retCount = SocketWrite(fd, (unsigned char *)buf, len);
    }

    if (g_bus_type == BUS_VXI)
    {
         retCount = VxiWrite(fd, (unsigned char *)buf, len);
    }
    UNLOCK();

    if (retCount <= 0)
    {
        return 0;
    }
    return retCount;
}

size_t busRead(unsigned long fd, char *buf, size_t len)
{
    int retCount = 0;
    int recvLen = len > MAX_TRANSMIT_LEN ? MAX_TRANSMIT_LEN : len;

    LOCK();

    if (g_bus_type == BUS_USB)
    {
        //! TODO
        retCount = 0;
    }

    if (g_bus_type == BUS_SOCKET)
    {
        retCount = SocketRead(fd, (unsigned char *)buf, recvLen);
    }

    if (g_bus_type == BUS_VXI)
    {
         retCount = VxiRead(fd, (unsigned char *)buf, recvLen);
    }

    if( (retCount == 0) ||  STRNCASECMP(buf, "Command error", strlen("Command error")) == 0 )
    {
        UNLOCK();
        return 0;
    }
    UNLOCK();
    if(buf[retCount-1] == '\n' && buf[0] != '#')
        buf[retCount-1] = '\0';
    return retCount;
}

size_t busQuery(unsigned long fd, char *input, size_t inputlen, char *output, size_t wantlen)
{
    int retCount = 0;
    int errCount = 1;

    int recvLen = wantlen > MAX_TRANSMIT_LEN ? MAX_TRANSMIT_LEN : wantlen;
    LOCK();
    while(errCount--)
    {
        if (g_bus_type == BUS_USB)
        {
            //! TODO
            UNLOCK();
            retCount = 0;
        }

        if (g_bus_type == BUS_SOCKET)
        {
            if ((retCount = SocketWrite(fd, (unsigned char *)input, inputlen)) != (int)inputlen)
            {
                UNLOCK();
                return 0;
            }
            retCount = SocketRead(fd, (unsigned char *)output, recvLen);
        }

        if (g_bus_type == BUS_VXI)
        {
            if ((retCount = VxiWrite(fd, (unsigned char *)input, inputlen)) <= 0)
            {
                UNLOCK();
                return 0;
            }
            retCount = VxiRead(fd, (unsigned char *)output, recvLen);
        }

        if(retCount > 0)
        {
            break;
        }
        retCount = 0;
    }
    if( (retCount == 0) ||  STRNCASECMP(output, "Command error", strlen("Command error")) == 0 )
    {
        UNLOCK();
        return 0;
    }
    UNLOCK();
    if(output[retCount-1] == '\n' && output[0] != '#')
        output[retCount-1] = '\0';
    return retCount;
}

int busCloseDevice(unsigned long fd)
{
    int ret = 0;
    if (g_bus_type == BUS_USB)
    {
        //! TODO
    }

    if (g_bus_type == BUS_SOCKET)
    {
        ret = SocketClose(fd);
    }

    if (g_bus_type == BUS_VXI)
    {
        ret = VxiClose(fd);
    }
    return ret;
}
