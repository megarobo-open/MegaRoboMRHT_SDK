# -*- coding:utf-8 -*-

import sys
import os
pwd = os.path.abspath(os.path.dirname(__file__))
sys.path.append( pwd + "/../../lib/python")
from MegaGateway import *

import random
import binascii

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
    ########### 开始测试 ###########
    ### 产生随机长度
    sendDataLength = random.randrange(100, 300)
    print("sendDataLength:", sendDataLength)

    ### 产生随机内容
    sendData = bytearray()
    for i in range(sendDataLength):
        sendData.append(random.randint(0,128))

    ### 计算随机内容校验值
    sendDataCRC = binascii.crc32(sendData)
    print("sendDataCRC:", sendDataCRC)

    ### 设置读写文件名
    mrpFileName = "test.mrp"

    ### 删除之前的文件
    gw.mrgStorageMotionFileDelete(visa, mrpFileName)

    ### 写入到MRHT上
    gw.mrgStorageMotionFileSaveContext(visa, str(sendData) , mrpFileName)
    
    ### 导入运动文件
    gw.mrgRobotMotionFileImport(visa, robotID, mrpFileName)

    ### 查询运动文件是否存在
    mrpFileList = gw.mrgStorageMotionFileQuery(visa, 0)
    print("mrpFileList:", mrpFileList)

    if not mrpFileName in mrpFileList:
        print("mrp file not exist write failed")
        sys.exit(-1)

    ### 从-T上读取运动文件内容
    recvData = gw.mrgStorageMotionFileContextRead(visa, mrpFileName)

    ### 计算读取内容的校验值
    recvDataCRC = binascii.crc32( recvData.encode() )
    print("recvDataCRC:", recvDataCRC)


    ### 比较两个校验值
    print("sendData:")
    print(sendData)

    print("recvData:")
    print(recvData)

    if sendDataCRC == recvDataCRC:
        print("Test OK")
    else:
        print("Test Error")

    ########### 测试结束 ###########
###############################################################################
    ret =  gw.mrgCloseGateWay(visa)
    print("device close:", visa, ret)
################################################################################