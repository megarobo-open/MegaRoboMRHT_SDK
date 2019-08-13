# -*- coding:utf-8 -*-

import sys
import os
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







    print("########### 测试结束 ###########")
###############################################################################
    ret =  gw.mrgCloseGateWay(visa)
    print("device close:", visa, ret)
################################################################################