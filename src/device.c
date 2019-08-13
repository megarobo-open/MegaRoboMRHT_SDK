#include "device.h"

EXPORT_API int CALL mrgFindGateWay(int mode, char *output, int len)
{
    return busFindDevice(mode, output, len);
}

EXPORT_API int CALL mrgOpenGateWay(int mode, char * desc, int timeout_ms)
{
    const int minTimeout = 200;//最小超时时间
    return busOpenDevice(mode, desc, (timeout_ms>minTimeout) ? timeout_ms : minTimeout);
}

EXPORT_API int CALL mrgCloseGateWay(ViSession  vi)
{
    return busCloseDevice(vi);
}

EXPORT_API int CALL mrgGateWaySendCmd(ViSession  vi, char* cmd, int len)
{
    return busWrite(vi, cmd, len);
}

EXPORT_API int CALL mrgGateWayRead(ViSession  vi, char * output, int wantlen)
{
    int retlen = 0;
    retlen = busRead(vi,output, wantlen>MAX_TRANSMIT_LEN?MAX_TRANSMIT_LEN:wantlen);
    return retlen;
}

EXPORT_API int CALL mrgGateWayQuery(ViSession  vi, char* cmd, char * output, int wantlen)
{
    int retlen = 0;
    retlen = busQuery(vi, cmd, strlen(cmd), output, wantlen);
    return retlen;
}

EXPORT_API int CALL mrgGateWayIDNQuery(ViSession  vi, char * idn)
{
    char args[SEND_BUF_LEN];
    int retlen = 0;
    char as8Ret[RECV_BUF_LEN] = "";
    snprintf(args, SEND_BUF_LEN, "*IDN?\n");
    if ((retlen = busQuery(vi, args, strlen(args), as8Ret, sizeof(as8Ret))) <= 0)
    {
        return -1;
    }
    strcpy(idn, as8Ret);
    return 0;
}

EXPORT_API int CALL mrgFindDevice(ViSession vi, int timeout_ms)
{
    char ret[RECV_BUF_LEN];
    char args[SEND_BUF_LEN];
    int retlen = 0;
    snprintf(args, SEND_BUF_LEN, "DEVICE:SEARCH\n");
    if (busWrite(vi, args, strlen(args)) <= 0)
    {
        return 0;
    }
    msSleep(timeout_ms);
    snprintf(args, SEND_BUF_LEN, "DEVICE:COUNT?\n");
    if ((retlen = busQuery(vi, args, strlen(args), ret, sizeof(ret))) <= 0)
    {
        return 0;
    }
    return atoi(ret);
}

EXPORT_API int CALL mrgGetDeviceName(ViSession vi, int * name)
{
    int retlen = 0;
    char args[SEND_BUF_LEN];
    char names[RECV_BUF_LEN];
    if (name == NULL)
    {
        return -2;
    }
    snprintf(args, SEND_BUF_LEN, "DEVICE:NAME?\n");
    if ((retlen = busQuery(vi, args, strlen(args), names, sizeof(names))) == 0)
    {
        return -1;
    }
    return splitStringToIntArray(names, ",", name);
}

EXPORT_API int CALL mrgGetDeviceType(ViSession vi, int name, char * ps8Type)
{
    char args[SEND_BUF_LEN];
    char as8Ret[RECV_BUF_LEN];
    int len = 0;
    if (ps8Type == NULL)
    {
        return -2;
    }
    snprintf(args, SEND_BUF_LEN, "DEVICE:TYPe? %d\n", name);
    if ((len = busQuery(vi, args, strlen(args), as8Ret, sizeof(as8Ret))) == 0)
    {
        return -1;
    }
    strcpy(ps8Type,as8Ret);
    return 0;
}

EXPORT_API int CALL mrgGetDeviceChannelCount(ViSession vi, int name)
{
    char args[SEND_BUF_LEN];
    int retLen = 0;
    char state[RECV_BUF_LEN];
    snprintf(args, SEND_BUF_LEN, "DEVice:CHANnel:COUNt? %d\n", name);
    if ((retLen = busQuery(vi, args, strlen(args), state, sizeof(state))) == 0) {
        return -1;
    }
    return atoi(state);
}

EXPORT_API int CALL mrgGetDeviceInfo(ViSession vi, int name, char * ps8Info)
{
    char args[SEND_BUF_LEN];
    char as8Ret[RECV_BUF_LEN];
    int retlen = 0;
    snprintf(args, SEND_BUF_LEN, "DEVICE:FIRMWARE:ALL? %d\n", name);
    if ((retlen = busQuery(vi, args, strlen(args), as8Ret, sizeof(as8Ret))) == 0) {
        return -1;
    }
    strcpy(ps8Info,as8Ret);
    return 0;
}

EXPORT_API int CALL mrgGetDeviceSoftVersion(ViSession vi, int name, char * ps8Version)
{
    char args[SEND_BUF_LEN];
    int len = 0;
    char as8Ret[RECV_BUF_LEN];
    snprintf(args, SEND_BUF_LEN, "DEVICE:FIRMWARE:SOFT? %d\n", name);
    if ((len = busQuery(vi, args, strlen(args), as8Ret, sizeof(as8Ret))) == 0) {
        return -1;
    }
    strcpy(ps8Version, as8Ret);
    return 0;
}

EXPORT_API int CALL mrgGetDeviceFirmWareHard(ViSession vi, int name, char *buf)
{
    char args[SEND_BUF_LEN];
    char as8Ret[RECV_BUF_LEN];
    int retLen = 0;
    snprintf(args, SEND_BUF_LEN, "DEVice:FIRMware:HARD? %d\n", name);
    if ((retLen = busQuery(vi, args, strlen(args), as8Ret, sizeof(as8Ret))) == 0) {
        return -1;
    }
    strcpy(buf, as8Ret);
    return 0;
}

EXPORT_API int CALL mrgGetDeviceFirmWareBoot(ViSession vi, int name, char *buf)
{
    char args[SEND_BUF_LEN];
    int retLen = 0;
    char as8Ret[RECV_BUF_LEN];
    snprintf(args, SEND_BUF_LEN, "DEVice:FIRMware:BOOT? %d\n", name);
    if ((retLen = busQuery(vi, args, strlen(args), as8Ret, RECV_BUF_LEN)) == 0) {
        return -1;
    }
    strcpy(buf, as8Ret);
    return 0;
}

EXPORT_API int CALL mrgGetDeviceFirmWareFpga(ViSession vi, int name, char *buf)
{
    char args[SEND_BUF_LEN];
    int retLen = 0;
    char as8Ret[RECV_BUF_LEN];
    snprintf(args, SEND_BUF_LEN, "DEVice:FIRMware:FPGA? %d\n", name);
    if ((retLen = busQuery(vi, args, strlen(args), as8Ret, RECV_BUF_LEN)) == 0) {
        return -1;
    }
    strcpy(buf, as8Ret);
    return 0;
}

EXPORT_API int CALL mrgGetDeviceSerialNumber(ViSession vi, int name, char * ps8Serial)
{
    char args[SEND_BUF_LEN];
    char as8Ret[RECV_BUF_LEN];
    int len = 0;
    snprintf(args, SEND_BUF_LEN, "DEVICE:FIRMWARE:SN? %d\n", name);
    if ((len = busQuery(vi, args, strlen(args), as8Ret, RECV_BUF_LEN)) == 0) {
        return -1;
    }
    strcpy(ps8Serial, as8Ret);
    return 0;
}

