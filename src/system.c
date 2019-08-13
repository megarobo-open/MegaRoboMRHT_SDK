#include "system.h"

EXPORT_API int CALL mrgIdentify(ViSession vi, int state)
{
    char args[SEND_BUF_LEN];
    snprintf(args, SEND_BUF_LEN, "SYSTEM:IDENTIFY %s\n", state ? "ON" : "OFF");
    if (busWrite(vi, args, strlen(args)) <= 0)
    {
        return -1;
    }
    return 0;
}

EXPORT_API int CALL mrgSysGetProjectSoftVersion(ViSession vi, char * version)
{
    char args[SEND_BUF_LEN];
    char ret[RECV_BUF_LEN] = { 0 };
    int retlen = 0;
    snprintf(args, SEND_BUF_LEN, ":PROJect:SOFTware?\n");
    if ((retlen = busQuery(vi, args, strlen(args), ret, sizeof(ret))) <= 0)
    {
        return -1;
    }
    memcpy(version, ret, retlen);
    return 0;
}

EXPORT_API int CALL mrgSysGetSoftVersion(ViSession vi, char * version)
{
    char args[SEND_BUF_LEN];
    char ret[RECV_BUF_LEN] = { 0 };
    int retlen = 0;
    snprintf(args, SEND_BUF_LEN, "SYSTEM:FIRMware:SOFT?\n");
    if ((retlen = busQuery(vi, args, strlen(args), ret, sizeof(ret))) <= 0)
    {
        return -1;
    }
    memcpy(version, ret, retlen);
    return 0;
}

EXPORT_API int CALL mrgSysGetBackBoardSoftVersion(ViSession vi,char * version)
{
    char args[SEND_BUF_LEN];
    char ret[RECV_BUF_LEN] = { 0 };
    int retlen = 0;
    snprintf(args, SEND_BUF_LEN, "SYSTEM:FIRMware:BACKBOARD:SOFT?\n");
    if ((retlen = busQuery(vi, args, strlen(args), ret, sizeof(ret))) <= 0)
    {
        return -1;
    }
    memcpy(version, ret, retlen);
    return 0;
}

EXPORT_API int CALL mrgSysGetBackBoardHardVersion(ViSession vi, char * version)
{
    char args[SEND_BUF_LEN];
    char ret[RECV_BUF_LEN] = { 0 };
    int retlen = 0;
    snprintf(args, SEND_BUF_LEN, "SYSTEM:FIRMware:BACKBOARD:HARD?\n");
    if ((retlen = busQuery(vi, args, strlen(args), ret, sizeof(ret))) <= 0)
    {
        return -1;
    }
    memcpy(version, ret, retlen);
    return 0;
}

EXPORT_API int CALL mrgSysStartCan(ViSession vi)
{
    char args[SEND_BUF_LEN];
    snprintf(args, SEND_BUF_LEN, "CAN:STARt\n");
    if (busWrite(vi, args, strlen(args)) <= 0)
    {
        return -1;
    }
    return 0;
}

EXPORT_API int CALL mrgSysResetCan(ViSession vi)
{
    char args[SEND_BUF_LEN];
    snprintf(args, SEND_BUF_LEN, "CAN:RESET\n");
    if (busWrite(vi, args, strlen(args)) <= 0)
    {
        return -1;
    }
    return 0;
}

EXPORT_API int CALL mrgSysSetCanConfig(ViSession vi,int baud)
{
    char args[SEND_BUF_LEN];
    snprintf(args, SEND_BUF_LEN, "CAN:CONFig %d\n", baud);
    if (busWrite(vi, args, strlen(args)) <= 0)
    {
        return -1;
    }
    return 0;
}

EXPORT_API int CALL mrgSysGetCanConfig(ViSession vi, int* baud)
{
    char args[SEND_BUF_LEN];
    char ret[RECV_BUF_LEN] = { 0 };
    int retlen = 0;
    snprintf(args, SEND_BUF_LEN, "CAN:CONFig?\n");
    if ((retlen = busQuery(vi, args, strlen(args), ret, sizeof(ret))) <= 0)
    {
        return -1;
    }
    *baud = atoi(ret);
    return 0;
}

