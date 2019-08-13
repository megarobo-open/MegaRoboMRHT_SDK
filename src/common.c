#include "common.h"
#include "platform.h"
#include <limits.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

char *__GetFileName__(const char *file)
{
    char *name = NULL;
    char *ptr = (char*)file;
    if(file == NULL)
        return NULL;

    while(*ptr != '\0')
    {
#ifdef __WIN32
        if(*ptr == '\\')
#else
        if(*ptr == '/')
#endif
        {
            name = ptr;
        }
        ptr++;
    }
    return name == NULL ? (char*)file : (name+1);
}

#ifndef MRG_DEBUG
void be_printf(char *fmt, ...){ (void)fmt; }
void print_buffer(char* msg, unsigned char* pBuff, unsigned long ulLen){(void)msg;(void)pBuff;(void)ulLen;}
#else
void be_printf (char *str, ...)
{
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mutex);
    va_list	vl;
    char	buf[1024] = "";
    va_start (vl, str);
    vsnprintf (buf, sizeof (buf), str, vl);
    va_end (vl);
    buf[sizeof (buf) - 1] = '\0';
    fprintf(stdout, "%s", buf);
    fflush(stdout);
    pthread_mutex_unlock(&mutex);
    return;
}

void print_buffer(char* msg, unsigned char* pBuff, unsigned long ulLen)
{
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mutex);
    unsigned long ulTmp = 0;
    be_printf("%s:\n", msg);
    be_printf("    ");
    while(ulTmp < ulLen)
    {
        be_printf("%02x ",pBuff[ulTmp]);
        ulTmp++;
        if(!(ulTmp%16))
        {
            be_printf("\n");
            be_printf("    ");
        }
    }
    be_printf("\n");
    pthread_mutex_unlock(&mutex);
}
#endif

int splitStringToIntArray(const char *str, const char *split, int *output)
{
    char *p = NULL;
    char *pNext = NULL;
    int count = 0;
    long val = 0;
    char *endptr = NULL;
    char strBuff[4096] = "";
    if(strlen(str) > sizeof(strBuff))
        return -1;

    strcpy(strBuff, str);
    p = STRTOK_S(strBuff, split, &pNext);
    while (p)
    {
        errno = 0;
        val = strtol(p, &endptr, 10);
        if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
                || (errno != 0 && val == 0))
        {}
        else if (endptr == p)
        {}
        else if (*endptr != '\0')
        {}
        else
        {
            output[count++] = val;
        }
        p = STRTOK_S(NULL, split, &pNext);
    }
    return count;
}

int splitStringToFloatArray(const char *str, const char *split, float *output)
{
    char *p = NULL;
    char *pNext = NULL;
    int count = 0;
    float val = 0;
    char *endptr = NULL;
    char strBuff[4096] = "";
    if(strlen(str) > sizeof(strBuff))
        return -1;

    strcpy(strBuff, str);
    p = STRTOK_S(strBuff, split, &pNext);
    while (p)
    {
        errno = 0;
        val = strtof(p, &endptr);
        if ((errno == ERANGE && val == HUGE_VALF)
                || (errno != 0 && val == 0))
        {}
        else if (endptr == p)
        {}
        else if (*endptr != '\0')
        {}
        else
        {
            output[count++] = val;
        }
        p = STRTOK_S(NULL, split, &pNext);
    }
    return count;
}

int splitString(const char *str, const char *split, char output[][64])
{
    char *p = NULL;
    char *pNext = NULL;
    int count = 0;
    char strBuff[4096] = "";
    if(strlen(str) > sizeof(strBuff))
        return -1;
    strcpy(strBuff, str);
    p = STRTOK_S(strBuff, split, &pNext);
    while (p)
    {
        strcpy( output[count++], p);
        p = STRTOK_S(NULL, split, &pNext);
    }
    return count;
}

static unsigned short CRC16Table[] =
{
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0,
};

