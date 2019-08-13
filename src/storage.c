#include "storage.h"
#include "system.h"

EXPORT_API int CALL mrgStorageMotionFileQuery(ViSession vi, int location, char* ps8FileList, int len)
{
    int retlen = 0;
    char args[SEND_BUF_LEN];
    if (ps8FileList == NULL || (location != 0 && location != 1))
    {
        return -2;
    }
    snprintf(args, SEND_BUF_LEN, "STORage:FIL:MOT:%s?\n", (location == 0) ? "LOC" : "EXTER");
    if ((retlen = busQuery(vi, args, strlen(args), ps8FileList, len)) == 0) {
        return -1;
    }
    return retlen;
}

EXPORT_API int CALL mrgStorageMotionFileDelete(ViSession vi, char* ps8FileName)
{
    char args[SEND_BUF_LEN];
    if (ps8FileName == NULL)
    {
        return -2;
    }
    snprintf(args, SEND_BUF_LEN, "STORage:FIL:MOT:DEL %s\n", ps8FileName);
    if ((busWrite(vi, args, strlen(args))) == 0) {
        return -1;
    }
    return 0;
}

EXPORT_API int CALL mrgStorageMotionFileContextRead(ViSession vi, char* ps8FileName, char* ps8Context)
{
    int retlen = 0;
    int count = 0;
    int lenOfLen = 0;
    int readLen = 0;
    int left = 0;
    char args[SEND_BUF_LEN];
    char as8Ret[RECV_BUF_LEN];
    char as8StrLen[20];
    if (ps8FileName == NULL || ps8Context == NULL)
    {
        return -2;
    }

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mutex);

    snprintf(args, SEND_BUF_LEN, "STORage:FILe:MOTion:CONTEXT:READ? %s\n", ps8FileName);
    if (busWrite(vi, args, strlen(args)) == 0)
    {
        pthread_mutex_unlock(&mutex);
        return 0;
    }
    // 1. 先读回一个#9的头。
    retlen = busRead(vi, as8Ret, 12);
    if (retlen <= 0)
    {
        pthread_mutex_unlock(&mutex);
        return 0;
    }
    if (as8Ret[0] != '#')//格式错误
    {
        pthread_mutex_unlock(&mutex);
        return count;
    }
    lenOfLen = as8Ret[1] - 0x30;
    memcpy(as8StrLen, &as8Ret[2], lenOfLen);//取出长度字符串
    left = strtoul(as8StrLen, NULL, 10);
    if (left == 0)
    {
        pthread_mutex_unlock(&mutex);
        return 0;
    }
    ps8Context[0] = as8Ret[11];
    count = 1;
    while (left >0)
    {
        readLen = (left > 512) ? 512 : left;
        //返回的#9数据最后，会有一个分号，所以这里多读一个字节。
        if ((retlen = busRead(vi, as8Ret, readLen)) == 0)
        {
            break;
        }
        memcpy(&ps8Context[count], as8Ret, retlen);
        count += retlen;
        left -= retlen;
    }
    pthread_mutex_unlock(&mutex);
    return count;
}

EXPORT_API int CALL mrgStorageMotionFileSaveContext(ViSession vi, char* ps8Context,int len, char * ps8SaveFileName)
{
    return mrgStorageWriteFile(vi, 0, "/home/megarobo/MRH-T/motionfile/", ps8SaveFileName, (unsigned char *)ps8Context, len);
}