EXPORT_API int CALL mrgSysSetRunMode(ViSession vi, int mode)
{
    char args[SEND_BUF_LEN];
    snprintf(args, SEND_BUF_LEN, "SYSTEM:MODe:SWITch %d\n", mode);
    if (busWrite(vi, args, strlen(args)) <= 0)
    {
        return -1;
    }
    return 0;
}

EXPORT_API int CALL mrgSysGetRunMode(ViSession vi)
{
    char args[SEND_BUF_LEN];
    char ret[RECV_BUF_LEN] = {0};
    int retlen = 0;
    snprintf(args, SEND_BUF_LEN, "SYSTEM:MODe:SWITch?\n");
    if ((retlen = busQuery(vi, args, strlen(args),ret,sizeof(ret))) <= 0)
    {
        return -1;
    }
    return atoi(ret);
}

EXPORT_API int CALL mrgSysSetInstructionMode(ViSession vi, int mode)
{
    char args[SEND_BUF_LEN];
    snprintf(args, SEND_BUF_LEN, "SYSTEM:INSTRUCTion:MODe %s\n", mode? "SECTION":"LINE");
    if (busWrite(vi, args, strlen(args)) <= 0)
    {
        return -1;
    }
    return 0;
}

EXPORT_API int CALL mrgSysGetInstructionMode(ViSession vi, int* mode)
{
    char args[SEND_BUF_LEN];
    char ret[RECV_BUF_LEN] = { 0 };
    int retlen = 0;
    snprintf(args, SEND_BUF_LEN, "SYSTEM:INSTRUCTion:MODe?\n");
    if ((retlen = busQuery(vi, args, strlen(args), ret, sizeof(ret))) <= 0)
    {
        return -1;
    }
    if (STRCASECMP(ret, "LINE") == 0)
    {
        *mode = Instruction_LINE;
    }
    else if (STRCASECMP(ret, "SECTION") == 0)
    {
        *mode = Instruction_SECTION;
    }
    else
    {
        return -2;
    }
    return 0;
}

EXPORT_API int CALL mrgSysWifiScan(ViSession vi, char* result,int len)
{
    char args[SEND_BUF_LEN];
    char as8Ret[RECV_BUF_LEN];
    int retLen = 0;
    snprintf(args, SEND_BUF_LEN, "SYSTEM:NETWORK:WIFI:SEARCH?\n");
    if ((retLen = busQuery(vi, args, strlen(args),as8Ret,sizeof(as8Ret))) <= 0)
    {
        return 0;
    }
    retLen = (len > retLen) ? retLen : len;
    memcpy(result, as8Ret,retLen);
    return retLen;
}

EXPORT_API int CALL mrgSysWifiConfig(ViSession vi, char* name, char* passwd)
{
    char args[SEND_BUF_LEN];
    snprintf(args, SEND_BUF_LEN, "SYSTEM:NETWORK:WIFI:CONFig %s,%s\n", name, passwd);
    if (busWrite(vi, args, strlen(args)) <= 0)
    {
        return -1;
    }
    return 0;
}

EXPORT_API int CALL mrgSysWifiUpDown(ViSession vi, int state)
{
    char args[SEND_BUF_LEN];
    snprintf(args, SEND_BUF_LEN, "SYSTEM:NETWORK:WIFI:%s\n", state?"UP":"DOWN");
    if (busWrite(vi, args, strlen(args)) <= 0)
    {
        return -1;
    }
    return 0;
}

EXPORT_API int CALL mrgSysWifiCheckState(ViSession vi, int timeout_ms)
{
    char args[SEND_BUF_LEN];
    char as8Ret[RECV_BUF_LEN];
    int retLen = 0;
    int time = 0;
    snprintf(args, SEND_BUF_LEN, "SYSTEM:NETWORK:WIFI:STATe?\n");
    while (time < timeout_ms)
    {
        if ((retLen = busQuery(vi, args, strlen(args), as8Ret, sizeof(as8Ret))) <= 0)
        {
            continue;
        }
        if (STRCASECMP(as8Ret, "ON") == 0)
        {
            return 1;
        }
        msSleep(100);
        time += 100;
    }
    return 0;
}