unsigned short makeCRC16(unsigned char *pu8Buf, unsigned int u32Len)
{
    int i = 0;
    unsigned char u8CrcReg;
    unsigned short u16CRC = 0xFFFF;
    while (u32Len-- > 0)
    {
        u8CrcReg = (unsigned char)(u16CRC >> 8);
        u16CRC <<= 8;
        u16CRC ^= CRC16Table[u8CrcReg ^ pu8Buf[i++]];
    }
    return (u16CRC);
}

#include "libssh2_config.h"
#include <libssh2.h>

//#ifdef HAVE_WINSOCK2_H
//# include <winsock2.h>
//#endif
//#ifdef HAVE_SYS_SOCKET_H
//# include <sys/socket.h>
//#endif
//#ifdef HAVE_NETINET_IN_H
//# include <netinet/in.h>
//#endif
//# ifdef HAVE_UNISTD_H
//#include <unistd.h>
//#endif
//#ifdef HAVE_ARPA_INET_H
//# include <arpa/inet.h>
//#endif
//#ifdef HAVE_SYS_TIME_H
//# include <sys/time.h>
//#endif

#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>

int scp_send(const char *ip, const char *localfile, const char *remotefile)
{
#if 0
    unsigned long hostaddr;
    int sock;
    struct sockaddr_in sin;
    LIBSSH2_SESSION *session = NULL;
    LIBSSH2_CHANNEL *channel;
    const char *username = "root";
    const char *password = "megarobo";
    const char *loclfile = localfile;
    const char *scppath = remotefile;
    FILE *local;
    int rc;
    char mem[1024];
    size_t nread;
    char *ptr;
    struct stat fileinfo;

#ifdef WIN32
    WSADATA wsadata;
    int err;

    err = WSAStartup(MAKEWORD(2, 0), &wsadata);
    if(err != 0) {
        be_printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }
#endif

    hostaddr = inet_addr(ip);

    rc = libssh2_init(0);
    if(rc != 0) {
        be_printf("libssh2 initialization failed (%d)\n", rc);
        return 1;
    }

    local = fopen(loclfile, "rb");
    if(!local) {
        be_printf("Can't open local file %s\n", loclfile);
        return -1;
    }

    stat(loclfile, &fileinfo);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sock) {
        be_printf("failed to create socket!\n");
        return -1;
    }

    sin.sin_family = AF_INET;
    sin.sin_port = htons(22);
    sin.sin_addr.s_addr = hostaddr;
    if(connect(sock, (struct sockaddr*)(&sin),
               sizeof(struct sockaddr_in)) != 0) {
        be_printf("failed to connect!\n");
        return -1;
    }

    session = libssh2_session_init();
    if(!session)
        return -1;

    rc = libssh2_session_handshake(session, sock);
    if(rc) {
        be_printf("Failure establishing SSH session: %d\n", rc);
        return -1;
    }

    libssh2_hostkey_hash(session, LIBSSH2_HOSTKEY_HASH_SHA1);

    /* We could authenticate via password */
    if(libssh2_userauth_password(session, username, password)) {
        be_printf("Authentication by password failed.\n");
        goto shutdown;
    }

    /* Send a file via scp. The mode parameter must only have permissions! */
    channel = libssh2_scp_send(session, scppath, fileinfo.st_mode & 0777,
                               (unsigned long)fileinfo.st_size);

    if(!channel) {
        char *errmsg;
        int errlen;
        int err = libssh2_session_last_error(session, &errmsg, &errlen, 0);
        be_printf("Unable to open a session: (%d) %s\n", err, errmsg);
        goto shutdown;
    }

    be_printf("SCP session waiting to send file\n");
    do {
        nread = fread(mem, 1, sizeof(mem), local);
        if(nread <= 0) {
            /* end of file */
            break;
        }
        ptr = mem;

        do {
            /* write the same data over and over, until error or completion */
            rc = libssh2_channel_write(channel, ptr, nread);
            if(rc < 0) {
                be_printf("ERROR %d\n", rc);
                break;
            }
            else {
                /* rc indicates how many bytes were written this time */
                ptr += rc;
                nread -= rc;
            }
        } while(nread);

    } while(1);

    be_printf("Sending EOF\n");
    libssh2_channel_send_eof(channel);

    be_printf("Waiting for EOF\n");
    libssh2_channel_wait_eof(channel);

    be_printf("Waiting for channel to close\n");
    libssh2_channel_wait_closed(channel);

    libssh2_channel_free(channel);
    channel = NULL;

 shutdown:

    if(session) {
        libssh2_session_disconnect(session, "Normal Shutdown");
        libssh2_session_free(session);
    }