EXPORT_API int CALL mrgStorageWriteFile(ViSession vi, int isUdisk, char *ps8Path, char *ps8SaveFileName,unsigned char *pu8Data, int dataLen)
{
    int retLen = -1;
    int count = 0;
    int errCount = 3;
    int len = dataLen;
    int writeLen = 0;
    int cmdLen = 0;
    char args[SEND_BUF_LEN];
    char as8State[RECV_BUF_LEN];
    if (pu8Data == NULL || ps8SaveFileName == NULL)
    {
        return -2;
    }

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mutex);

    memset(args, 0, sizeof(args));
    snprintf(args, SEND_BUF_LEN, "mkdir -p %s\n", ps8Path);
    mrgSystemRunCmd(vi, args, 0);

    memset(args, 0, sizeof(args));
    snprintf(args, SEND_BUF_LEN, "STORage:FILe:WRITe:START %s,%s,%s\n",isUdisk?"UDISK":"LOCAL", ps8Path, ps8SaveFileName);
    if (busWrite(vi, args, strlen(args)) == 0)//写入文件名
    {
        pthread_mutex_unlock(&mutex);
        return -3;
    }
    //写入文件内容
    while (len > 0)
    {
        writeLen = len > 512 ? 512 : len;
        memset(args, 0, sizeof(args));
        snprintf(args, sizeof(args), "STORage:FILe:WRITe:DATa? #9%09d", writeLen);
        cmdLen = strlen(args);
        memcpy(&args[cmdLen],&pu8Data[count], writeLen);

        if (busWrite(vi, args, writeLen + cmdLen) == 0)
        {
            pthread_mutex_unlock(&mutex);
            return -4;
        }

        errCount = 3;
        while(errCount--)
        {
            memset(as8State,0,sizeof(as8State));
            retLen = busRead(vi, as8State, sizeof(as8State));
            if (retLen > 0)
            {
                break;
            }
            msSleep(200);
        }
        if( retLen == 0 )
        {
            pthread_mutex_unlock(&mutex);
            return -5;
        }

        if(STRCASECMP(as8State, "ERROR") == 0)
        {
            continue;
        }
        len -= writeLen;
        count += writeLen;
    }
    memset(args, 0, sizeof(args));
    memset(as8State,0,sizeof(as8State));
    snprintf(args, SEND_BUF_LEN, "STORage:FILe:WRITe:END? %d\n", dataLen);
    retLen = busQuery(vi, args, strlen(args), as8State, sizeof(as8State));
    if (retLen == 0)//写入文件结束
    {
        pthread_mutex_unlock(&mutex);
        return -6;
    }
    if(STRCASECMP(as8State, "ERROR") == 0)
    {
        pthread_mutex_unlock(&mutex);
        return -7;
    }

//    memset(args, 0, sizeof(args));
//    snprintf(args, SEND_BUF_LEN, "chmod -R 777 %s\n", ps8Path);
//    mrgSystemRunCmd(vi, args, 0);
    pthread_mutex_unlock(&mutex);
    return 0;
}

EXPORT_API int CALL mrgStorageReadFile(ViSession vi, int isUdisk, char *ps8Path, char* ps8Filename, unsigned char* ps8Context)
{
    int retlen = 0;
    int count = 0;
    int lenOfLen = 0;
    int readLen = 0;
    int left = 0;
    char args[SEND_BUF_LEN];
    char as8Ret[RECV_BUF_LEN];
    char as8StrLen[20];
    if (ps8Path == NULL || ps8Context == NULL || ps8Filename == NULL)
    {
        return 0;
    }
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mutex);

//    snprintf(args, SEND_BUF_LEN, "SYSTEM:CMDLine? %s%s,%s\n", "chmod -R 777 ", ps8Path, "WAIT");
//    busQuery(vi, args, strlen(args), as8Ret, sizeof(as8Ret));

    snprintf(args, SEND_BUF_LEN, "STORage:FILe:READ? %s,%s,%s\n",
             isUdisk?"UDISK":"LOCAL", ps8Path, ps8Filename);

    if (busWrite(vi, args, strlen(args)) == 0)
    {
        pthread_mutex_unlock(&mutex);
        return 0;
    }
    // 1. 先读回一个#9的头。
    retlen = busRead(vi, as8Ret, 12);
    if (retlen <= 0)
    {
        pthread_mutex_unlock(&mutex);
        return 0;
    }
    if (as8Ret[0] != '#')//格式错误
    {
        pthread_mutex_unlock(&mutex);
        return count;
    }
    lenOfLen = as8Ret[1] - 0x30;
    memcpy(as8StrLen, &as8Ret[2], lenOfLen);//取出长度字符串
    left = strtoul(as8StrLen, NULL, 10);
    if (left == 0)
    {
        pthread_mutex_unlock(&mutex);
        return 0;
    }
    ps8Context[0] = as8Ret[11];
    count = 1;
    while (left >0)
    {
        readLen = (left > 512) ? 512 : left;
        //返回的#9数据最后，会有一个分号，所以这里多读一个字节。
        if ((retlen = busRead(vi, as8Ret, readLen)) == 0)
        {
            break;
        }
        memcpy(&ps8Context[count], as8Ret, retlen);
        count += retlen;
        left -= retlen;
    }
    pthread_mutex_unlock(&mutex);
    return count;
}

