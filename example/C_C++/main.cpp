
#include "MegaGateway.h"
#include <stdio.h>

int main()
{
    int i = 0;
    char *p = NULL, *pNext = NULL;
    int count = 0;
    int fd = 0;
    int ret = 0;

    char  buff[4096] = "";
    char  mrhtDescList[20][64] = {""};
    char  mrhtDesc[64] = "";
    char  IDN[64] = "";
    int   robotNames[64] = {0};
    int   robotName = 0;
    int   robotType = 0;
    int   deviceList[32];
    int   deviceName = 0;
    char  deviceType[32] = "";
    int   channelCount = 0;
    float posx=0, posy=0, posz=0;

    //! 查找网关
    mrgFindGateWay(BUS_VXI, buff, sizeof(buff));

    p = STRTOK_S(buff, ",", &pNext);
    while (p){
        strcpy(mrhtDescList[count++], p);
        p = STRTOK_S(NULL, ",", &pNext);
    }

    //! 显示网关列表
    for(i=0; i<count; i++)
    {
        memset(IDN, 0, sizeof(IDN));
        fd = mrgOpenGateWay(BUS_VXI, mrhtDescList[i], 800);
        if(fd <= 0)
            continue;

        if(-1 == mrgGateWayIDNQuery(fd, IDN))
        {
            mrgCloseGateWay(fd);
            continue;
        }
        mrgCloseGateWay(fd);
        printf(" %02d: %s [%s]\n", i, mrhtDescList[i], IDN);
    }
    if(count <= 0){
        printf("Search MRH-T is empty!!!\n");
        return -1;
    }

    //! 选择网关
    if(count == 1)
    {
        strcpy(mrhtDesc, mrhtDescList[0]);
    }
    else
    {
        do
        {
            i = -1;
            printf("Please select MRH-T: ");
            scanf("%d",&i);
            msSleep(1000);
        }while(i < 0 || i >= count);
        strcpy(mrhtDesc, mrhtDescList[i]);
    }

    //! 打开网关
    fd =  (int)mrgOpenGateWay(BUS_VXI, mrhtDesc, 800);
    printf("device open: %d\n", fd);

    //! 获取网关识别码
    memset(IDN, 0, sizeof(IDN));
    mrgGateWayIDNQuery(fd, IDN);
    printf("MRH-T IDN: %s\n", IDN);

    //! 获取机器人
    count = mrgGetRobotName(fd, robotNames);
    if(count <= 0){
        printf("Search Robot is empty!!!\n");
        return -2;
    }
    for(i=0; i<count; i++){
        robotType = mrgGetRobotType(fd, robotNames[i]);
        printf("%02d: %d, type: %d\n", i, robotNames[i], robotType);
    }

    //! 选择机器人
    if(count == 1)
    {
        i = 0;
    }
    else
    {
        do
        {
            i = -1;
            printf("Please select Robot: ");
            scanf("%d",&i);
            msSleep(1000);
        }while(i < 0 || i >= count);
    }

    robotName = robotNames[i];
    printf("robot name: %d\n", robotName);
    robotType = mrgGetRobotType(fd, robotName);


    //! 获取设备
    count = mrgGetRobotDevice(fd, robotName, deviceList);
    if(count <= 0){
        printf("Search MRQ is empty!!!\n");
        return -3;
    }
    for(i=0; i<count; i++){
        printf("Find MRQ Device[%d]: %d\n", i, deviceList[i]);
    }
    deviceName = deviceList[0];
    printf("deviceName name: %d\n", deviceName);

    mrgGetDeviceType(fd, deviceName, deviceType);
    printf( "device type: %s\n", deviceType );

    channelCount = mrgGetDeviceChannelCount(fd, deviceName);
    printf( "device channel count: %d\n", channelCount );

#define PRINT_POS() \
    do \
    { \
    mrgGetRobotCurrentPosition(fd, robotName, &posx, &posy, &posz); \
    printf("Current Pos:\t%f,\t%f,\t%f\n", posx, posy, posz); \
}while(0);

    //! 回零
    mrgRobotGoHome(fd, robotName, 0);
    PRINT_POS();

    //! 运动测试
    if(robotType == MRX_TYPE_H2)
    {
        mrgRobotRelMove(fd, robotName, -1, 0, 300, 0, 1, 0);
        PRINT_POS();

        mrgRobotRelMove(fd, robotName, -1, 300, 0, 0, 1, 0);
        PRINT_POS();

        mrgRobotRelMove(fd, robotName, -1, 0, -300, 0, 1, 0);
        PRINT_POS();

        mrgRobotRelMove(fd, robotName, -1, -100, 0, 0, 0.5, 0);
        PRINT_POS();
    }

    if(robotType == MRX_TYPE_T4)
    {
        //### 前后
        mrgRobotRelMove(fd, robotName, -1, 150, 0, 0, 2, 0);
        PRINT_POS();

        mrgRobotRelMove(fd, robotName, -1, -150, 0, 0, 2, 0);
        PRINT_POS();

        //### 右转
        mrgRobotRelMove(fd, robotName, -1, 0, -200, 0, 2, 0);
        PRINT_POS();

        mrgRobotRelMove(fd, robotName, -1, 0, 200, 0, 2, 0);
        PRINT_POS();

        //### 左转
        mrgRobotRelMove(fd, robotName, -1, 0, 200, 0, 2, 0);
        PRINT_POS();

        mrgRobotRelMove(fd, robotName, -1, 0, -200, 0, 2, 0);
        PRINT_POS();

        //### 上下
        mrgRobotRelMove(fd, robotName, -1, 0, 0, -300, 2, 0);
        PRINT_POS();

        mrgRobotRelMove(fd, robotName, -1, 0, 0, 300, 2, 0);
        PRINT_POS();
    }


    mrgRobotStop(fd, robotName, -1);


    //! 关闭网关
    ret =  mrgCloseGateWay(fd);
    printf("device close: %d %d\n", fd, ret);

    printf("Press AnyKey Continue...\n");
    getchar();
    return 0;
}