#ifdef WIN32
    closesocket(sock);
#else
    close(sock);
#endif
    if(local)
        fclose(local);

    libssh2_exit();
    return 0;
}

int scp_recv(const char *ip,const char *remotefile,const char *localfile)
{
    unsigned long hostaddr;
    int sock;
    struct sockaddr_in sin;
    LIBSSH2_SESSION *session;
    LIBSSH2_CHANNEL *channel;
    const char *username = "root";
    const char *password = "megarobo";
    const char *scppath = remotefile;
    const char *loclfile = localfile;
    libssh2_struct_stat fileinfo;
    int rc;
    libssh2_struct_stat_size got = 0;
    FILE *local;

#ifdef WIN32
    WSADATA wsadata;
    int err;

    err = WSAStartup(MAKEWORD(2, 0), &wsadata);
    if(err != 0) {
        be_printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }
#endif

    hostaddr = inet_addr(ip);

    local = fopen(loclfile, "wb");
    if(!local) {
        be_printf("Can't open local file %s\n", loclfile);
        return -1;
    }

    rc = libssh2_init(0);
    if(rc) {
        be_printf("libssh2 initialization failed (%d)\n", rc);
        return 1;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(22);
    sin.sin_addr.s_addr = hostaddr;
    if(connect(sock, (struct sockaddr*)(&sin),
               sizeof(struct sockaddr_in)) != 0) {
        be_printf("failed to connect!\n");
        return -1;
    }

    session = libssh2_session_init();
    if(!session)
        return -1;

    rc = libssh2_session_handshake(session, sock);
    if(rc) {
        be_printf("Failure establishing SSH session: %d\n", rc);
        return -1;
    }

    libssh2_hostkey_hash(session, LIBSSH2_HOSTKEY_HASH_SHA1);

    /* We could authenticate via password */
    if(libssh2_userauth_password(session, username, password)) {
        be_printf("Authentication by password failed.\n");
        goto shutdown;
    }

    /* Request a file via SCP */
    channel = libssh2_scp_recv2(session, scppath, &fileinfo);

    if(!channel) {
        be_printf("Unable to open a session: %d\n",
                libssh2_session_last_errno(session));
        goto shutdown;
    }

    while(got < fileinfo.st_size) {
        char mem[1024];
        int amount = sizeof(mem);

        if((fileinfo.st_size -got) < amount) {
            amount = (int)(fileinfo.st_size -got);
        }

        rc = libssh2_channel_read(channel, mem, amount);
        if(rc > 0) {
            fwrite(mem, 1, rc, local);
        }
        else if(rc < 0) {
            be_printf("libssh2_channel_read() failed: %d\n", rc);
            break;
        }
        got += rc;
    }

    libssh2_channel_free(channel);
    channel = NULL;

 shutdown:

    libssh2_session_disconnect(session,
                               "Normal Shutdown, Thank you for playing");
    libssh2_session_free(session);

#ifdef WIN32
    closesocket(sock);
#else
    close(sock);
#endif
    if(local)
        fclose(local);

    libssh2_exit();
#endif
    return 0;
}