EXPORT_API int CALL mrgSysGetWifiConnected(ViSession vi, char* name)
{
    char args[SEND_BUF_LEN];
    char as8Ret[RECV_BUF_LEN];
    int retLen = 0;
    snprintf(args, SEND_BUF_LEN, "SYSTEM:NETWORK:WIFI:NAMe?\n");

    if ((retLen = busQuery(vi, args, strlen(args), as8Ret, sizeof(as8Ret))) <= 0)
    {
        return -1;
    }
    strncpy(name, as8Ret,sizeof(as8Ret));
    return 0;
}

EXPORT_API int CALL mrgSysSetHeartPeriod(ViSession vi, int value)
{
    char args[SEND_BUF_LEN];
    snprintf(args, SEND_BUF_LEN, "SYSTEM:HEARTBEAT:VALUE %d\n", value);
    if (busWrite(vi, args, strlen(args)) <= 0)
    {
        return -1;
    }
    return 0;
}

EXPORT_API int CALL mrgSysGetHeartPeriod(ViSession vi, int * value)
{
    char args[SEND_BUF_LEN];
    char as8Ret[RECV_BUF_LEN];
    int retLen = 0;
    snprintf(args, SEND_BUF_LEN, "SYSTEM:HEARTBEAT:VALUE?\n");

    if ((retLen = busQuery(vi, args, strlen(args), as8Ret, sizeof(as8Ret))) <= 0)
    {
        return -1;
    }
    *value = atoi(as8Ret);
    return 0;
}

EXPORT_API int CALL mrgSysSetPowerOn(ViSession vi, int value)
{
    char args[SEND_BUF_LEN];
    snprintf(args, SEND_BUF_LEN, "SYSTEM:POWERON %s\n", value == 0? "DEFAULT":"LAST");
    if (busWrite(vi, args, strlen(args)) <= 0)
    {
        return -1;
    }
    return 0;
}

EXPORT_API int CALL mrgSysGetPowerOn(ViSession vi, int * value)
{
    char args[SEND_BUF_LEN];
    char as8Ret[RECV_BUF_LEN];
    int retLen = 0;
    snprintf(args, SEND_BUF_LEN, "SYSTEM:POWERON?\n");

    if ((retLen = busQuery(vi, args, strlen(args), as8Ret, sizeof(as8Ret))) <= 0)
    {
        return -1;
    }
    if (STRCASECMP(as8Ret, "DEFAULT") == 0 || STRCASECMP(as8Ret, "0") == 0)
    {
        *value = PowerOn_DEFAULT;
    }
    else if (STRCASECMP(as8Ret, "LAST") == 0 || STRCASECMP(as8Ret, "1") == 0)
    {
        *value = PowerOn_LAST;
    }
    else
    {
        return -2;
    }
    return 0;
}

EXPORT_API int CALL mrgSysSetInterface(ViSession vi, int face)
{
    char args[SEND_BUF_LEN];
    char *ps8Interface[3] = { "LAN", "USB", "IO" };
    snprintf(args, SEND_BUF_LEN, "SYSTEM:INTERFACE %s\n", ps8Interface[face]);
    if (busWrite(vi, args, strlen(args)) <= 0)
    {
        return -1;
    }
    return 0;
}

