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
    print("########### 开始测试 ###########")

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

    gw.mrgGateWaySendCmd(visa, "*IDN?")
    print("*IDN? ==> ", gw.mrgGateWayRead(visa) )

    print("*IDN? ==> ", gw.mrgGateWayQuery(visa, "*IDN?") )

    deviceCount = gw.mrgFindDevice(visa, 2000)
    if deviceCount == 0:
        print("Device Not Found!")
        sys.exit(-1)

    deviceList = gw.mrgGetDeviceName(visa)

    ### 选择驱控器
    if deviceCount == 1:
        device_index = '0'
    else:
        for index in range(deviceCount):
            device = deviceList[index]
            print( "%02d: Device:%s,\tTYPE:%s" % (index, device, gw.mrgGetDeviceType(visa, device)))

        device_index = input("please select Device:")
        if device_index == "" or int(device_index) < 0 or int(device_index) >= deviceCount:
            print("input error")
            sys.exit(0)
    
    deviceID = deviceList[int(device_index)]
    deviceType = gw.mrgGetDeviceType(visa, deviceID)

    deviceChannelCount = gw.mrgGetDeviceChannelCount(visa,deviceID)
    print("DeviceID: %s, DeviceType:%s" % (deviceID, deviceType))
    print("DeviceInfo:", gw.mrgGetDeviceInfo(visa,deviceID) )

    deviceSoftwareVersion = gw.mrgGetDeviceSoftVersion(visa, deviceID)
    print( "device software version:", deviceSoftwareVersion )

    firmWareHard = gw.mrgGetDeviceFirmWareHard(visa, deviceID)
    print( "device firmWareHard:", firmWareHard )

    firmWareBoot = gw.mrgGetDeviceFirmWareBoot(visa, deviceID)
    print( "device firmWareBoot:", firmWareBoot )

    firmWareFpga = gw.mrgGetDeviceFirmWareFpga(visa, deviceID)
    print( "device firmWareFpga:", firmWareFpga )

    deviceSerialNumber = gw.mrgGetDeviceSerialNumber(visa, deviceID)
    print( "device deviceSerialNumber:", deviceSerialNumber )
 
    ret =  gw.mrgCloseGateWay(visa)
    print("device close:", visa, ret)

    print("########### 测试结束 ###########")
################################################################################