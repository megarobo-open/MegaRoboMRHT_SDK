#include "project.h"

EXPORT_API int CALL mrgSetProjectMode(ViSession vi, int state)
{
    char args[SEND_BUF_LEN];
    if (state != 0 && state != 1)
    {
        return -2;
    }
    snprintf(args, SEND_BUF_LEN, "PROJect:STATe %s\n", state ? "ON" : "OFF");
    if (busWrite(vi, args, strlen(args)) <= 0)
    {
        return -1;
    }
    return 0;
}

EXPORT_API int CALL mrgProjectIOGet(ViSession vi, int index, char *strState)
{
    char args[SEND_BUF_LEN];
    char as8Ret[RECV_BUF_LEN];
    int retLen = 0;
    char *ps8IOtables[] = {"ALL","X1","X2","X3","X4","X5","X6","X7","X8","X9","X10",
                           "Y1","Y2","Y3","Y4","STOP", "DB15"};
    if (index >= IOGET_MAXNUM || index < 0 || strState == NULL)
    {
        return -2;
    }
    snprintf(args, SEND_BUF_LEN, "PROJect:XREAD? %s\n",ps8IOtables[index]);
    if ((retLen = busQuery(vi, args, strlen(args), as8Ret, sizeof(as8Ret))) <= 0)
    {
        return -1;
    }
    strcpy(strState, as8Ret);
    return 0;
}

EXPORT_API int CALL mrgProjectIOGetAll(ViSession vi, int *state)
{
    char args[SEND_BUF_LEN];
    char as8Ret[RECV_BUF_LEN];
    int retLen = 0;
   
    snprintf(args, SEND_BUF_LEN, "PROJect:XREAD? 0\n");
    if ((retLen = busQuery(vi, args, strlen(args), as8Ret, sizeof(as8Ret))) <= 0)
    {
        return -1;
    }
    *state = atoi(as8Ret);
    return 0;
}

EXPORT_API int CALL mrgProjectIOSet(ViSession vi, int index, int state,int mask)
{
    char args[SEND_BUF_LEN];
    char *ps8OutTable[] = {"ALL","Y1","Y2","Y3","Y4","READY","FAULT","ACK","MC","ENABLED"};
    if (index >= IOSET_MAXNUM || index < 0)
    {
        return -2;
    }
    if (index == 0)
    {
        snprintf(args, SEND_BUF_LEN, "PROJect:YWRITE ALL,%d,%d\n",state, mask);
    }
    else
    {
        snprintf(args, SEND_BUF_LEN, "PROJect:YWRITE %s,%s\n", ps8OutTable[index], (state&0x01) ? "H" : "L");
    }
    
    if (busWrite(vi, args, strlen(args)) <= 0)
    {
        return -1;
    }
    return 0;
}

EXPORT_API int CALL mrgProjectSetSerialNum(ViSession vi, char * ps8Serial)
{
    char args[SEND_BUF_LEN];
    if (ps8Serial == NULL)
    {
        return -2;
    }
    snprintf(args, SEND_BUF_LEN, "PROJect:SN %s\n", ps8Serial);
    if (busWrite(vi, args, strlen(args)) <= 0)
    {
        return -1;
    }
    return 0;
}

EXPORT_API int CALL mrgProjectGetSerialNum(ViSession vi, char * ps8Serial)
{
    char args[SEND_BUF_LEN];
    char as8Ret[RECV_BUF_LEN];
    int retLen = 0;
    snprintf(args, SEND_BUF_LEN, "PROJect:SN?\n");
    if (ps8Serial == NULL)
    {
        return 0;
    }
    if ((retLen = busQuery(vi, args, strlen(args), as8Ret, sizeof(as8Ret))) <= 0)
    {
        return 0;
    }
    strcpy(ps8Serial, as8Ret);
    return retLen-1;
}

EXPORT_API int CALL mrgWriteDeviceSerial(ViSession  vi, int name, char * ps8Serial)
{
    char args[SEND_BUF_LEN];
    if (ps8Serial == NULL)
    {
        return -2;
    }
    snprintf(args, SEND_BUF_LEN, "PROJECT:DEVICE:SN %d,%s\n", name, ps8Serial);
    if (busWrite(vi, args, strlen(args)) <= 0)
    {
        return -1;
    }
    return 0;
}
#if 0
EXPORT_API int CALL mrgCanTestStart(ViSession  vi, int name)
{
    char args[SEND_BUF_LEN];
    snprintf(args, SEND_BUF_LEN, "DEBUG:CAN:TEST:START %d\n", name);
    if (busWrite(vi, args, strlen(args)) <= 0)
    {
        return -1;
    }
    return 0;
}
EXPORT_API int CALL mrgCanTestStop(ViSession  vi, int name)
{
    char args[SEND_BUF_LEN];
    snprintf(args, SEND_BUF_LEN, "DEBUG:CAN:TEST:STOP %d\n", name);
    if (busWrite(vi, args, strlen(args)) <= 0)
    {
        return -1;
    }
    return 0;
}
EXPORT_API int CALL mrgGetCanTestResult(ViSession  vi, int name,unsigned int * pu32Send, unsigned int * pu32Rec, unsigned int * pu32Lost, unsigned int * pu32Time_ms)
{
    char as8Ret[100];
    int retLen = 0;
    char args[SEND_BUF_LEN];
    char strVal[16][64] = {""};
    snprintf(args, SEND_BUF_LEN, "DEBUG:CAN:TEST:INFO? %d\n", name);
    if (pu32Send == NULL || pu32Rec == NULL || pu32Lost == NULL || pu32Time_ms== NULL)
    {
        return -2;
    }
    if ((retLen = busQuery(vi, args, strlen(args), as8Ret, 100)) <= 0)
    {
        return -1;
    }

    splitString(as8Ret, ",", strVal);

    *pu32Send = strtoul(strVal[0], NULL, 0);
    *pu32Rec = strtoul(strVal[1], NULL, 0);
    *pu32Lost = strtoul(strVal[2], NULL, 0);
    *pu32Time_ms = strtoul(strVal[3], NULL, 0);

    return 0;
}
#endif