EXPORT_API int CALL mrgSysGetInterface(ViSession vi, int * face)
{
    char args[SEND_BUF_LEN];
    char as8Ret[RECV_BUF_LEN];
    int retLen = 0;
    snprintf(args, SEND_BUF_LEN, "SYSTEM:INTERFACE?\n");

    if ((retLen = busQuery(vi, args, strlen(args), as8Ret, sizeof(as8Ret))) <= 0)
    {
        return -1;
    }
    if (STRCASECMP(as8Ret, "LAN") == 0 || STRCASECMP(as8Ret, "0") == 0)
    {
        *face = Interface_LAN;
    }
    else if (STRCASECMP(as8Ret, "USB") == 0 || STRCASECMP(as8Ret, "1") == 0)
    {
        *face = Interface_USB;
    }
    else if (STRCASECMP(as8Ret, "IO") == 0 || STRCASECMP(as8Ret, "2") == 0)
    {
        *face = Interface_IO;
    }
    else
    {
        return -2;
    }
    return 0;
}
EXPORT_API int CALL mrgSysSetDateTime(ViSession vi, char *ps8Date)
{
    char args[SEND_BUF_LEN];
    if (ps8Date == NULL)
    {
        return -2;
    }
    snprintf(args, SEND_BUF_LEN, "SYSTEM:DATE %s\n", ps8Date);
    if (busWrite(vi, args, strlen(args)) <= 0)
    {
        return -1;
    }
    return 0;
}
EXPORT_API int CALL mrgSysGetDateTime(ViSession vi, char *ps8Date)
{
    int retLen = 0;
    char as8Ret[RECV_BUF_LEN];
    char args[SEND_BUF_LEN];
    if (ps8Date == NULL)
    {
        return -1;
    }
    snprintf(args, SEND_BUF_LEN, "SYSTEM:DATE?\n");
    if ((retLen = busQuery(vi, args, strlen(args), as8Ret, sizeof(as8Ret))) <= 0)
    {
        return -1;
    }
    memcpy(ps8Date, as8Ret, retLen - 1);
    return 0;
}
EXPORT_API int CALL mrgSysSetEmergencyStopState(ViSession vi, int state)
{
    char args[SEND_BUF_LEN];
    snprintf(args, SEND_BUF_LEN, "SCRAM:STATE %s\n", state?"ON":"OFF");
    if (busWrite(vi, args, strlen(args)) <= 0)
    {
        return -1;
    }
    return 0;
}

EXPORT_API int CALL mrgSysGetEmergencyStopState(ViSession vi,int *state)
{
    char args[SEND_BUF_LEN];
    char as8Ret[RECV_BUF_LEN];
    int retLen = 0;
    snprintf(args, SEND_BUF_LEN, "SCRAM:STATE?\n");

    if ((retLen = busQuery(vi, args, strlen(args), as8Ret, sizeof(as8Ret))) <= 0)
    {
        return -1;
    }
    if (STRCASECMP(as8Ret, "OFF") == 0 || STRCASECMP(as8Ret, "0") == 0)
    {
        *state = SWITCH_OFF;
    }
    else if (STRCASECMP(as8Ret, "ON") == 0 || STRCASECMP(as8Ret, "1") == 0)
    {
        *state = SWITCH_ON;
    }
    else
    {
        return -2;
    }
    return 0;
}


EXPORT_API int CALL mrgSysUpdateFileSearch(ViSession vi, char *fileList)
{
    char args[SEND_BUF_LEN];
    char ret[RECV_BUF_LEN] = {0};
    int retlen = 0;
    snprintf(args, SEND_BUF_LEN, "SYST:UPGR:SEAR?\n");
    if ((retlen = busQuery(vi, args, strlen(args),ret,sizeof(ret))) <= 0)
    {
        return -1;
    }
    if( STRCASECMP(ret, "NO_UDISK") == 0 )
        return 1;
    memcpy(fileList, ret, retlen);
    return 0;
}

EXPORT_API int CALL mrgSysUpdateFileStart(ViSession vi, char *filename)
{
    char args[SEND_BUF_LEN];
    snprintf(args, SEND_BUF_LEN, "SYST:UPGR:STAR %s\n", filename);
    if (busWrite(vi, args, strlen(args)) == 0) {
        return -1;
    }

    int time = 0, intervalTime = 20;
    while (1)
    {
        if (time > (20 * 1000) )
        {
            return 1;
        }
        int ret = mrgSysUpdateFirmwareStatusQuery(vi);
        if(ret <= 0)
        {
            return ret;
        }
        if(ret == 1)
        {
            msSleep(intervalTime);
            time += intervalTime;
            continue;
        }
    }
}

EXPORT_API int CALL mrgSysUpdateFirmwareStatusQuery(ViSession vi)
{
    char args[SEND_BUF_LEN];
    char state[RECV_BUF_LEN] = {0};
    int retlen = 0;
    snprintf(args, SEND_BUF_LEN, "SYST:UPGR:STAT?\n");
    if ((retlen = busQuery(vi, args, strlen(args),state,sizeof(state))) <= 0)
    {
        return -1;
    }
    if( STRCASECMP(state, "COMPLETED") == 0 )
        return 0;
    else if( STRCASECMP(state, "UPDATING") == 0 )
        return 1;
    else if( STRCASECMP(state, "FAILD") == 0 )
        return -2;
    else
        return -3;
}