EXPORT_API int CALL mrgStorageDirectoryEnum(ViSession vi, int isUdisk, const char *ps8Path, char* ps8FileList, int *fileListLen)
{
    int retlen = 0;
    int count = 0;
    int dataLen = 0;
    char args[SEND_BUF_LEN];
    char as8Ret[RECV_BUF_LEN];
    char as8StrLen[20];
    char *as8Buff = NULL;
    if (ps8Path == NULL || ps8FileList == NULL || *fileListLen == 0)
    {
        return 0;
    }

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mutex);

    snprintf(args, SEND_BUF_LEN, "STORage:DIRECTory:ENUM? %s,%s\n",
             isUdisk?"UDISK":"LOCAL", ps8Path);
    if (busWrite(vi, args, strlen(args)) == 0)
    {
        pthread_mutex_unlock(&mutex);
        return 0;
    }
    // 1. 先读回一个#9的头。
    retlen = busRead(vi, as8Ret, 12);
    if (retlen <= 0)
    {
        pthread_mutex_unlock(&mutex);
        return 0;
    }
    if (as8Ret[0] != '#')//格式错误
    {
        pthread_mutex_unlock(&mutex);
        return count;
    }
    dataLen = as8Ret[1] - 0x30;
    memcpy(as8StrLen, &as8Ret[2], dataLen);//取出长度字符串
    dataLen = strtoul(as8StrLen, NULL, 10);
    if (dataLen == 0)
    {
        pthread_mutex_unlock(&mutex);
        return 0;
    }

    as8Buff = (char *)malloc(dataLen+1);
    memset(as8Buff, 0, dataLen+1);

    count = 1;
    memcpy(as8Buff, &as8Ret[11], count);

    while (dataLen >0)
    {
        //返回的#9数据最后，会有一个分号，所以这里多读一个字节。
        if ((retlen = busRead(vi, as8Ret, ((dataLen > 512) ? 512 : dataLen) )) == 0)
        {
            break;
        }
        memcpy(as8Buff + count, as8Ret, retlen);
        count += retlen;
        dataLen -= retlen;
    }

    if(*fileListLen >= count)
    {
        memcpy(ps8FileList, as8Buff, count);
        *fileListLen = count;
    }
    else
    {
        memset(as8Buff, 0, count);
        *fileListLen = 0;
    }

    free(as8Buff);
    pthread_mutex_unlock(&mutex);
    return count;
}

EXPORT_API int CALL mrgStorageGetFileSize(ViSession vi, int isUdisk, const char *ps8Path, char* ps8Filename)
{
    int retlen = 0;
    char args[SEND_BUF_LEN];
    char as8Ret[RECV_BUF_LEN];
    if (ps8Path == NULL || ps8Filename == NULL)
    {
        return -2;
    }
    snprintf(args, SEND_BUF_LEN, "STORage:FILE:SIZE? %s,%s,%s\n",
             isUdisk?"UDISK":"LOCAL", ps8Path, ps8Filename);
    if ((retlen = busQuery(vi, args, strlen(args), as8Ret, sizeof(as8Ret))) == 0) {
        return -1;
    }
    retlen = atoi(as8Ret);
    return retlen;
}



//! **********************************************************************************
EXPORT_API int CALL mrgStorageWriteBigFile(ViSession vi, char *ps8Path, char *ps8SaveFileName,unsigned char *pu8Data, int dataLen)
{
    int retLen = -1;
    int count = 0;
    int len = dataLen;
    int writeLen = 0;
    int cmdLen = 0;
    char args[SEND_BUF_LEN];
    char as8Ret[RECV_BUF_LEN];
    char as8State[RECV_BUF_LEN];
    int fd = -1;
    int offset = 0;
    if (ps8Path == NULL || pu8Data == NULL || ps8SaveFileName == NULL)
    {
        return -2;
    }

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mutex);

    snprintf(args, SEND_BUF_LEN, "SYSTEM:CMDLine? mkdir -p %s,WAIT\n", ps8Path);
    if ((retLen = busQuery(vi, args, strlen(args), as8State, sizeof(as8State))) <= 0)
    {
        pthread_mutex_unlock(&mutex);
        return -1;
    }

    snprintf(args, SEND_BUF_LEN, "STORage:FILe:OPEN? %s,%s,WRITE\n", ps8Path, ps8SaveFileName);
    retLen = busQuery(vi, args, strlen(args), as8State, sizeof(as8State));
    fd = atoi(as8State);
    if (retLen <= 0 || fd <= 2)
    {
        pthread_mutex_unlock(&mutex);
        return -3;
    }

    //写入文件内容
    while (len > 0)
    {
        writeLen = len > 512 ? 512 : len;
        snprintf(as8Ret, sizeof(as8Ret), "STORage:FILe:WRITeDATa? %d,%d,#9%09d", fd, offset, writeLen);
        cmdLen = strlen(as8Ret);
        memcpy(&as8Ret[cmdLen],&pu8Data[count], writeLen);

        memset(as8State,0,sizeof(as8State));
        if (busQuery(vi, as8Ret, writeLen + cmdLen, as8State, sizeof(as8State)) > 0)
        {
            if(STRNCASECMP(as8State, "ERROR", strlen("ERROR")) == 0)
            {
                continue;
            }
            len -= writeLen;
            count += writeLen;
            offset += writeLen;
        }
    }
    int crc = makeCRC16(pu8Data, dataLen);

    snprintf(args, SEND_BUF_LEN, "STORage:FILe:CLOSE? %d,%d\n", fd, crc);
    memset(as8State,0,sizeof(as8State));
    retLen = busQuery(vi, args, strlen(args), as8State, sizeof(as8State));
    if (retLen == 0 || STRNCASECMP(as8State, "ERROR", strlen("ERROR")) == 0)//写入文件结束
    {
        pthread_mutex_unlock(&mutex);
        return -6;
    }

    pthread_mutex_unlock(&mutex);
    return 0;
}

