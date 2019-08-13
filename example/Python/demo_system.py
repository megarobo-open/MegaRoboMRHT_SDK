# -*- coding:utf-8 -*-

import sys
import os
import time
pwd = os.path.abspath(os.path.dirname(__file__))
sys.path.append( pwd + "/../../lib/python")
from MegaGateway import *

if __name__ == "__main__":
###############################################################################
    gw = MageGateway()
###############################################################################
    ### 查找网关
    mrhtList = gw.mrgFindGateWay(0)
    mrhtCount = len(mrhtList)
    if mrhtCount == 0:
        print("MRHT Not Found!")
        sys.exit(-1)
    
    ### 选择网关
    if mrhtCount == 1:
        device_index = '0'
    else:
        for index in range(mrhtCount):
            mrht = mrhtList[index]
            visa =  gw.mrgOpenGateWay(mrht, 800)
            print( "%02d: MRHT:[%s],\tIDN:[%s]" % (index, mrht, gw.mrgGateWayIDNQuery(visa) ) )
            ret =  gw.mrgCloseGateWay(visa)

        device_index = input("please select MRHT:")
        if device_index == "" or int(device_index) < 0 or int(device_index) >= mrhtCount:
            print("input error")
            sys.exit(0)

    ### 打开网关
    mrht = mrhtList[int(device_index)]
    visa =  gw.mrgOpenGateWay(mrht, 800)
    IDN = gw.mrgGateWayIDNQuery(visa)
    print("MRHT Open[%s]: %s" % (visa, IDN))

    ### 查找机器人
    robotList = gw.mrgGetRobotName(visa)
    robotCount = len(robotList)
    if robotCount == 0:
        print("Robot Not Found!")
        sys.exit(-1)

    ### 选择机器人
    if robotCount == 1:
        robot_index = '0'
    else:
        for index in range(robotCount):
            robot = robotList[index]
            print( "%02d: Robot:%s,\tTYPE:%s" % (index, robot, gw.mrgGetRobotType(visa, robot)))

        robot_index = input("please select Robot:")
        if robot_index == "" or int(robot_index) < 0 or int(robot_index) >= robotCount:
            print("input error")
            sys.exit(0)
    
    robotID = robotList[int(robot_index)]
    robotType = gw.mrgGetRobotType(visa, robotID)
    print("RobotID: %s, RobotType:%s" % (robotID, robotType))

    deviceID = gw.mrgGetRobotDevice(visa, robotID)
    deviceType = gw.mrgGetDeviceType(visa, deviceID)
    deviceChannelCount = gw.mrgGetDeviceChannelCount(visa,deviceID)
    print("DeviceID: %s, DeviceType:%s" % (deviceID, deviceType))
    print("DeviceInfo:", gw.mrgGetDeviceInfo(visa,deviceID) )

###############################################################################
    print("########### 开始测试 ###########")
    ret = gw.mrgIdentify(visa, 1)
    print("mrgIdentify on", ret)
    time.sleep(2)
    ret = gw.mrgIdentify(visa, 0)
    print("mrgIdentify off", ret)

    print("mrgSysGetProjectSoftVersion", gw.mrgSysGetProjectSoftVersion(visa))
    print("mrgSysGetSoftVersion", gw.mrgSysGetSoftVersion(visa))
    print("mrgSysGetBackBoardSoftVersion", gw.mrgSysGetBackBoardSoftVersion(visa))
    print("mrgSysGetBackBoardHardVersion", gw.mrgSysGetBackBoardHardVersion(visa))
    print("mrgSysStartCan", gw.mrgSysStartCan(visa))
    print("mrgSysResetCan", gw.mrgSysResetCan(visa))

    gw.mrgSysConfCan(visa, 2000000)
    print("mrgSysConfCan 2000000 ==>", gw.mrgSysConfCan_Query(visa))

    gw.mrgSysConfCan(visa, 1000000)
    print("mrgSysConfCan 1000000 ==>", gw.mrgSysConfCan_Query(visa))

    sysMode = gw.mrgGetSysMode(visa)
    ret = gw.mrgSysModeSwitch(visa, 1-sysMode)
    print("mrgSysModeSwitch ", 1-sysMode , ret)
    time.sleep(2)
    ret = gw.mrgSysModeSwitch(visa, sysMode)
    print("mrgSysModeSwitch ", sysMode, ret)



    # gw.mrgSysInstructionMode
    # gw.mrgSysInstructionMode_Query
    # gw.mrgSysSetNetworkIPMode
    # gw.mrgSysGetNetworkIPMode
    # gw.mrgSysWifiScan
    # gw.mrgSysWifiConfig
    # gw.mrgSysWifiUpDown
    # gw.mrgSysWifiCheckState
    # gw.mrgSysGetWifiConnected

    # gw.mrgSysSetHeartPeriod
    # gw.mrgSysGetHeartPeriod
    # gw.mrgSysSetPowerOn
    # gw.mrgSysGetPowerOn
    # gw.mrgSysSetInterface
    # gw.mrgSysGetInterface
    # gw.mrgSysSetEmergencyStop
    # gw.mrgSysGetEmergencyStopState
    # gw.mrgSysUpdateFileSearch
    # gw.mrgSysUpdateFileStart
    # gw.mrgSysUpdateFirmwareStatusQuery
    # gw.mrgScriptSearch
    # gw.mrgScriptConfig
    # gw.mrgScriptConfigQuery
    # gw.mrgScriptRun
    # gw.mrgScriptStop
    # gw.mrgScriptGetCurrentStates
    # gw.mrgSystemRunCmd
    # gw.mrgSystemSetMRQConfig
    # gw.mrgSystemGetMRQConfig

    print("########### 测试结束 ###########")
###############################################################################
    ret =  gw.mrgCloseGateWay(visa)
    print("device close:", visa, ret)
################################################################################