EXPORT_API int CALL mrgScriptSearch(ViSession vi, int isUdisk, char *fileList)
{
    char args[SEND_BUF_LEN];
    char ret[RECV_BUF_LEN] = {0};
    int retlen = 0;
    snprintf(args, SEND_BUF_LEN, "SCRipt:SEARCH? %s\n", isUdisk?"UDISK":"LOCAL");
    if ((retlen = busQuery(vi, args, strlen(args),ret,sizeof(ret))) <= 0)
    {
        return -1;
    }
    strncpy(fileList, ret, SEND_BUF_LEN);
    if( STRCASECMP(ret, "NO_UPGRADE_FILE") == 0 )
        return -2;

    else if( STRCASECMP(ret, "NO_UDISK") == 0 )
        return -3;

    return 0;
}

EXPORT_API int CALL mrgScriptSetConfig(ViSession vi, char *filename, int isBoot)
{
    char args[SEND_BUF_LEN];
    snprintf(args, SEND_BUF_LEN, "SCRipt:CONFIG %s,%s\n", filename, isBoot?"ON":"OFF");
    if (busWrite(vi, args, strlen(args)) <= 0)
    {
        return -1;
    }
    return 0;
}

EXPORT_API int CALL mrgScriptGetConfig(ViSession vi, char *filename)
{
    char args[SEND_BUF_LEN];
    char ret[SEND_BUF_LEN];
    int retlen = 0;
    snprintf(args, SEND_BUF_LEN, "SCRipt:CONFIG?\n");
    if ((retlen = busQuery(vi, args, strlen(args), ret, RECV_BUF_LEN)) <= 0) {
        return -1;
    }
    strcpy(filename, ret);
    return 0;
}

EXPORT_API int CALL mrgScriptRun(ViSession vi)
{
    char args[SEND_BUF_LEN];
    snprintf(args, SEND_BUF_LEN, "SCRipt:START\n");
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mutex);
    if (busWrite(vi, args, strlen(args)) <= 0)
    {
        pthread_mutex_unlock(&mutex);
        return -1;
    }
    msSleep(1000);
    pthread_mutex_unlock(&mutex);
    return 0;
}

EXPORT_API int CALL mrgScriptStop(ViSession vi)
{
    char args[SEND_BUF_LEN];
    snprintf(args, SEND_BUF_LEN, "SCRipt:STOP\n");
    if (busWrite(vi, args, strlen(args)) <= 0)
    {
        return -1;
    }
    return 0;
}

EXPORT_API int CALL mrgScriptGetCurrentStates(ViSession vi)
{
    char args[SEND_BUF_LEN];
    char state[RECV_BUF_LEN] = {0};
    int retlen = 0;
    snprintf(args, SEND_BUF_LEN, "SCRipt:RUNState?\n");
    if ((retlen = busQuery(vi, args, strlen(args),state,sizeof(state))) <= 0)
    {
        return -1;
    }
    if( STRCASECMP(state, "STOP") == 0 )
        return Script_STOP;
    else if( STRCASECMP(state, "RUNNING") == 0 )
        return Script_RUNNING;
    else
        return -2;
}

EXPORT_API int CALL mrgSystemRunCmd(ViSession vi, char *cmd, int isBackground)
{
    char args[SEND_BUF_LEN];
    char state[RECV_BUF_LEN] = {0};
    int retlen = 0;
    snprintf(args, SEND_BUF_LEN, "SYSTEM:CMDLine? %s,%s\n", cmd, isBackground?"NOWAIT":"WAIT");

    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mutex);

    if ((retlen = busQuery(vi, args, strlen(args), state, sizeof(state))) <= 0)
    {
        msSleep(2000);
        retlen = busRead(vi, state, sizeof(state));
        if(retlen <= 0)
        {
            pthread_mutex_unlock(&mutex);
            return -1;
        }
    }
    pthread_mutex_unlock(&mutex);
    msSleep(100);
    if( !isBackground && STRCASECMP(state, "ERROR") == 0 )
        return -2;
    else
        return 0;
}