EXPORT_API int CALL mrgStorageReadBigFile(ViSession vi, char *ps8Path, char* ps8Filename, unsigned char* ps8Context, int contextLen)
{
    int retlen = 0;
    int count = 0;
    int readLen = 0;
    int offset = 0;
    int len = 0;
    int fd = -1;
    int filesize = 0;
    char args[SEND_BUF_LEN];
    char as8Ret[RECV_BUF_LEN];

     char as8StrLen[20];
     int dataLen = 0;

    if (ps8Path == NULL || ps8Context == NULL || ps8Filename == NULL)
    {
        return -2;
    }
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mutex);

//    snprintf(args, SEND_BUF_LEN, "SYSTEM:CMDLine? chmod -R 777 %s,WAIT\n", ps8Path);
//    busQuery(vi, args, strlen(args), as8Ret, sizeof(as8Ret));

    filesize = mrgStorageGetFileSize(vi, 0, ps8Path, ps8Filename);
    if(filesize <= 0 || contextLen < filesize)
    {
        pthread_mutex_unlock(&mutex);
        return -1;
    }

    snprintf(args, SEND_BUF_LEN, "STORage:FILe:OPEN? %s,%s,READ\n", ps8Path, ps8Filename);
    retlen = busQuery(vi, args, strlen(args), as8Ret, sizeof(as8Ret));
    fd = atoi(as8Ret);
    if (retlen <= 0 || fd <= 2)
    {
        pthread_mutex_unlock(&mutex);
        return -3;
    }

    //写入文件内容
    len = filesize;
    while (len > 0)
    {
        readLen = len > 512 ? 512 : len;
        snprintf(args, sizeof(args), "STORage:FILe:READDATa? %d,%d,%d\n", fd, offset, readLen);
        if (0 == busQuery(vi, args, strlen(args), as8Ret, sizeof(as8Ret)))
        {
            pthread_mutex_unlock(&mutex);
            return -4;
        }

        if(STRNCASECMP(as8Ret, "ERROR", strlen("ERROR")) == 0 || as8Ret[0] != '#')
        {
            continue;
        }
        dataLen = as8Ret[1] - 0x30;
        memcpy(as8StrLen, &as8Ret[2], dataLen);//取出长度字符串
        dataLen = strtoul(as8StrLen, NULL, 10);
        if (dataLen == 0)
        {
            pthread_mutex_unlock(&mutex);
            return 0;
        }
        memcpy(ps8Context+offset, as8Ret + strlen("#9000000000"), dataLen);

        len -= readLen;
        count += readLen;
        offset += readLen;
    }

    int crc = makeCRC16(ps8Context, filesize);
    snprintf(args, SEND_BUF_LEN, "STORage:FILe:CLOSE? %d,%d\n", fd, crc);
    retlen = busQuery(vi, args, strlen(args), as8Ret, sizeof(as8Ret));
    if (retlen == 0)//写入文件结束
    {
        pthread_mutex_unlock(&mutex);
        return -6;
    }
    if(STRCASECMP(as8Ret, "ERROR") == 0)
    {
        pthread_mutex_unlock(&mutex);
        return -7;
    }

    pthread_mutex_unlock(&mutex);
    return filesize;
}