EXPORT_API int CALL mrgSystemSetMRQConfig(ViSession vi, const char *version, const char *sn)
{
    char args[SEND_BUF_LEN];
    snprintf(args, SEND_BUF_LEN, ":SYSTem:CONFIG:MRQInfo %s,%s\n", version, sn);
    if (busWrite(vi, args, strlen(args)) <= 0)
    {
        return -1;
    }
    return 0;
}

EXPORT_API int CALL mrgSystemGetMRQConfig(ViSession vi, char *version, char *sn)
{
    char args[SEND_BUF_LEN];
    char state[RECV_BUF_LEN] = {0};
    int retlen = 0;
    char strVal[16][64] = {""};
    snprintf(args, SEND_BUF_LEN, ":SYSTem:CONFIG:MRQInfo?\n");
    if ((retlen = busQuery(vi, args, strlen(args), state, sizeof(state))) <= 0)
    {
        return -1;
    }
    splitString(state, ",", strVal);
    strcpy(version, strVal[0]);
    strcpy(sn, strVal[1]);
    return 0;
}

EXPORT_API int CALL mrgSysErrorSetAlertConf(ViSession vi, unsigned int errorCode, int isAlert)
{
    char args[SEND_BUF_LEN];
    snprintf(args, SEND_BUF_LEN, "SYSTEM:ERRCOnf:ALERT 0x%08X,%s\n", errorCode, isAlert?"ON":"OFF");
    if (busWrite(vi, args, strlen(args)) <= 0)
    {
        return -1;
    }
    return 0;
}

EXPORT_API int CALL mrgSysErrorGetAlertConf(ViSession vi, unsigned int errorCode)
{
    char args[SEND_BUF_LEN];
    char ret[SEND_BUF_LEN];
    int retlen = 0;
    snprintf(args, SEND_BUF_LEN, "SYSTEM:ERRCOnf:ALERT? 0x%08X\n", errorCode);
    if ((retlen = busQuery(vi, args, strlen(args), ret, RECV_BUF_LEN)) <= 0) {
        return -1;
    }
    if(0 == STRCASECMP(ret, "ERROR") )
        return -2;
    if(0 == STRCASECMP(ret, "ON") )
        return 1;
    if(0 == STRCASECMP(ret, "OFF") )
        return 0;
    return -3;
}

EXPORT_API int CALL mrgSysErrorSetOutputConf(ViSession vi, unsigned int errorCode, int isCloseOutput)
{
    char args[SEND_BUF_LEN];
    snprintf(args, SEND_BUF_LEN, "SYSTEM:ERRCOnf:OUTPUT 0x%08X,%s\n", errorCode, isCloseOutput?"ON":"OFF");
    if (busWrite(vi, args, strlen(args)) <= 0)
    {
        return -1;
    }
    return 0;
}

EXPORT_API int CALL mrgSysErrorGetOutputConf(ViSession vi, unsigned int errorCode)
{
    char args[SEND_BUF_LEN];
    char ret[SEND_BUF_LEN];
    int retlen = 0;
    snprintf(args, SEND_BUF_LEN, "SYSTEM:ERRCOnf:OUTPUT? 0x%08X\n", errorCode);
    if ((retlen = busQuery(vi, args, strlen(args), ret, RECV_BUF_LEN)) <= 0) {
        return -1;
    }
    if(0 == STRCASECMP(ret, "ERROR") )
        return -2;
    if(0 == STRCASECMP(ret, "ON") )
        return 1;
    if(0 == STRCASECMP(ret, "OFF") )
        return 0;
    return -3;
}

EXPORT_API int CALL mrgSystemErrorAck(ViSession vi)
{
    char args[SEND_BUF_LEN];
    int retlen = 0;
    snprintf(args, SEND_BUF_LEN, "SYSTEM:ERROR:ANSWER\n");
    if ((retlen = busWrite(vi, args, strlen(args))) == 0) {
        return -1;
    }
    return 0;
}
