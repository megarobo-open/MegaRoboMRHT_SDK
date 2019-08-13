# -*- coding:utf-8 -*-

import time
import threading
import socket
import os
import platform
from ctypes import *

### 将C语言的char*字符串转化为标准的python字符串
def cbuf_to_string(cbuf):
    return bytes.decode(cbuf.value)

### python的字符串转化为C语言的char *格式
def string_to_charp(s):
    return c_char_p(s.encode())

### 删除list指定值的所有元素
def list_remove_all(lst, ele):
    for i in range(len(lst)-1,-1,-1):
        if lst[i] == ele:
            lst.remove(ele)
    return lst


class MageGateway(object):
    
    def __init__(self):
        pwd = os.path.abspath(os.path.dirname(__file__))
        _platform = platform.system()
        if _platform == "Windows" :
            os.chdir(pwd + "/../windows/")
            self._dll = cdll.LoadLibrary('MegaGateway.dll')
        else:
            info = os.popen('file /bin/ls').readlines()[0]
            cpuarch = info.split(",")[1]
            if 'x86' in cpuarch:
                self._dll = cdll.LoadLibrary(pwd + "/../linux/libMegaGateway.so")
            else:
                self._dll = cdll.LoadLibrary(pwd + "/../armlinux/libMegaGateway.so")

############################################################
### device.h
############################################################
    '''
    查找设备, devtype: 0表示LAN 1表示USB
    '''
    def mrgFindGateWay(self, devtype = 0):
        MAX_LENGTH=1024
        buf = create_string_buffer(MAX_LENGTH)
        self._dll.mrgFindGateWay(devtype, buf, MAX_LENGTH, 1)
        if cbuf_to_string(buf) == '':
            return list()
        else:
            return list(filter(None, cbuf_to_string(buf).split(',') ))


    def mrgOpenGateWay(self, desc, timeout_ms):
        '''
        打开设备 desc: 设备描述， timeout_ms: 超时时间
        '''
        return self._dll.mrgOpenGateWay(string_to_charp(desc), timeout_ms)


    def mrgCloseGateWay(self, fd):
        return self._dll.mrgCloseGateWay(fd)


    def mrgGateWaySendCmd(self, fd, cmd):
        return self._dll.mrgGateWaySendCmd(fd, string_to_charp(cmd), len(cmd))

    
    def mrgGateWayRead(self, fd):
        buf = create_string_buffer(1024)
        ret = self._dll.mrgGateWayRead(fd, buf, 1024)
        if ret < 0:
            return ""
        else:
            return cbuf_to_string(buf)


    def mrgGateWayQuery(self, fd, cmd):
        buf = create_string_buffer(1024)
        ret = self._dll.mrgGateWayQuery(fd, string_to_charp(cmd), buf, 1024)
        if ret < 0:
            return ""
        else:
            return cbuf_to_string(buf)


    def mrgGateWayIDNQuery(self, fd):
        buf = create_string_buffer(1024)
        ret = self._dll.mrgGateWayIDNQuery(fd, buf)
        if ret < 0:
            return ""
        else:
            return cbuf_to_string(buf)


    def mrgFindDevice(self, fd, timeout_ms):
        return self._dll.mrgFindDevice(fd, timeout_ms)


    def mrgGetDeviceName(self, fd):
        names = (c_int * 100)()
        self._dll.mrgGetDeviceName(fd, names )
        nameList = list(names)
        return list_remove_all(nameList, 0)


    def mrgGetDeviceType(self, fd, name):
        buf = create_string_buffer(1024)
        ret = self._dll.mrgGetDeviceType(fd, name, buf)
        if ret < 0:
            return ""
        else:
            return cbuf_to_string(buf)


    def mrgGetDeviceChannelCount(self, fd, name):
        return self._dll.mrgGetDeviceChannelCount(fd, name)


    def mrgGetDeviceInfo(self, fd, name):
        buf = create_string_buffer(1024)
        ret = self._dll.mrgGetDeviceInfo(fd, name, buf)
        if ret < 0:
            return ""
        else:
            return cbuf_to_string(buf)


    def mrgGetDeviceSoftVersion(self, fd, name):
        buf = create_string_buffer(1024)
        ret = self._dll.mrgGetDeviceSoftVersion(fd, name, buf)
        if ret < 0:
            return ""
        else:
            return cbuf_to_string(buf)


    def mrgGetDeviceFirmWareHard(self, fd, name):
        buf = create_string_buffer(1024)
        ret = self._dll.mrgGetDeviceFirmWareHard(fd, name, buf)
        if ret < 0:
            return ""
        else:
            return cbuf_to_string(buf)


    def mrgGetDeviceFirmWareBoot(self, fd, name):
        buf = create_string_buffer(1024)
        ret = self._dll.mrgGetDeviceFirmWareBoot(fd, name, buf)
        if ret < 0:
            return ""
        else:
            return cbuf_to_string(buf)


    def mrgGetDeviceFirmWareFpga(self, fd, name):
        buf = create_string_buffer(1024)
        ret = self._dll.mrgGetDeviceFirmWareFpga(fd, name, buf)
        if ret < 0:
            return ""
        else:
            return cbuf_to_string(buf)


    def mrgWriteDeviceSerial(self, fd, name, sn):
        ret = self._dll.mrgWriteDeviceSerial(fd, name, string_to_charp(sn))
        ###保证完全写到背板上
        time.sleep(1)
        return ret


    def mrgGetDeviceSerialNumber(self, fd, name):
        buf = create_string_buffer(1024)
        ret = self._dll.mrgGetDeviceSerialNumber(fd, name, buf)
        if ret < 0:
            return ""
        else:
            return cbuf_to_string(buf)


    def mrgIdentify(self, fd, state):
        return self._dll.mrgIdentify(fd, state)


    def mrgModeSwitch(self, fd, mode):
        return self._dll.mrgModeSwitch(fd, mode)


    ##返回IO状态,失败返回空list
    def mrgGetXinState(self, fd):
        states = (c_int * 4)()
        ret = self._dll.mrgGetXinState(fd, states)
        if ret == -1:
            return list()
        else:
            return list(states)


    def mrgSetYoutState(self, fd, yout, state):
        return self._dll.mrgSetYoutState(fd, yout, state)
############################################################


############################################################
### megarobot.h  机器人操作 
############################################################
### int mrgBuildRobot(ViSession vi, char * robotType, char * chlist, int *robotname);
    def mrgBuildRobot(self, fd, robotype, chlist):
        robotname = c_int(-1)
        ret = self._dll.mrgBuildRobot(fd, string_to_charp(robotype), string_to_charp(chlist), byref(robotname) )
        if ret < 0:
            return -1
        else:
            return robotname.value

### int mrgBuildRobotNamed(ViSession vi, char * robotType, char * devList, int robotid, int *robotname);
    def mrgBuildRobotNamed(self, fd, robotype, devList, robotid):
        robotname = c_int(-1)
        ret = self._dll.mrgBuildRobotNamed(fd, string_to_charp(robotype), string_to_charp(devList), robotid, byref(robotname) )
        if ret < 0:
            return -1
        else:
            return robotname.value

### int mrgDeleteRobot(ViSession vi, int name);
    def mrgDeleteRobot(self, fd, robotname):
        return self._dll.mrgDeleteRobot(fd, robotname)

### int mrgSetRobotLinks(ViSession vi, int name, float * links, int link_count);
    def mrgSetRobotLinks(self, fd, linkList):
        links = (c_float * 100)()
        for i in range(len(linkList)):
            links[i] = linkList[i]

        ret = self._dll.mrgSetRobotLinks(fd, links, len(linkList))
        return ret

### int mrgGetRobotLinks(ViSession vi, int name, float * links, int *link_count);
    def mrgGetRobotLinks(self, fd):
        links = (c_float * 100)()
        count = c_int(-1)
        self._dll.mrgGetRobotLinks(fd, links, byref(count))
        linkList = list(links)
        return linkList[0:count.value]


### int mrgGetRobotType(ViSession vi, int name);
    def mrgGetRobotType(self, fd, robotname):
        typeList = ["MRX-T4", "MRX-AS", "MRX-H2", "MRX-DELTA", "MRX-RAW", "UNKNOWN"]
        ret = self._dll.mrgGetRobotType(fd, robotname)
        if ret < 0:
            return "UNKNOWN"
        elif ret < len(typeList):
            return typeList[ret]
        else:
            return "UNKNOWN"

### int mrgExportRobotConfig(ViSession vi);
    def mrgExportRobotConfig(self, fd):
        return self._dll.mrgExportRobotConfig(fd)

### int mrgExportRobotConfig_default(ViSession vi);
    def mrgExportRobotConfig_default(self, fd):
        return self._dll.mrgExportRobotConfig_default(fd)

### int mrgRestoreRobotConfig(ViSession vi);
    def mrgRestoreRobotConfig(self, fd):
        return self._dll.mrgRestoreRobotConfig(fd)

### int mrgGetRobotConfigState(ViSession vi);
    def mrgGetRobotConfigState(self, fd):
        return self._dll.mrgGetRobotConfigState(fd)

### int mrgSetRobotMachineSerialNum(ViSession vi, int name, char * sn);
    def mrgSetRobotMachineSerialNum(self, fd, roboname, sn):
        return self._dll.mrgSetRobotMachineSerialNum(fd, roboname, string_to_charp(sn))

### int mrgGetRobotMachineSerialNum(ViSession vi, int name, char*serial);
    def mrgGetRobotMachineSerialNum(self, fd, name):
        buf = create_string_buffer(1024)
        ret = self._dll.mrgGetRobotMachineSerialNum(fd, name, buf)
        if ret < 0:
            return ""
        else:
            return cbuf_to_string(buf)

### int mrgSetRobotSubType(ViSession vi, int name, int subtype);
    def mrgSetRobotSubType(self, fd, roboname, subtype):
        return self._dll.mrgSetRobotSubType(fd, roboname, subtype)

### int mrgGetRobotSubType(ViSession vi, int name);
    def mrgGetRobotSubType(self, fd, robotname):
        return self._dll.mrgGetRobotSubType(fd, robotname)

### int mrgSetRobotCoordinateSystem(ViSession vi, int name, int coord);
    def mrgSetRobotCoordinateSystem(self, fd, roboname, coord):
        return self._dll.mrgSetRobotCoordinateSystem(fd, roboname, coord)

### int mrgGetRobotCoordinateSystem(ViSession vi, int name);
    def mrgGetRobotCoordinateSystem(self, fd, robotname):
        return self._dll.mrgGetRobotCoordinateSystem(fd, robotname)

### int mrgGetRobotCount(ViSession vi);
    def mrgGetRobotCount(self, fd):
        return self._dll.mrgGetRobotCount(fd)

### int mrgGetRobotName(ViSession vi, int *robotnames);
    def mrgGetRobotName(self, fd):
        names = (c_int * 100)()
        self._dll.mrgGetRobotName(fd, names )
        nameList = list(names)
        return list_remove_all(nameList, 0)

### int mrgGetRobotDevice(ViSession vi, int robotname, int * device);
    def mrgGetRobotDevice(self, fd, roboname):
        result = c_int(-1)
        ret = self._dll.mrgGetRobotDevice(fd,  roboname, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

### int mrgSetRobotProjectZero(ViSession vi, int name, float x, float y, float z);
    def mrgSetRobotProjectZero(self, fd, roboname, x, y, z):
        return self._dll.mrgSetRobotProjectZero(fd, roboname, c_float(x), c_float(y), c_float(z))

### int mrgGetRobotProjectZero(ViSession vi, int name, float * x, float *y, float *z);
    def mrgGetRobotProjectZero(self, fd, roboname):
        x = c_float(-1)
        y = c_float(-1)
        z = c_float(-1)
        ret = self._dll.mrgGetRobotProjectZero(fd,  roboname, byref(x), byref(y), byref(z) )
        if ret < 0:
            return list()
        else:
            return [x.value, y.value, z.value]

### int mrgSetRobotAxisZero(ViSession vi, int name, float x, float y, float z);
    def mrgSetRobotAxisZero(self, fd, roboname, x, y, z):
        return self._dll.mrgSetRobotAxisZero(fd, roboname, c_float(x), c_float(y), c_float(z))

### int mrgGetRobotAxisZero(ViSession vi, int name, float * x, float *y, float *z);
    def mrgGetRobotAxisZero(self, fd, roboname):
        x = c_float(-1)
        y = c_float(-1)
        z = c_float(-1)
        ret = self._dll.mrgGetRobotAxisZero(fd,  roboname, byref(x), byref(y), byref(z) )
        if ret < 0:
            return list()
        else:
            return [x.value, y.value, z.value]

### int mrgSetRobotSoftWareLimit(ViSession vi, int name, int type, float x, float y, float z);
    def mrgSetRobotSoftWareLimit(self, fd, roboname, tp, x, y, z):
        return self._dll.mrgSetRobotSoftWareLimit(fd, roboname, tp, c_float(x), c_float(y), c_float(z))

### int mrgGetRobotSoftWareLimit(ViSession vi, int name, int type, float * x, float *y, float *z);
    def mrgGetRobotSoftWareLimit(self, fd, roboname, tp):
        x = c_float(-1)
        y = c_float(-1)
        z = c_float(-1)
        ret = self._dll.mrgGetRobotSoftWareLimit(fd,  roboname, tp, byref(x), byref(y), byref(z) )
        if ret < 0:
            return list()
        else:
            return [x.value, y.value, z.value]

### int mrgSetRobotWavetable(ViSession vi, int name, int wavetable);
    def mrgSetRobotWavetable(self, fd, roboname, wavetable):
        return self._dll.mrgSetRobotWavetable(fd, roboname, wavetable)

### int mrgRobotWavetableQuery(ViSession vi, int name);
    def mrgRobotWavetableQuery(self, fd, robotname):
        return self._dll.mrgRobotWavetableQuery(fd, robotname)

### int mrgRobotRun(ViSession vi, int name, int wavetable);
    def mrgRobotRun(self, fd, roboname, wavetable):
        return self._dll.mrgRobotRun(fd, roboname, wavetable)

### int mrgRobotStop(ViSession vi, int name, int wavetable);
    def mrgRobotStop(self, fd, roboname, wavetable):
        return self._dll.mrgRobotStop(fd, roboname, wavetable)

### int CALL mrgGetRobotStates(ViSession vi, int name, int wavetable, char *state);
    def mrgGetRobotStates(self, fd, roboname, wavetable):
        buf = create_string_buffer(1024)
        ret = self._dll.mrgGetRobotStates(fd, roboname, wavetable, buf)
        if ret < 0:
            return ""
        else:
            return cbuf_to_string(buf)

### int mrgRobotWaitReady(ViSession vi, int name, int wavetable, int timeout_ms);
    def mrgRobotWaitReady(self, fd, roboname, wavetable, timeout_ms):
        return self._dll.mrgRobotWaitReady(fd, roboname, wavetable, timeout_ms)

### int mrgRobotWaitEnd(ViSession vi, int name, int wavetable, int timeout_ms);
    def mrgRobotWaitEnd(self, fd, roboname, wavetable, timeout_ms):
        return self._dll.mrgRobotWaitEnd(fd, roboname, wavetable, timeout_ms)

### int mrgRobotMove(ViSession vi, int name, int wavetable, float x, float y, float z, float time, int timeout_ms);
    def mrgRobotMove(self, fd, roboname, wavetable, x, y, z, time, timeout_ms):
        return self._dll.mrgRobotMove(fd, roboname, wavetable, c_float(x), c_float(y), c_float(z), c_float(time), timeout_ms)

### int mrgRobotMoveOn(ViSession vi, int name, int wavetable, float x, float y, float z, float speed);
    def mrgRobotMoveOn(self, fd, roboname, wavetable, x, y, z, speed):
        return self._dll.mrgRobotMoveOn(fd, roboname, wavetable, c_float(x), c_float(y), c_float(z), c_float(speed))

### int mrgRobotAxisMoveOn(ViSession vi, int name, int wavetable, int ax, float speed);
    def mrgRobotAxisMoveOn(self, fd, roboname, wavetable, ax, speed):
        return self._dll.mrgRobotAxisMoveOn(fd, roboname, wavetable, ax, c_float(speed))

### int mrgRobotAxisMoveJog(ViSession vi, int name, int wavetable, int ax, float cr_time, float cr_speed, float speed);
    def mrgRobotAxisMoveJog(self, fd, roboname, wavetable, ax, cr_time, cr_speed, speed):
        return self._dll.mrgRobotAxisMoveJog(fd, roboname, wavetable, ax, c_float(cr_time), c_float(cr_speed), c_float(speed))

### int mrgRobotRelMove(ViSession vi, int name, int wavetable, float x, float y, float z, float time, int timeout_ms);
    def mrgRobotRelMove(self, fd, roboname, wavetable, x, y, z, time, timeout_ms):
        return self._dll.mrgRobotRelMove(fd, roboname, wavetable, c_float(x), c_float(y), c_float(z), c_float(time), timeout_ms)

### int mrgRobotMoveL(ViSession vi, int name, int wavetable, float x, float y, float z, float time, int timeout_ms);
    def mrgRobotMoveL(self, fd, roboname, wavetable, x, y, z, time, timeout_ms):
        return self._dll.mrgRobotMoveL(fd, roboname, wavetable, c_float(x), c_float(y), c_float(z), c_float(time), timeout_ms)

### int mrgRobotRelMoveL(ViSession vi, int name, int wavetable, float x, float y, float z, float time, int timeout_ms);
    def mrgRobotRelMoveL(self, fd, roboname, wavetable, x, y, z, time, timeout_ms):
        return self._dll.mrgRobotRelMoveL(fd, roboname, wavetable, c_float(x), c_float(y), c_float(z), c_float(time), timeout_ms)

### int mrgSetRobotInterPolateMode(ViSession vi, int name, int mode);
    def mrgSetRobotInterPolateMode(self, fd, roboname, mode):
        return self._dll.mrgSetRobotInterPolateMode(fd, roboname, mode)

### int mrgGetRobotInterPolateMode(ViSession vi, int name, int* mode);
    def mrgGetRobotInterPolateMode(self, fd, roboname):
        result = c_int(-1)
        ret = self._dll.mrgGetRobotInterPolateMode(fd,  roboname, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

### int mrgSetRobotInterPolateStep(ViSession vi, int name, float step);
    def mrgSetRobotInterPolateStep(self, fd, roboname, step):
        return self._dll.mrgSetRobotInterPolateStep(fd, roboname, c_float(step))

### int mrgGetRobotInterPolateStep(ViSession vi, int name, float* step);
    def mrgGetRobotInterPolateStep(self, fd, roboname):
        result = c_float(-1)
        ret = self._dll.mrgGetRobotInterPolateStep(fd,  roboname, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

### int mrgSetRobotHomeWavetable(ViSession vi, int name, int wavetable);
    def mrgSetRobotHomeWavetable(self, fd, roboname, wavetable):
        return self._dll.mrgSetRobotHomeWavetable(fd, roboname, wavetable)

### int mrgGetRobotHomeWavetable(ViSession vi, int name);
    def mrgGetRobotHomeWavetable(self, fd, robotname):
        return self._dll.mrgGetRobotHomeWavetable(fd, robotname)

### int mrgRobotGoHome(ViSession vi, int name, int timeout_ms);
    def mrgRobotGoHome(self, fd, roboname, timeout_ms):
        return self._dll.mrgRobotGoHome(fd, roboname, timeout_ms)

### int mrgRobotGoHomeWithParam(ViSession vi, int name, float param, int timeout_ms);
    def mrgRobotGoHomeWithParam(self, fd, roboname, param, timeout_ms):
        return self._dll.mrgRobotGoHomeWithParam(fd, roboname, c_float(param), timeout_ms)

### int mrgRobotGoHomeStop(ViSession vi, int name);
    def mrgRobotGoHomeStop(self, fd, robotname):
        return self._dll.mrgRobotGoHomeStop(fd, robotname)

### int mrgRobotWaitHomeEnd(ViSession vi, int name, int timeout_ms);
    def mrgRobotWaitHomeEnd(self, fd, roboname, timeout_ms):
        return self._dll.mrgRobotWaitHomeEnd(fd, roboname, timeout_ms)

### int mrgGetRobotHomeAngle(ViSession vi, int name, float * angles);
    def mrgGetRobotHomeAngle(self, fd, roboname):
        result = c_float(-1)
        ret = self._dll.mrgGetRobotHomeAngle(fd,  roboname, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

### int mrgGetRobotHomePosition(ViSession vi, int name, float * x, float *y, float* z);
    def mrgGetRobotHomePosition(self, fd, roboname):
        x = c_float(-1)
        y = c_float(-1)
        z = c_float(-1)
        ret = self._dll.mrgGetRobotHomePosition(fd,  roboname, byref(x), byref(y), byref(z) )
        if ret < 0:
            return list()
        else:
            return [x.value, y.value, z.value]

### int mrgSetRobotHomeMode(ViSession vi, int name, int mode);
    def mrgSetRobotHomeMode(self, fd, roboname, mode):
        return self._dll.mrgSetRobotHomeMode(fd, roboname, mode)

### int mrgGetRobotHomeMode(ViSession vi, int name);
    def mrgGetRobotHomeMode(self, fd, robotname):
        return self._dll.mrgGetRobotHomeMode(fd, robotname)

### int mrgGetRobotHomeRequire(ViSession vi, int name);
    def mrgGetRobotHomeRequire(self, fd, robotname):
        return self._dll.mrgGetRobotHomeRequire(fd, robotname)

### int mrgRobotPointClear(ViSession vi, int name);
    def mrgRobotPointClear(self, fd, robotname):
        return self._dll.mrgRobotPointClear(fd, robotname)

### int mrgRobotPointLoad(ViSession vi, int name, float x, float y, float z, float end, float time,int mod,float step);
    def mrgRobotPointLoad(self, fd, roboname, x, y, z, end, time, mod, step):
        return self._dll.mrgRobotPointLoad(fd, roboname, c_float(x), c_float(y), c_float(z), c_float(end), c_float(time), mod, c_float(step))

### int mrgRobotPointResolve(ViSession vi, int name, int wavetable, int timeout_ms);
    def mrgRobotPointResolve(self, fd, roboname, wavetable, timeout_ms):
        return self._dll.mrgRobotPointResolve(fd, roboname, wavetable, timeout_ms)

### int mrgRobotPvtClear(ViSession vi, int name);
    def mrgRobotPvtClear(self, fd, robotname):
        return self._dll.mrgRobotPvtClear(fd, robotname)

### int mrgRobotPvtLoad(ViSession vi, int name, float p, float v, float t, int axle);
    def mrgRobotPvtLoad(self, fd, roboname, p, v, t, axle):
        return self._dll.mrgRobotPvtLoad(fd, roboname, c_float(p), c_float(v), c_float(t), axle)

### int mrgRobotPvtResolve(ViSession vi, int name, int wavetable, int timeout_ms);
    def mrgRobotPvtResolve(self, fd, roboname, wavetable, timeout_ms):
        return self._dll.mrgRobotPvtResolve(fd, roboname, wavetable, timeout_ms)

### int mrgRobotMotionFileImport(ViSession vi, int name,char* filename);
    def mrgRobotMotionFileImport(self, fd, roboname, filename):
        return self._dll.mrgRobotMotionFileImport(fd, roboname, string_to_charp(filename))

### int mrgRobotMotionFileImportLocal(ViSession vi, int name, char* filename);
    def mrgRobotMotionFileImportLocal(self, fd, roboname, filename):
        return self._dll.mrgRobotMotionFileImportLocal(fd, roboname, string_to_charp(filename))

### int mrgRobotMotionFileImportExternal(ViSession vi, int name, char* filename);
    def mrgRobotMotionFileImportExternal(self, fd, roboname, filename):
        return self._dll.mrgRobotMotionFileImportExternal(fd, roboname, string_to_charp(filename))

### int mrgRobotFileResolve(ViSession vi, int name, int section, int line, int wavetable, int timeout_ms);
    def mrgRobotFileResolve(self, fd, roboname, section, line, wavetable, timeout_ms):
        return self._dll.mrgRobotFileResolve(fd, roboname, section, line, wavetable, timeout_ms)

### int mrgRobotMotionFileExport(ViSession vi, int name, int location, char* filename);
    def mrgRobotMotionFileExport(self, fd, roboname, location, filename):
        return self._dll.mrgRobotMotionFileExport(fd, roboname, location, string_to_charp(filename))

### int mrgRobotToolSet(ViSession vi, int robotname, int type, char* dev);
    def mrgRobotToolSet(self, fd, roboname, tp, dev):
        return self._dll.mrgRobotToolSet(fd, roboname, tp, string_to_charp(dev))

### int mrgRobotWaitToolExeEnd(ViSession vi, int name, int timeout_ms);
    def mrgRobotWaitToolExeEnd(self, fd, roboname, timeout_ms):
        return self._dll.mrgRobotWaitToolExeEnd(fd, roboname, timeout_ms)

### int mrgRobotToolExe(ViSession vi, int name, float position, float time, int timeout_ms);
    def mrgRobotToolExe(self, fd, roboname, position, time, timeout_ms):
        return self._dll.mrgRobotToolExe(fd, roboname, c_float(position), c_float(time), timeout_ms)

### int mrgRobotToolStop(ViSession vi, int name);
    def mrgRobotToolStop(self, fd, robotname):
        return self._dll.mrgRobotToolStop(fd, robotname)

### int mrgRobotToolStopGoHome(ViSession vi, int name);
    def mrgRobotToolStopGoHome(self, fd, robotname):
        return self._dll.mrgRobotToolStopGoHome(fd, robotname)

### int mrgRobotToolGoHome(ViSession vi, int name, int timeout_ms);
    def mrgRobotToolGoHome(self, fd, roboname, timeout_ms):
        return self._dll.mrgRobotToolGoHome(fd, roboname, timeout_ms)

### int mrgGetRobotToolPosition(ViSession vi, int name, float * position);
    def mrgGetRobotToolPosition(self, fd, roboname):
        result = c_float(-1)
        ret = self._dll.mrgGetRobotToolPosition(fd,  roboname, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value


### int  mrgRobotGetToolType(ViSession vi, int robotname, int * type);
    def mrgRobotGetToolType(self, fd, roboname):
        result = c_int(-1)
        ret = self._dll.mrgRobotGetToolType(fd, roboname, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

### int  mrgRobotToolExeMode(ViSession vi, int name, int mode);
    def mrgRobotToolExeMode(self, fd, roboname, mode):
        return self._dll.mrgRobotToolExeMode(fd, roboname, mode)

    
### int  mrgRobotToolExeMode_Query(ViSession vi, int name, int* mode);        
    def mrgRobotToolExeMode_Query(self, fd, roboname):
        result = c_int(-1)
        ret = self._dll.mrgRobotToolExeMode_Query(fd, roboname, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

### int mrgGetRobotCurrentAngle(ViSession vi, int name, float * angles);
    def mrgGetRobotCurrentAngle(self, fd, roboname):
        result = c_float(-1)
        ret = self._dll.mrgGetRobotCurrentAngle(fd,  roboname, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

### int mrgGetRobotCurrentPosition(ViSession vi, int name, float * x, float *y, float* z);
    def mrgGetRobotCurrentPosition(self, fd, roboname):
        x = c_float(-1)
        y = c_float(-1)
        z = c_float(-1)
        ret = self._dll.mrgGetRobotCurrentPosition(fd,  roboname, byref(x), byref(y), byref(z) )
        if ret < 0:
            return list()
        else:
            return [x.value, y.value, z.value]

### int mrgRobotJointHome(ViSession vi, int name, int axi, float speed, int timeout_ms);
    def mrgRobotJointHome(self, fd, roboname, axi, speed, timeout_ms):
        return self._dll.mrgRobotJointHome(fd, roboname, axi, c_float(speed), timeout_ms)

### int mrgRobotJointMove(ViSession vi, int name, int axi, float position, float time, int timeout_ms);
    def mrgRobotJointMove(self, fd, roboname, axi, position, time, timeout_ms):
        return self._dll.mrgRobotJointMove(fd, roboname, axi, c_float(position), c_float(time), timeout_ms)

### int  mrgRobotJointMoveOn(ViSession vi, int name, int axi, float speed);
    def mrgRobotJointMoveOn(self, fd, roboname, axi, speed):
        return self._dll.mrgRobotJointMoveOn(fd, roboname, axi, c_float(speed))


### int mrgGetRobotJointAngle(ViSession vi, int name, int joint, float *angle);
    def mrgGetRobotJointAngle(self, fd, roboname, joint):
        result = c_float(-1)
        ret = self._dll.mrgGetRobotJointAngle(fd,  roboname, joint, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

### int mrgGetRobotCurrentMileage(ViSession vi, int name, float * x, float *y, float* z);
    def mrgGetRobotCurrentMileage(self, fd, roboname):
        x = c_float(-1)
        y = c_float(-1)
        z = c_float(-1)
        ret = self._dll.mrgGetRobotCurrentMileage(fd,  roboname, byref(x), byref(y), byref(z) )
        if ret < 0:
            return list()
        else:
            return [x.value, y.value, z.value]

### int mrgGetRobotTargetPosition(ViSession vi, int name, float * x, float *y, float* z);
    def mrgGetRobotTargetPosition(self, fd, roboname):
        x = c_float(-1)
        y = c_float(-1)
        z = c_float(-1)
        ret = self._dll.mrgGetRobotTargetPosition(fd,  roboname, byref(x), byref(y), byref(z) )
        if ret < 0:
            return list()
        else:
            return [x.value, y.value, z.value]

### int mrgGetRobotCurrentRecord(ViSession vi, int name, int *record);
    def mrgGetRobotCurrentRecord(self, fd, roboname):
        result = c_int(-1)
        ret = self._dll.mrgGetRobotCurrentRecord(fd,  roboname, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

### int mrgGetRobotWristPose(ViSession vi, int name, float *angle);
    def mrgGetRobotWristPose(self, fd, roboname):
        result = c_float(-1)
        ret = self._dll.mrgGetRobotWristPose(fd, roboname, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

### int mrgSetRobotWristPose(ViSession vi, int name, int wavetable, float angle, float speed, int timeout_ms);
    def mrgSetRobotWristPose(self, fd, roboname, wavetable, angle, speed, timeout_ms):
        return self._dll.mrgSetRobotWristPose(fd, roboname, wavetable, c_float(angle), c_float(speed), timeout_ms)

### int mrgSetRobotFold(ViSession vi, int name, float axi0, float axi1, float axi2, float axi3, int timeout);
    def mrgSetRobotFold(self, fd, roboname, axi0, axi1, axi2, axi3, timeout):
        return self._dll.mrgSetRobotFold(fd, roboname, c_float(axi0), c_float(axi1), c_float(axi2), c_float(axi3), timeout)

### int mrgGetRobotFoldState(ViSession vi, int name);
    def mrgGetRobotFoldState(self, fd, roboname):
        return self._dll.mrgGetRobotFoldState(fd, roboname)

############################################################
### "mrqdevice.h"
############################################################
# int mrgMRQIdentify(ViSession vi, int name, int state);
    def mrgMRQIdentify(self, fd, name, state):
        return self._dll.mrgMRQIdentify(fd, name, state)

# int mrgGetMRQDioState(ViSession vi, int name, unsigned short * state);
    def mrgGetMRQDioState(self, fd, dev):
        result = c_int(-1)
        ret = self._dll.mrgGetMRQDioState(fd, dev, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgGetMRQGroup(ViSession vi, char * devList, unsigned int * groupID, int grouptype);
    def mrgGetMRQGroup(self, fd, devListStr, grouptype):
        groupID = c_uint(-1)
        ret = self._dll.mrgGetMRQGroup(fd, devListStr, byref(groupID), grouptype)
        if ret < 0:
            return -1
        else:
            return groupID.value


# int mrgMRQMotionStateReport(ViSession vi, int name, int ch, int state);
    def mrgMRQMotionStateReport(self, fd, int_name, int_ch, int_state):
        return self._dll.mrgMRQMotionStateReport(fd, int_name, int_ch, int_state)

# int mrgMRQMotionStateReport_Query(ViSession vi, int name, int ch,int *state);
    def mrgMRQMotionStateReport_Query(self, fd, dev, ch):
        result = c_int(-1)
        ret = self._dll.mrgMRQMotionStateReport_Query(fd, dev, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQMotionRun(ViSession vi, int name, int ch, int wavetable);
    def mrgMRQMotionRun(self, fd, int_name, int_ch, int_wavetable):
        return self._dll.mrgMRQMotionRun(fd, int_name, int_ch, int_wavetable)

# int mrgMRQMotionRunState_Query(ViSession vi, int name, int ch, int wavetable, int*robotstate);
    def mrgMRQMotionRunState_Query(self, fd, dev, ch, wavetable):
        result = c_int(-1)
        ret = self._dll.mrgMRQMotionRunState_Query(fd, dev, ch, wavetable, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQMotionWaitReady(ViSession vi, int name, int ch, int wavetable, int timeout_ms);
    def mrgMRQMotionWaitReady(self, fd, int_name, int_ch, int_wavetable, int_timeout_ms):
        return self._dll.mrgMRQMotionWaitReady(fd, int_name, int_ch, int_wavetable, int_timeout_ms)

# int mrgMRQMotionWaitEnd(ViSession vi, int name, int ch, int wavetable, int timeout_ms);
    def mrgMRQMotionWaitEnd(self, fd, int_name, int_ch, int_wavetable, int_timeout_ms):
        return self._dll.mrgMRQMotionWaitEnd(fd, int_name, int_ch, int_wavetable, int_timeout_ms)

# int mrgMRQMotionStop(ViSession vi, int name, int ch, int wavetable);
    def mrgMRQMotionStop(self, fd, int_name, int_ch, int_wavetable):
        return self._dll.mrgMRQMotionStop(fd, int_name, int_ch, int_wavetable)

# int mrgMRQMotionTrigSource(ViSession vi, int name, int ch, int source);
    def mrgMRQMotionTrigSource(self, fd, int_name, int_ch, int_source):
        return self._dll.mrgMRQMotionTrigSource(fd, int_name, int_ch, int_source)

# int mrgMRQMotionTrigSource_Query(ViSession vi, int name, int ch, int * source);
    def mrgMRQMotionTrigSource_Query(self, fd, dev, ch):
        result = c_int(-1)
        ret = self._dll.mrgMRQMotionTrigSource_Query(fd, dev, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQMotionOffsetState(ViSession vi, int name, int ch, int state);
    def mrgMRQMotionOffsetState(self, fd, int_name, int_ch, int_state):
        return self._dll.mrgMRQMotionOffsetState(fd, int_name, int_ch, int_state)

# int mrgMRQMotionOffsetState_Query(ViSession vi, int name, int ch, int *state);
    def mrgMRQMotionOffsetState_Query(self, fd, dev, ch):
        result = c_int(-1)
        ret = self._dll.mrgMRQMotionOffsetState_Query(fd, dev, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQMotionOffsetValue_Query(ViSession vi, int name, int ch, float *distance);
    def mrgMRQMotionOffsetValue_Query(self, fd, dev, ch):
        result = c_int(-1)
        ret = self._dll.mrgMRQMotionOffsetValue_Query(fd, dev, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQMotionABCount_Query(ViSession vi, int name, int ch);
    def mrgMRQMotionABCount_Query(self, fd, int_name, int_ch):
        return self._dll.mrgMRQMotionABCount_Query(fd, int_name, int_ch)

# int mrgMRQMotionABCountClear(ViSession vi, int name, int ch);
    def mrgMRQMotionABCountClear(self, fd, int_name, int_ch):
        return self._dll.mrgMRQMotionABCountClear(fd, int_name, int_ch)

# int mrgMRQMotionReverse(ViSession vi, int name, int state);
    def mrgMRQMotionReverse(self, fd, int_name, int_state):
        return self._dll.mrgMRQMotionReverse(fd, int_name, int_state)

# int mrgMRQMotionReverse_Query(ViSession vi, int name,int * reverse);
    def mrgMRQMotionReverse_Query(self, fd, dev):
        result = c_int(-1)
        ret = self._dll.mrgMRQMotionReverse_Query(fd, dev, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQAdjust(ViSession vi, int name, int ch, int wavetable,float position, float time, int timeout_ms);
    def mrgMRQAdjust(self, fd, int_name, int_ch, int_wavetable,float_position, float_time, int_timeout_ms):
        return self._dll.mrgMRQAdjust(fd, int_name, int_ch, int_wavetable, c_float(float_position), c_float(float_time), int_timeout_ms)

# int mrgMRQClockSync(ViSession vi, char *name_list, float time);
    def mrgMRQClockSync(self, fd, strNameList, tm):
        return self._dll.mrgMRQClockSync(fd, string_to_charp(strNameList), c_float(tm))


# int mrgMRQMotorStepAngle(ViSession vi, int name, int ch, int stepangle);
    def mrgMRQMotorStepAngle(self, fd, int_name, int_ch, int_stepangle):
        return self._dll.mrgMRQMotorStepAngle(fd, int_name, int_ch, int_stepangle)

# int mrgMRQMotorStepAngle_Query(ViSession vi, int name, int ch, int *stepangle);
    def mrgMRQMotorStepAngle_Query(self, fd, dev, ch):
        result = c_int(-1)
        ret = self._dll.mrgMRQMotorStepAngle_Query(fd, dev, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQMotorMotionType(ViSession vi, int name, int ch, int type);
    def mrgMRQMotorMotionType(self, fd, int_name, int_ch, int_type):
        return self._dll.mrgMRQMotorMotionType(fd, int_name, int_ch, int_type)

# int mrgMRQMotorMotionType_Query(ViSession vi, int name, int ch, int *type);
    def mrgMRQMotorMotionType_Query(self, fd, dev, ch):
        result = c_int(-1)
        ret = self._dll.mrgMRQMotorMotionType_Query(fd, dev, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQMotorPositionUnit(ViSession vi, int name, int ch, int unit);
    def mrgMRQMotorPositionUnit(self, fd, int_name, int_ch, int_unit):
        return self._dll.mrgMRQMotorPositionUnit(fd, int_name, int_ch, int_unit)

# int mrgMRQMotorPositionUnit_Query(ViSession vi, int name, int ch, int *unit);
    def mrgMRQMotorPositionUnit_Query(self, fd, dev, ch):
        result = c_int(-1)
        ret = self._dll.mrgMRQMotorPositionUnit_Query(fd, dev, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQMotorGearRatio(ViSession vi, int name, int ch, int a, int b);
    def mrgMRQMotorGearRatio(self, fd, int_name, int_ch, int_a, int_b):
        return self._dll.mrgMRQMotorGearRatio(fd, int_name, int_ch, int_a, int_b)

# int mrgMRQMotorGearRatio_Query(ViSession vi, int name, int ch, int *a, int *b);
    def mrgMRQMotorGearRatio_Query(self, fd, name, ch):
        a = c_int(-1)
        b = c_int(-1)
        ret = self._dll.mrgMRQMotorGearRatio_Query(fd, name, ch, byref(a), byref(b) )
        if ret < 0:
            return list()
        else:
            return [a.value, b.value]

# int mrgMRQMotorLead(ViSession vi, int name, int ch, float millimeter);
    def mrgMRQMotorLead(self, fd, int_name, int_ch, float_millimeter):
        return self._dll.mrgMRQMotorLead(fd, int_name, int_ch, c_float(float_millimeter))


# int mrgMRQMotorLead_Query(ViSession vi, int name, int ch, float *millimeter);
    def mrgMRQMotorLead_Query(self, fd, dev, ch):
        result = c_float(-1)
        ret = self._dll.mrgMRQMotorLead_Query(fd, dev, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQMotorSize(ViSession vi, int name, int ch, int size);
    def mrgMRQMotorSize(self, fd, int_name, int_ch, int_size):
        return self._dll.mrgMRQMotorSize(fd, int_name, int_ch, int_size)

# int mrgMRQMotorSize_Query(ViSession vi, int name, int ch, int *size);
    def mrgMRQMotorSize_Query(self, fd, name, ch):
        result = c_int(-1)
        ret = self._dll.mrgMRQMotorSize_Query(fd, name, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQMotorVoltate(ViSession vi, int name, int ch, int volt);
    def mrgMRQMotorVoltate(self, fd, int_name, int_ch, int_volt):
        return self._dll.mrgMRQMotorVoltate(fd, int_name, int_ch, int_volt)

# int mrgMRQMotorVoltage_Query(ViSession vi, int name, int ch, int *volt);
    def mrgMRQMotorVoltage_Query(self, fd, name, ch):
        result = c_int(-1)
        ret = self._dll.mrgMRQMotorVoltage_Query(fd, name, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQMotorCurrent(ViSession vi, int name, int ch, float current);
    def mrgMRQMotorCurrent(self, fd, int_name, int_ch, float_current):
        return self._dll.mrgMRQMotorCurrent(fd, int_name, int_ch, c_float(float_current))

# int mrgMRQMotorCurrent_Query(ViSession vi, int name, int ch, float *current);
    def mrgMRQMotorCurrent_Query(self, fd, dev, ch):
        result = c_float(-1)
        ret = self._dll.mrgMRQMotorCurrent_Query(fd, dev, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQMotorBackLash(ViSession vi, int name, int ch, float lash);
    def mrgMRQMotorBackLash(self, fd, int_name, int_ch, float_lash):
        return self._dll.mrgMRQMotorBackLash(fd, int_name, int_ch, c_float(float_lash))

# int mrgMRQMotorBackLash_Query(ViSession vi, int name, int ch, float *lash);
    def mrgMRQMotorBackLash_Query(self, fd, dev, ch):
        result = c_float(-1)
        ret = self._dll.mrgMRQMotorBackLash_Query(fd, dev, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQPVTConfig(ViSession vi, int name, int ch, int wavetable, int state);
    def mrgMRQPVTConfig(self, fd, int_name, int_ch, int_wavetable, int_state):
        return self._dll.mrgMRQPVTConfig(fd, int_name, int_ch, int_wavetable, int_state)

# int mrgMRQPVTValue(ViSession vi, int name, int ch, int wavetable, float p, float v, float t);
    def mrgMRQPVTValue(self, fd, int_name, int_ch, int_wavetable, f_P, f_V, f_T):
        return self._dll.mrgMRQPVTValue(fd, int_name, int_ch, int_wavetable, c_float(f_P), c_float(f_V), c_float(f_T))

# int mrgMRQPVTState(ViSession vi, int name, int ch, int wavetable, int state);
    def mrgMRQPVTState(self, fd, int_name, int_ch, int_wavetable, int_state):
        return self._dll.mrgMRQPVTState(fd, int_name, int_ch, int_wavetable, int_state)

# int mrgMRQPVTState_Query(ViSession vi, int name, int ch, int wavetable, int *state1);
    def mrgMRQPVTState_Query(self, fd, dev, ch, wavetable):
        result = c_int(-1)
        ret = self._dll.mrgMRQPVTState_Query(fd, dev, ch, wavetable, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int  mrgMRQPVTLoad(ViSession vi, int name, int ch, int wavetable, float * p, float * v, float *t, int step, int line);
    def mrgMRQPVTLoad(self, fd, name, ch, wavetable, listP, listV, listT, step, line):
        self.mrgMRQPVTConfig(fd, name, ch, wavetable, 1)
        for i in range(0, line):
            self.mrgMRQPVTValue(fd, name, ch, wavetable, listP[i*step], listV[i*step], listT[i*step])
        self.mrgMRQPVTConfig(fd, name, ch, wavetable, 0)
        
# int  mrgMRQPVTRun(ViSession vi, int name, int ch, int wavetable, int timeout_ms);
    def mrgMRQPVTRun(self, fd, name, ch, wavetable, timeout_ms):
        return self._dll.mrgMRQPVTRun(fd, name, ch, wavetable, timeout_ms)

# int mrgMRQPVTStateWait(ViSession vi, int name, int ch, int wavetable, int state, int timeout_ms);
    def mrgMRQPVTStateWait(self, fd, int_name, int_ch, int_wavetable, int_state, int_timeout_ms):
        return self._dll.mrgMRQPVTStateWait(fd, int_name, int_ch, int_wavetable, int_state, int_timeout_ms)

# int  mrgMRQPVTStateWaitEnd(ViSession vi, int name, int ch, int wavetable,int timeout_ms);
    def mrgMRQPVTStateWaitEnd(self, fd, name, ch, wavetable, timeout_ms):
        return self._dll.mrgMRQPVTStateWaitEnd(fd, name, ch, wavetable, timeout_ms)

# int mrgMRQPVTTimeScale(ViSession vi, int name, int ch, int wavetable, int speedup, int speedcut);
    def mrgMRQPVTTimeScale(self, fd, int_name, int_ch, int_wavetable, int_speedup, int_speedcut):
        return self._dll.mrgMRQPVTTimeScale(fd, int_name, int_ch, int_wavetable, int_speedup, int_speedcut)

# int mrgMRQPVTTimeScale_Query(ViSession vi, int name, int ch, int wavetable, int* speedup, int* speedcut);
    def mrgMRQPVTTimeScale_Query(self, fd, dev, ch, wavetable):
        a = c_int(-1)
        b = c_int(-1)
        ret = self._dll.mrgMRQPVTTimeScale_Query(fd, dev, ch, wavetable, byref(a), byref(b) )
        if ret < 0:
            return list()
        else:
            return [a.value, b.value]

# int mrgMRQPVTCycle(ViSession vi, int name, int ch, int wavetable, unsigned int cycle);
    def mrgMRQPVTCycle(self, fd, int_name, int_ch, int_wavetable, uint_cycle):
        return self._dll.mrgMRQPVTCycle(fd, int_name, int_ch, int_wavetable, uint_cycle)

# int mrgMRQPVTCycle_Query(ViSession vi, int name, int ch, int wavetable, unsigned int *cycle);
    def mrgMRQPVTCycle_Query(self, fd, dev, ch, wavetable):
        result = c_int(-1)
        ret = self._dll.mrgMRQPVTCycle_Query(fd, dev, ch, wavetable, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQPVTFifoBufferTime(ViSession vi, int name, int ch, int wavetable, unsigned int time);
    def mrgMRQPVTFifoBufferTime(self, fd, int_name, int_ch, int_wavetable, uint_time):
        return self._dll.mrgMRQPVTFifoBufferTime(fd, int_name, int_ch, int_wavetable, uint_time)

# int mrgMRQPVTFifoBufferTime_Query(ViSession vi, int name, int ch, int wavetable, unsigned int *time);
    def mrgMRQPVTFifoBufferTime_Query(self, fd, dev, ch, wavetable):
        result = c_int(-1)
        ret = self._dll.mrgMRQPVTFifoBufferTime_Query(fd, dev, ch, wavetable, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQPVTModeConfig_Query(ViSession vi, int name, int ch,int wavetable, int *exe, int *plan, int *motion);
    def mrgMRQPVTModeConfig_Query(self, fd, dev, ch, wavetable):
        a = c_int(-1)
        b = c_int(-1)
        c = c_int(-1)
        ret = self._dll.mrgMRQPVTModeConfig_Query(fd, dev, ch, wavetable, byref(a), byref(b), byref(c) )
        if ret < 0:
            return list()
        else:
            return [a.value, b.value, c.value]

# int mrgMRQPVTModeConfig(ViSession vi, int name, int ch,int wavetable, int exe, int plan, int motion);
    def mrgMRQPVTModeConfig(self, fd, int_name, int_ch, int_wavetable, int_exe, int_plan, int_motion):
        return self._dll.mrgMRQPVTModeConfig(fd, int_name, int_ch, int_wavetable, int_exe, int_plan, int_motion)

# int mrgMRQPVTModeExe(ViSession vi, int name, int ch, int wavetable, int mode);
    def mrgMRQPVTModeExe(self, fd, int_name, int_ch, int_wavetable, int_mode):
        return self._dll.mrgMRQPVTModeExe(fd, int_name, int_ch, int_wavetable, int_mode)

# int mrgMRQPVTModeExe_Query(ViSession vi, int name, int ch, int wavetable, int *mode);
    def mrgMRQPVTModeExe_Query(self, fd, dev, ch, wavetable):
        result = c_int(-1)
        ret = self._dll.mrgMRQPVTModeExe_Query(fd, dev, ch, wavetable, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQPVTModePlan(ViSession vi, int name, int ch, int wavetable, int mode);
    def mrgMRQPVTModePlan(self, fd, int_name, int_ch, int_wavetable, int_mode):
        return self._dll.mrgMRQPVTModePlan(fd, int_name, int_ch, int_wavetable, int_mode)

# int mrgMRQPVTModePlan_Query(ViSession vi, int name, int ch, int wavetable, int *mode);
    def mrgMRQPVTModePlan_Query(self, fd, dev, ch, wavetable):
        result = c_int(-1)
        ret = self._dll.mrgMRQPVTModePlan_Query(fd, dev, ch, wavetable, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQPVTModeMotion(ViSession vi, int name, int ch, int wavetable, int pattern);
    def mrgMRQPVTModeMotion(self, fd, int_name, int_ch, int_wavetable, int_pattern):
        return self._dll.mrgMRQPVTModeMotion(fd, int_name, int_ch, int_wavetable, int_pattern)

# int mrgMRQPVTModeMotion_Query(ViSession vi, int name, int ch, int wavetable, int *pattern);
    def mrgMRQPVTModeMotion_Query(self, fd, dev, ch, wavetable):
        result = c_int(-1)
        ret = self._dll.mrgMRQPVTModeMotion_Query(fd, dev, ch, wavetable, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQPVTModifyDuty(ViSession vi, int name, int ch, int wavetable, int duty);
    def mrgMRQPVTModifyDuty(self, fd, int_name, int_ch, int_wavetable, int_duty):
        return self._dll.mrgMRQPVTModifyDuty(fd, int_name, int_ch, int_wavetable, int_duty)

# int mrgMRQPVTModifyDuty_Query(ViSession vi, int name, int ch, int wavetable, int *duty);
    def mrgMRQPVTModifyDuty_Query(self, fd, dev, ch, wavetable):
        result = c_int(-1)
        ret = self._dll.mrgMRQPVTModifyDuty_Query(fd, dev, ch, wavetable, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQPVTEndState(ViSession vi, int name, int ch, int wavetable, int pattern);
    def mrgMRQPVTEndState(self, fd, int_name, int_ch, int_wavetable, int_pattern):
        return self._dll.mrgMRQPVTEndState(fd, int_name, int_ch, int_wavetable, int_pattern)

# int mrgMRQPVTEndState_Query(ViSession vi, int name, int ch, int wavetable, int *pattern);
    def mrgMRQPVTEndState_Query(self, fd, dev, ch, wavetable):
        result = c_int(-1)
        ret = self._dll.mrgMRQPVTEndState_Query(fd, dev, ch, wavetable, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQPVTStopMode(ViSession vi, int name, int ch, int wavetable, int type);
    def mrgMRQPVTStopMode(self, fd, int_name, int_ch, int_wavetable, int_type):
        return self._dll.mrgMRQPVTStopMode(fd, int_name, int_ch, int_wavetable, int_type)

# int mrgMRQPVTStopMode_Query(ViSession vi, int name, int ch, int wavetable, int *mode);
    def mrgMRQPVTStopMode_Query(self, fd, dev, ch, wavetable):
        result = c_int(-1)
        ret = self._dll.mrgMRQPVTStopMode_Query(fd, dev, ch, wavetable, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQPVTStopTime(ViSession vi, int name, int ch, int wavetable, float time);
    def mrgMRQPVTStopTime(self, fd, int_name, int_ch, int_wavetable, float_time):
        return self._dll.mrgMRQPVTStopTime(fd, int_name, int_ch, int_wavetable, c_float(float_time))

# int mrgMRQPVTStopTime_Query(ViSession vi, int name, int ch, int wavetable, float *time);
    def mrgMRQPVTStopTime_Query(self, fd, dev, ch, wavetable):
        result = c_int(-1)
        ret = self._dll.mrgMRQPVTStopTime_Query(fd, dev, ch, wavetable, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQPVTStopDistance(ViSession vi, int name, int ch, int wavetable, float distance);
    def mrgMRQPVTStopDistance(self, fd, int_name, int_ch, int_wavetable, float_distance):
        return self._dll.mrgMRQPVTStopDistance(fd, int_name, int_ch, int_wavetable, c_float(float_distance))

# int mrgMRQPVTStopDistance_Query(ViSession vi, int name, int ch, int  wavetable, float *distance);
    def mrgMRQPVTStopDistance_Query(self, fd, dev, ch, wavetable):
        result = c_float(-1)
        ret = self._dll.mrgMRQPVTStopDistance_Query(fd, dev, ch, wavetable, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQPVTWavetableAddress(ViSession vi, int name, int ch, int wavetable, unsigned int address);
    def mrgMRQPVTWavetableAddress(self, fd, int_name, int_ch, int_wavetable, uint_address):
        return self._dll.mrgMRQPVTWavetableAddress(fd, int_name, int_ch, int_wavetable, uint_address)

# int mrgMRQPVTWavetableAddress_Query(ViSession vi, int name,int ch, int  wavetable, unsigned int * address);
    def mrgMRQPVTWavetableAddress_Query(self, fd, dev, ch, wavetable):
        result = c_int(-1)
        ret = self._dll.mrgMRQPVTWavetableAddress_Query(fd, dev, ch, wavetable, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQPVTWavetableSize(ViSession vi, int name, int ch, int wavetable, unsigned int size);
    def mrgMRQPVTWavetableSize(self, fd, int_name, int_ch, int_wavetable, uint_size):
        return self._dll.mrgMRQPVTWavetableSize(fd, int_name, int_ch, int_wavetable, uint_size)

# int mrgMRQPVTWavetableSize_Query(ViSession vi, int name,int ch, int  wavetable, unsigned int * size);
    def mrgMRQPVTWavetableSize_Query(self, fd, dev, ch, wavetable):
        result = c_int(-1)
        ret = self._dll.mrgMRQPVTWavetableSize_Query(fd, dev, ch, wavetable, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQLostStepLineConfig_Query(ViSession vi, int name, int ch, int wavetable, int *state,float *threshold, int *resp);
    def mrgMRQLostStepLineConfig_Query(self, fd, dev, ch, wavetable):
        a = c_int(-1)
        b = c_float(-1)
        c = c_int(-1)
        ret = self._dll.mrgMRQLostStepLineConfig_Query(fd, dev, ch, wavetable, byref(a), byref(b), byref(c) )
        if ret < 0:
            return list()
        else:
            return [a.value, b.value, c.value]

# int mrgMRQLostStepLineConfig(ViSession vi, int name, int ch, int wavetable, int state, float threshold, int resp);
    def mrgMRQLostStepLineConfig(self, fd, int_name, int_ch, int_wavetable, int_state, float_threshold, int_resp):
        return self._dll.mrgMRQLostStepLineConfig(fd, int_name, int_ch, int_wavetable, int_state, c_float(float_threshold), int_resp)

# int mrgMRQLostStepState(ViSession vi, int name, int ch, int wavetable, int state);
    def mrgMRQLostStepState(self, fd, int_name, int_ch, int_wavetable, int_state):
        return self._dll.mrgMRQLostStepState(fd, int_name, int_ch, int_wavetable, int_state)

# int mrgMRQLostStepState_Query(ViSession vi, int name, int ch, int wavetable, int *state);
    def mrgMRQLostStepState_Query(self, fd, dev, ch, wavetable):
        result = c_int(-1)
        ret = self._dll.mrgMRQLostStepState_Query(fd, dev, ch, wavetable, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQLostStepThreshold(ViSession vi, int name, int ch, int wavetable, float value);
    def mrgMRQLostStepThreshold(self, fd, int_name, int_ch, int_wavetable, float_value):
        return self._dll.mrgMRQLostStepThreshold(fd, int_name, int_ch, int_wavetable, c_float(float_value))

# int mrgMRQLostStepThreshold_Query(ViSession vi, int name, int ch, int wavetable, float *value);
    def mrgMRQLostStepThreshold_Query(self, fd, dev, ch, wavetable):
        result = c_float(-1)
        ret = self._dll.mrgMRQLostStepThreshold_Query(fd, dev, ch, wavetable, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQLostStepResponse(ViSession vi, int name, int ch, int wavetable, int resp );
    def mrgMRQLostStepResponse(self, fd, int_name, int_ch, int_wavetable, int_resp):
        return self._dll.mrgMRQLostStepResponse(fd, int_name, int_ch, int_wavetable, int_resp)

# int QLostStepResponse_Query(ViSession vi, int name, int ch, int wavetable, int *resp);
    def QLostStepResponse_Query(self, fd, dev, ch, wavetable):
        result = c_int(-1)
        ret = self._dll.QLostStepResponse_Query(fd, dev, ch, wavetable, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQReportConfig_Query(ViSession vi, int name, int ch, int funs, int *state, float *period);
    def mrgMRQReportConfig_Query(self, fd, dev, ch, wavetable):
        a = c_int(-1)
        b = c_float(-1)
        ret = self._dll.mrgMRQReportConfig_Query(fd, dev, ch, wavetable, byref(a), byref(b) )
        if ret < 0:
            return list()
        else:
            return [a.value, b.value]

# int mrgMRQReportConfig(ViSession vi, int name, int ch, int funs, int state, float period);
    def mrgMRQReportConfig(self, fd, int_name, int_ch, int_funs, int_state, float_period):
        return self._dll.mrgMRQReportConfig(fd, int_name, int_ch, int_funs, int_state, c_float(float_period))

# int mrgMRQReportState(ViSession vi, int name, int ch, int funs, int state);
    def mrgMRQReportState(self, fd, int_name, int_ch, int_funs, int_state):
        return self._dll.mrgMRQReportState(fd, int_name, int_ch, int_funs, int_state)

# int mrgMRQReportState_Query(ViSession vi, int name, int ch, int funs, int *state);
    def mrgMRQReportState_Query(self, fd, dev, ch, funs):
        result = c_int(-1)
        ret = self._dll.mrgMRQReportState_Query(fd, dev, ch, funs, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQReportPeriod(ViSession vi, int name, int ch, int funs, int period);
    def mrgMRQReportPeriod(self, fd, int_name, int_ch, int_funs, int_period):
        return self._dll.mrgMRQReportPeriod(fd, int_name, int_ch, int_funs, int_period)

# int mrgMRQReportPeriod_Query(ViSession vi, int name, int ch, int funs, int *period);
    def mrgMRQReportPeriod_Query(self, fd, dev, ch, funs):
        result = c_int(-1)
        ret = self._dll.mrgMRQReportPeriod_Query(fd, dev, ch, funs, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQReportData_Query(ViSession vi, int name, int ch, int index, unsigned int *data);
    def mrgMRQReportData_Query(self, fd, dev, ch, index):
        result = c_int(-1)
        ret = self._dll.mrgMRQReportData_Query(fd, dev, ch, index, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQReportQueue_Query(ViSession vi, int name, int ch, int func, unsigned int *data);
    def mrgMRQReportQueue_Query(self, fd, dev, ch, func):
        result = c_int(-1)
        ret = self._dll.mrgMRQReportQueue_Query(fd, dev, ch, func, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQTriggerMode(ViSession vi, int name, int ch, int mode);
    def mrgMRQTriggerMode(self, fd, int_name, int_ch, int_mode):
        return self._dll.mrgMRQTriggerMode(fd, int_name, int_ch, int_mode)

# int mrgMRQTriggerMode_Query(ViSession vi, int name, int ch, int *mode);
    def mrgMRQTriggerMode_Query(self, fd, dev, ch):
        result = c_int(-1)
        ret = self._dll.mrgMRQTriggerMode_Query(fd, dev, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value
			
# int mrgMRQTriggerLevelConfig_Query(ViSession vi, int name, int ch, int trig,int * state, int * type, float *period, int * response);
    def mrgMRQTriggerLevelConfig_Query(self, fd, dev, ch, trig):
        a = c_int(-1)
        b = c_int(-1)
        c = c_float(-1)
        d = c_int(-1)
        ret = self._dll.mrgMRQTriggerLevelConfig_Query(fd, dev, ch, trig, byref(a), byref(b), byref(c), byref(d))
        if ret < 0:
            return list()
        else:
            return [a.value, b.value, c.value, d.value]
            
# int mrgMRQTriggerLevelConfig(ViSession vi, int name, int ch, int trig, int state, int type, float period, int response);
    def mrgMRQTriggerLevelConfig(self, fd, int_name, int_ch, int_trig, int_state, int_type, float_period, int_response):
        return self._dll.mrgMRQTriggerLevelConfig(fd, int_name, int_ch, int_trig, int_state, int_type, c_float(float_period), int_response)

# int mrgMRQTriggerLevelState(ViSession vi, int name, int ch, int trig, int state);
    def mrgMRQTriggerLevelState(self, fd, int_name, int_ch, int_trig, int_state):
        return self._dll.mrgMRQTriggerLevelState(fd, int_name, int_ch, int_trig, int_state)

# int mrgMRQTriggerLevelState_Query(ViSession vi, int name, int ch, int trig, int *state);
    def mrgMRQTriggerLevelState_Query(self, fd, dev, ch, trig):
        result = c_int(-1)
        ret = self._dll.mrgMRQTriggerLevelState_Query(fd, dev, ch, trig, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQTriggerLevelType(ViSession vi, int name, int ch, int trig, int type);
    def mrgMRQTriggerLevelType(self, fd, int_name, int_ch, int_trig, int_type):
        return self._dll.mrgMRQTriggerLevelType(fd, int_name, int_ch, int_trig, int_type)

# int mrgMRQTriggerLevelType_Query(ViSession vi, int name, int ch, int trig, int *type);
    def mrgMRQTriggerLevelType_Query(self, fd, dev, ch, trig):
        result = c_int(-1)
        ret = self._dll.mrgMRQTriggerLevelType_Query(fd, dev, ch, trig, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQTriggerLevelResponse(ViSession vi, int name, int ch, int trig, int resp);
    def mrgMRQTriggerLevelResponse(self, fd, int_name, int_ch, int_trig, int_resp):
        return self._dll.mrgMRQTriggerLevelResponse(fd, int_name, int_ch, int_trig, int_resp)

# int mrgMRQTriggerLevelResponse_Query(ViSession vi, int name, int ch, int trig,int *resp);
    def mrgMRQTriggerLevelResponse_Query(self, fd, dev, ch, trig):
        result = c_int(-1)
        ret = self._dll.mrgMRQTriggerLevelResponse_Query(fd, dev, ch, trig, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQTriggerLevelPeriod(ViSession vi, int name, int ch, int trig, float period);
    def mrgMRQTriggerLevelPeriod(self, fd, int_name, int_ch, int_trig, float_period):
        return self._dll.mrgMRQTriggerLevelPeriod(fd, int_name, int_ch, int_trig, c_float(float_period))


# int mrgMRQTriggerLevelPeriod_Query(ViSession vi, int name, int ch, int trig, float *period);
    def mrgMRQTriggerLevelPeriod_Query(self, fd, int_name, int_ch, int_trig):
        result = c_int(-1)
        ret = self._dll.mrgMRQTriggerLevelPeriod_Query(fd, int_name, int_ch, int_trig, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQDriverConfig_Query(ViSession vi, int name, int ch, int *state, int *microstep, float*current);
    def mrgMRQDriverConfig_Query(self, fd, dev, ch):
        a = c_int(-1)
        b = c_int(-1)
        c = c_float(-1)
        ret = self._dll.mrgMRQDriverConfig_Query(fd, dev, ch, byref(a), byref(b), byref(c))
        if ret < 0:
            return list()
        else:
            return [a.value, b.value, c.value]

# int mrgMRQDriverConfig(ViSession vi, int name, int ch, int state, int microstep, float current);
    def mrgMRQDriverConfig(self, fd, int_name, int_ch, int_state, int_microstep, float_current):
        return self._dll.mrgMRQDriverConfig(fd, int_name, int_ch, int_state, int_microstep, c_float(float_current))

# int mrgMRQDriverType_Query(ViSession vi, int name, int ch, int *type);
    def mrgMRQDriverType_Query(self, fd, dev, ch):
        result = c_int(-1)
        ret = self._dll.mrgMRQDriverType_Query(fd, dev, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQDriverCurrent(ViSession vi, int name, int ch, float current);
    def mrgMRQDriverCurrent(self, fd, int_name, int_ch, float_current):
        return self._dll.mrgMRQDriverCurrent(fd, int_name, int_ch, c_float(float_current))

# int mrgMRQDriverCurrent_Query(ViSession vi, int name, int ch, float *current);
    def mrgMRQDriverCurrent_Query(self, fd, dev, ch):
        result = c_int(-1)
        ret = self._dll.mrgMRQDriverCurrent_Query(fd, dev, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQDriverIdleCurrent(ViSession vi, int name, int ch, float current);
    def mrgMRQDriverIdleCurrent(self, fd, int_name, int_ch, float_current):
        return self._dll.mrgMRQDriverIdleCurrent(fd, int_name, int_ch, c_float(float_current))

# int mrgMRQDriverIdleCurrent_Query(ViSession vi, int name, int ch, float *current);
    def mrgMRQDriverIdleCurrent_Query(self, fd, dev, ch):
        result = c_float(-1)
        ret = self._dll.mrgMRQDriverIdleCurrent_Query(fd, dev, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQDriverMicroStep(ViSession vi, int name, int ch, int microstep);
    def mrgMRQDriverMicroStep(self, fd, int_name, int_ch, int_microstep):
        return self._dll.mrgMRQDriverMicroStep(fd, int_name, int_ch, int_microstep)

# int mrgMRQDriverMicroStep_Query(ViSession vi, int name, int ch, int *microstep);
    def mrgMRQDriverMicroStep_Query(self, fd, dev, ch):
        result = c_int(-1)
        ret = self._dll.mrgMRQDriverMicroStep_Query(fd, dev, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQDriverState(ViSession vi, int name, int ch, int state);
    def mrgMRQDriverState(self, fd, int_name, int_ch, int_state):
        return self._dll.mrgMRQDriverState(fd, int_name, int_ch, int_state)

# int mrgMRQDriverState_Query(ViSession vi, int name, int ch, int *state);
    def mrgMRQDriverState_Query(self, fd, dev, ch):
        result = c_int(-1)
        ret = self._dll.mrgMRQDriverState_Query(fd, dev, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQDriverRegisterValue(ViSession vi, int name, int ch, int regIndex, unsigned int value);
    def mrgMRQDriverRegisterValue(self, fd, int_name, int_ch, int_regIndex, uint_value):
        return self._dll.mrgMRQDriverRegisterValue(fd, int_name, int_ch, int_regIndex, uint_value)

# int mrgMRQDriverRegisterValue_Query(ViSession vi, int name, int ch, int regIndex, unsigned int *value);
    def mrgMRQDriverRegisterValue_Query(self, fd, dev, ch, regIndex):
        a = c_uint(-1)
        ret = self._dll.mrgMRQDriverRegisterValue_Query(fd, dev, ch, regIndex, byref(a))
        if ret < 0:
            return -1
        else:
            return a.value

# int mrgMRQDriverTuningState(ViSession vi, int name, int ch, int state);
    def mrgMRQDriverTuningState(self, fd, int_name, int_ch, int_state):
        return self._dll.mrgMRQDriverTuningState(fd, int_name, int_ch, int_state)

# int mrgMRQDriverTuningState_Query(ViSession vi, int name, int ch, int *state);
    def mrgMRQDriverTuningState_Query(self, fd, dev, ch):
        result = c_int(-1)
        ret = self._dll.mrgMRQDriverTuningState_Query(fd, dev, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQDriverTuningMinCurrent(ViSession vi, int name, int ch, int ratio);
    def mrgMRQDriverTuningMinCurrent(self, fd, int_name, int_ch, int_ratio):
        return self._dll.mrgMRQDriverTuningMinCurrent(fd, int_name, int_ch, int_ratio)

# int mrgMRQDriverTuningMinCurrent_Query(ViSession vi, int name, int ch, int *ratio);
    def mrgMRQDriverTuningMinCurrent_Query(self, fd, dev, ch):
        result = c_int(-1)
        ret = self._dll.mrgMRQDriverTuningMinCurrent_Query(fd, dev, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQDriverTuningCurrentRegulate(ViSession vi, int name, int ch, int speedUp, int speedDown);
    def mrgMRQDriverTuningCurrentRegulate(self, fd, int_name, int_ch, int_speedUp, int_speedDown):
        return self._dll.mrgMRQDriverTuningCurrentRegulate(fd, int_name, int_ch, int_speedUp, int_speedDown)

# int mrgMRQDriverTuningCurrentRegulate_Query(ViSession vi, int name, int ch, int *speedUp, int *speedDown);
    def mrgMRQDriverTuningCurrentRegulate_Query(self, fd, dev, ch):
        a = c_int(-1)
        b = c_int(-1)
        ret = self._dll.mrgMRQDriverTuningCurrentRegulate_Query(fd, dev, ch, byref(a), byref(b))
        if ret < 0:
            return list()
        else:
            return [a.value, b.value]

# int mrgMRQEncoderConfig_Query(ViSession vi, int name, int ch,int *state, int *type, int * lineNum, int *chanNum);
    def mrgMRQEncoderConfig_Query(self, fd, dev, ch):
        a = c_int(-1)
        b = c_int(-1)
        c = c_int(-1)
        d = c_int(-1)
        ret = self._dll.mrgMRQEncoderConfig_Query(fd, dev, ch, byref(a), byref(b), byref(c), byref(d))
        if ret < 0:
            return list()
        else:
            return [a.value, b.value, c.value, d.value]

# int mrgMRQEncoderConfig(ViSession vi, int name, int ch,int state, int type, int linenum, int channelnum);
    def mrgMRQEncoderConfig(self, fd, int_name, int_ch,int_state, int_type, int_linenum, int_channelnum):
        return self._dll.mrgMRQEncoderConfig(fd, int_name, int_ch,int_state, int_type, int_linenum, int_channelnum)

# int mrgMRQEncoderLineNum(ViSession vi, int name, int ch, int num);
    def mrgMRQEncoderLineNum(self, fd, int_name, int_ch, int_num):
        return self._dll.mrgMRQEncoderLineNum(fd, int_name, int_ch, int_num)

# int mrgMRQEncoderLineNum_Query(ViSession vi, int name, int ch, int *num);
    def mrgMRQEncoderLineNum_Query(self, fd, dev, ch):
        result = c_int(-1)
        ret = self._dll.mrgMRQEncoderLineNum_Query(fd, dev, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQEncoderChannelNum(ViSession vi, int name, int ch, int channelnum);
    def mrgMRQEncoderChannelNum(self, fd, int_name, int_ch, int_channelnum):
        return self._dll.mrgMRQEncoderChannelNum(fd, int_name, int_ch, int_channelnum)

# int mrgMRQEncoderChannelNum_Query(ViSession vi, int name, int ch, int *channelnum);
    def mrgMRQEncoderChannelNum_Query(self, fd, dev, ch):
        result = c_int(-1)
        ret = self._dll.mrgMRQEncoderChannelNum_Query(fd, dev, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQEncoderType(ViSession vi, int name, int ch, int type);
    def mrgMRQEncoderType(self, fd, int_name, int_ch, int_type):
        return self._dll.mrgMRQEncoderType(fd, int_name, int_ch, int_type)

# int mrgMRQEncoderType_Query(ViSession vi, int name, int ch, int *type);
    def mrgMRQEncoderType_Query(self, fd, int_name, int_ch):
        result = c_int(-1)
        ret = self._dll.mrgMRQEncoderType_Query(fd, int_name, int_ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQEncoderMultiple(ViSession vi, int name, int ch, int multiple);
    def mrgMRQEncoderMultiple(self, fd, int_name, int_ch, int_multiple):
        return self._dll.mrgMRQEncoderMultiple(fd, int_name, int_ch, int_multiple)

# int mrgMRQEncoderMultiple_Query(ViSession vi, int name, int ch, int *multiple);
    def mrgMRQEncoderMultiple_Query(self, fd, dev, ch):
        result = c_int(-1)
        ret = self._dll.mrgMRQEncoderMultiple_Query(fd, dev, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQEncoderState(ViSession vi, int name, int ch, int state);
    def mrgMRQEncoderState(self, fd, int_name, int_ch, int_state):
        return self._dll.mrgMRQEncoderState(fd, int_name, int_ch, int_state)

# int mrgMRQEncoderState_Query(ViSession vi, int name, int ch, int*state);
    def mrgMRQEncoderState_Query(self, fd, dev, ch):
        result = c_int(-1)
        ret = self._dll.mrgMRQEncoderState_Query(fd, dev, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQEncoderFeedback(ViSession vi, int name, int ch, int value);
    def mrgMRQEncoderFeedback(self, fd, int_name, int_ch, int_value):
        return self._dll.mrgMRQEncoderFeedback(fd, int_name, int_ch, int_value)

# int mrgMRQEncoderFeedback_Query(ViSession vi, int name, int ch, int *value);
    def mrgMRQEncoderFeedback_Query(self, fd, dev, ch):
        result = c_int(-1)
        ret = self._dll.mrgMRQEncoderFeedback_Query(fd, dev, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQEncoderDirection(ViSession vi, int name, int ch, int value);
    def mrgMRQEncoderDirection(self, fd, int_name, int_ch, int_value):
        return self._dll.mrgMRQEncoderDirection(fd, int_name, int_ch, int_value)

# int mrgMRQEncoderDirection_Query(ViSession vi, int name, int ch, int *value);
    def mrgMRQEncoderDirection_Query(self, fd, dev, ch):
        result = c_int(-1)
        ret = self._dll.mrgMRQEncoderDirection_Query(fd, dev, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQAbsEncoderAlarmState(ViSession vi, int name, int ch, int state);
    def mrgMRQAbsEncoderAlarmState(self, fd, int_name, int_ch, int_state):
        return self._dll.mrgMRQAbsEncoderAlarmState(fd, int_name, int_ch, int_state)

# int mrgMRQAbsEncoderAlarmState_Query(ViSession vi, int name, int ch, int *state);
    def mrgMRQAbsEncoderAlarmState_Query(self, fd, dev, ch):
        result = c_int(-1)
        ret = self._dll.mrgMRQAbsEncoderAlarmState_Query(fd, dev, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQAbsEncoderAlarmUpLimit(ViSession vi, int name, int ch, int value);
    def mrgMRQAbsEncoderAlarmUpLimit(self, fd, int_name, int_ch, int_value):
        return self._dll.mrgMRQAbsEncoderAlarmUpLimit(fd, int_name, int_ch, int_value)

# int mrgMRQAbsEncoderAlarmUpLimit_Query(ViSession vi, int name, int ch, int *value);
    def mrgMRQAbsEncoderAlarmUpLimit_Query(self, fd, dev, ch):
        result = c_int(-1)
        ret = self._dll.mrgMRQAbsEncoderAlarmUpLimit_Query(fd, dev, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQAbsEncoderAlarmDownLimit(ViSession vi, int name, int ch, int value);
    def mrgMRQAbsEncoderAlarmDownLimit(self, fd, int_name, int_ch, int_value):
        return self._dll.mrgMRQAbsEncoderAlarmDownLimit(fd, int_name, int_ch, int_value)

# int mrgMRQAbsEncoderAlarmDownLimit_Query(ViSession vi, int name, int ch, int *value);
    def mrgMRQAbsEncoderAlarmDownLimit_Query(self, fd, dev, ch):
        result = c_int(-1)
        ret = self._dll.mrgMRQAbsEncoderAlarmDownLimit_Query(fd, dev, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQAbsEncoderAlarmResponse(ViSession vi, int name, int ch, int value);
    def mrgMRQAbsEncoderAlarmResponse(self, fd, int_name, int_ch, int_value):
        return self._dll.mrgMRQAbsEncoderAlarmResponse(fd, int_name, int_ch, int_value)

# int mrgMRQAbsEncoderAlarmResponse_Query(ViSession vi, int name, int ch, int *value);
    def mrgMRQAbsEncoderAlarmResponse_Query(self, fd, dev, ch):
        result = c_int(-1)
        ret = self._dll.mrgMRQAbsEncoderAlarmResponse_Query(fd, dev, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQAbsEncoderZeroValue(ViSession vi, int name, int ch, int value);
    def mrgMRQAbsEncoderZeroValue(self, fd, int_name, int_ch, int_value):
        return self._dll.mrgMRQAbsEncoderZeroValue(fd, int_name, int_ch, int_value)

# int mrgMRQAbsEncoderZeroValue_Query(ViSession vi, int name, int ch, int *value);
    def mrgMRQAbsEncoderZeroValue_Query(self, fd, dev, ch):
        result = c_int(-1)
        ret = self._dll.mrgMRQAbsEncoderZeroValue_Query(fd, dev, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQUartConfig(ViSession vi, int num, int name, int baud,char parity, int wordlen, int stopbit);    
    def mrgMRQUartConfig(self, fd, int_num, int_name, int_baud, int_parity, int_wordlen, int_stopbit):
        return self._dll.mrgMRQUartConfig(fd, int_num, int_name, int_baud, int_parity, int_wordlen, int_stopbit)

# int mrgMRQUartConfig_Query(ViSession vi, int num, int name, int *baud,char * parity,int * wordlen,int * stopbit);
    def mrgMRQUartConfig_Query(self, fd, num, dev):
        a = c_int(-1)
        b = c_char(-1)
        c = c_int(-1)
        d = c_int(-1)
        ret = self._dll.mrgMRQUartConfig_Query(fd, num, dev, byref(a), byref(b), byref(c), byref(d))
        if ret < 0:
            return list()
        else:
            return [a.value, b.value, c.value, d.value]

# int mrgMRQUartFlowctrl(ViSession vi, int num, int name, int mode);
    def mrgMRQUartFlowctrl(self, fd, int_num, int_name, int_mode):
        return self._dll.mrgMRQUartFlowctrl(fd, int_num, int_name, int_mode)

# int mrgMRQUartFlowctrl_Query(ViSession vi, int num, int name, int *mode);
    def mrgMRQUartFlowctrl_Query(self, fd, dev):
        result = c_int(-1)
        ret = self._dll.mrgMRQUartFlowctrl_Query(fd, dev, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQUartSensorState(ViSession vi, int num, int num1, int name, int state);
    def mrgMRQUartSensorState(self, fd, int_num, int_num1, int_name, int_state):
        return self._dll.mrgMRQUartSensorState(fd, int_num, int_num1, int_name, int_state)

# int mrgMRQUartSensorState_Query(ViSession vi, int num, int num1, int name, int *state);
    def mrgMRQUartSensorState_Query(self, fd, int_num, int_num1, int_name):
        result = c_int(-1)
        ret = self._dll.mrgMRQUartSensorState_Query(fd, int_num, int_num1, int_name, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQUartSensorConfAll(ViSession vi, int num, int num1, int name, int sof, int framelen, int num2, int period);
    def mrgMRQUartSensorConfAll(self, fd, int_num, int_num1, int_name, int_sof, int_framelen, int_num2, int_period):
        return self._dll.mrgMRQUartSensorConfAll(fd, int_num, int_num1, int_name, int_sof, int_framelen, int_num2, int_period)

# int mrgMRQUartSensorConfAll_Query(ViSession vi, int num,int num1, int name, int *sof, int* framelen, int* framenum, int* period);
    def mrgMRQUartSensorConfAll_Query(self, fd, num, num1, dev):
        a = c_int(-1)
        b = c_int(-1)
        c = c_int(-1)
        d = c_int(-1)
        ret = self._dll.mrgMRQUartSensorConfAll_Query(fd, num, num1, dev, byref(a), byref(b), byref(c), byref(d))
        if ret < 0:
            return list()
        else:
            return [a.value, b.value, c.value, d.value]

# int mrgMRQUartSensorConfSof(ViSession vi, int num, int num1, int name, int sof);
    def mrgMRQUartSensorConfSof(self, fd, int_num, int_num1, int_name, int_sof):
        return self._dll.mrgMRQUartSensorConfSof(fd, int_num, int_num1, int_name, int_sof)

# int mrgMRQUartSensorConfSof_Query(ViSession vi, int num, int num1, int name, int *sof);
    def mrgMRQUartSensorConfSof_Query(self, fd, int_num, int_num1, int_name):
        result = c_int(-1)
        ret = self._dll.mrgMRQUartSensorConfSof_Query(fd, int_num, int_num1, int_name, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQUartSensorConfFrameLen(ViSession vi, int num, int num1, int name, int len);
    def mrgMRQUartSensorConfFrameLen(self, fd, int_num, int_num1, int_name, int_len):
        return self._dll.mrgMRQUartSensorConfFrameLen(fd, int_num, int_num1, int_name, int_len)

# int mrgMRQUartSensorConfFrameLen_Query(ViSession vi, int num, int num1, int name, int *len);
    def mrgMRQUartSensorConfFrameLen_Query(self, fd, int_num, int_num1, int_name):
        result = c_int(-1)
        ret = self._dll.mrgMRQUartSensorConfFrameLen_Query(fd, int_num, int_num1, int_name, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQUartSensorConfRecvNum(ViSession vi, int num, int num1, int name, int num2);
    def mrgMRQUartSensorConfRecvNum(self, fd, int_num, int_num1, int_name, int_num2):
        return self._dll.mrgMRQUartSensorConfRecvNum(fd, int_num, int_num1, int_name, int_num2)

# int mrgMRQUartSensorConfRecvNum_Query(ViSession vi, int num, int num1, int name, int *num2);
    def mrgMRQUartSensorConfRecvNum_Query(self, fd, int_num, int_num1, int_name):
        result = c_int(-1)
        ret = self._dll.mrgMRQUartSensorConfRecvNum_Query(fd, int_num, int_num1, int_name, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQUartSensorConfPeriod(ViSession vi, int num, int num1, int name, int period);
    def mrgMRQUartSensorConfPeriod(self, fd, int_num, int_num1, int_name, int_period):
        return self._dll.mrgMRQUartSensorConfPeriod(fd, int_num, int_num1, int_name, int_period)

# int mrgMRQUartSensorConfPeriod_Query(ViSession vi, int num, int num1, int name, int *period);
    def mrgMRQUartSensorConfPeriod_Query(self, fd, int_num, int_num1, int_name):
        result = c_int(-1)
        ret = self._dll.mrgMRQUartSensorConfPeriod_Query(fd, int_num, int_num1, int_name, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQUartSensorData_Query(ViSession vi, int num, int num1, int name, char *buf);
    def mrgMRQUartSensorData_Query(self, fd, num, num1, name):
        buf = create_string_buffer(1024)
        ret = self._dll.mrgMRQUartSensorData_Query(fd, num, num1, name, buf)
        if ret < 0:
            return ""
        else:
            return cbuf_to_string(buf)

# int mrgMRQDistanceAlarmState_Query(ViSession vi, int name, int ch, int *state);
    def mrgMRQDistanceAlarmState_Query(self, fd, dev, ch):
        result = c_int(-1)
        ret = self._dll.mrgMRQDistanceAlarmState_Query(fd, dev, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value
            
# int mrgMRQDistanceAlarmState(ViSession vi, int name, int ch, int state);
    def mrgMRQDistanceAlarmState(self, fd, int_name, int_ch, int_state):
        return self._dll.mrgMRQDistanceAlarmState(fd, int_name, int_ch, int_state)

# int mrgMRQDistanceAlarm(ViSession vi, int name, int num, int ch, float distance);
    def mrgMRQDistanceAlarm(self, fd, int_name, int_num, int_ch, float_distance):
        return self._dll.mrgMRQDistanceAlarm(fd, int_name, int_num, int_ch, c_float(float_distance))

# int mrgMRQDistanceAlarm_Query(ViSession vi, int num, int name, int ch, float *distance);
    def mrgMRQDistanceAlarm_Query(self, fd, num, name, ch):
        x = c_float(-1)
        ret = self._dll.mrgMRQDistanceAlarm_Query(fd, num, name, ch, byref(x) )
        if ret < 0:
            return -1
        else:
            return x.value

# int mrgMRQNewDriverType_Query(ViSession vi, int name, int ch, int *type);
    def mrgMRQNewDriverType_Query(self, fd, name, ch):
        result = c_int(-1)
        ret = self._dll.mrgMRQNewDriverType_Query(fd, name, ch, byref(result))
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQNewDriverCurrent(ViSession vi, int name, float current);
    def mrgMRQNewDriverCurrent(self, fd, int_name, float_current):
        return self._dll.mrgMRQNewDriverCurrent(fd, int_name, c_float(float_current))

# int mrgMRQNewDriverCurrent_Query(ViSession vi, int name, float *current);
    def mrgMRQNewDriverCurrent_Query(self, fd, dev):
        result = c_int(-1)
        ret = self._dll.mrgMRQNewDriverCurrent_Query(fd, dev, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQNewDriverMicrosteps(ViSession vi, int name, int microstep);
    def mrgMRQNewDriverMicrosteps(self, fd, int_name, int_microstep):
        return self._dll.mrgMRQNewDriverMicrosteps(fd, int_name, int_microstep)

# int mrgMRQNewDriverMicrosteps_Query(ViSession vi, int name, int *microstep);
    def mrgMRQNewDriverMicrosteps_Query(self, fd, dev):
        result = c_int(-1)
        ret = self._dll.mrgMRQNewDriverMicrosteps_Query(fd, dev, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQNewDriverState_Query(ViSession vi, int name, int ch, int*state);
    def mrgMRQNewDriverState_Query(self, fd, dev, ch):
        result = c_int(-1)
        ret = self._dll.mrgMRQNewDriverState_Query(fd, dev, ch, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

# int mrgMRQNewDriverState(ViSession vi, int name, int ch, int state);
    def mrgMRQNewDriverState(self, fd, int_name, int_ch, int_state):
        return self._dll.mrgMRQNewDriverState(fd, int_name, int_ch, int_state)




############################################################
### "system.h"
############################################################
### int mrgSysGetProjectSoftVersion(ViSession vi, char *version);
    def mrgSysGetProjectSoftVersion(self, fd):
        buf = create_string_buffer(1024)
        ret = self._dll.mrgSysGetProjectSoftVersion(fd, buf)
        if ret < 0:
            return ""
        else:
            return cbuf_to_string(buf)

### int  mrgSysGetSoftVersion(ViSession vi, char * version);
    def mrgSysGetSoftVersion(self, fd):
        buf = create_string_buffer(1024)
        ret = self._dll.mrgSysGetSoftVersion(fd, buf)
        if ret < 0:
            return ""
        else:
            return cbuf_to_string(buf)

### int  mrgSysGetBackBoardSoftVersion(ViSession vi,char * version);
    def mrgSysGetBackBoardSoftVersion(self, fd):
        buf = create_string_buffer(1024)
        ret = self._dll.mrgSysGetBackBoardSoftVersion(fd, buf)
        if ret < 0:
            return ""
        else:
            return cbuf_to_string(buf)
 
### int  mrgSysGetBackBoardHardVersion(ViSession vi, char * version);
    def mrgSysGetBackBoardHardVersion(self, fd):
        buf = create_string_buffer(1024)
        ret = self._dll.mrgSysGetBackBoardHardVersion(fd, buf)
        if ret < 0:
            return ""
        else:
            return cbuf_to_string(buf)

### int mrgSysStartCan(ViSession vi);
    def mrgSysStartCan(self, fd):
        return self._dll.mrgSysStartCan(fd)

### int mrgSysResetCan(ViSession vi);
    def mrgSysResetCan(self, fd):
        return self._dll.mrgSysResetCan(fd)

### int mrgSysConfCan(ViSession vi, int baud);
    def mrgSysConfCan(self, fd, int_baud):
        return self._dll.mrgSysConfCan(fd, int_baud)

### int  mrgSysConfCan_Query(ViSession vi, int* baud);
    def mrgSysConfCan_Query(self, fd):
        result = c_int(-1)
        ret = self._dll.mrgSysConfCan_Query(fd, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

### int  mrgSysInstructionMode(ViSession vi, int mode);
    def mrgSysInstructionMode(self, fd, mode):
        return self._dll.mrgSysInstructionMode(fd, mode)

### int  mrgSysInstructionMode_Query(ViSession vi, int* mode);
    def mrgSysInstructionMode_Query(self, fd):
        result = c_int(-1)
        ret = self._dll.mrgSysInstructionMode_Query(fd, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value
     
### int mrgSysModeSwitch(ViSession vi, int mode);
    def mrgSysModeSwitch(self, fd, int_mode):
        return self._dll.mrgSysModeSwitch(fd, int_mode)

### int mrgGetSysMode(ViSession vi);
    def mrgGetSysMode(self, fd):
        return self._dll.mrgGetSysMode(fd)

### int  mrgSysSetNetworkIPMode(ViSession vi, int mode);
    def mrgSysSetNetworkIPMode(self, fd, mode):
        return self._dll.mrgSysSetNetworkIPMode(fd, mode)

### int  mrgSysInstructionMode_Query(ViSession vi, int* mode);
    def mrgSysGetNetworkIPMode(self, fd):
        result = c_int(-1)
        ret = self._dll.mrgSysGetNetworkIPMode(fd, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

### int mrgSysWifiScan(ViSession vi, char* result, int len);
    def mrgSysWifiScan(self, fd):
        buf = create_string_buffer(1024)
        ret = self._dll.mrgSysWifiScan(fd, buf, 1024)
        if ret < 0:
            return ""
        else:
            return cbuf_to_string(buf)

### int mrgSysWifiConfig(ViSession vi, char* name, char* passwd);
    def mrgSysWifiConfig(self, fd, name, passwd):
        return self._dll.mrgSysWifiConfig(fd, string_to_charp(name), string_to_charp(passwd))

### int mrgSysWifiUpDown(ViSession vi, int state);
    def mrgSysWifiUpDown(self, fd, int_state):
        return self._dll.mrgSysWifiUpDown(fd, int_state)

### int mrgSysWifiCheckState(ViSession vi, int timeout_ms);
    def mrgSysWifiCheckState(self, fd, int_timeout_ms):
        return self._dll.mrgSysWifiCheckState(fd, int_timeout_ms)

### int mrgSysGetWifiConnected(ViSession vi, char* name);
    def mrgSysGetWifiConnected(self, fd, name):
        return self._dll.mrgSysGetWifiConnected(fd, string_to_charp(name))

### int mrgSysSetHeartPeriod(ViSession vi, int value);
    def mrgSysSetHeartPeriod(self, fd, int_value):
        return self._dll.mrgSysSetHeartPeriod(fd, int_value)

### int mrgSysGetHeartPeriod(ViSession vi, int * value);
    def mrgSysGetHeartPeriod(self, fd):
        result = c_int(-1)
        ret = self._dll.mrgSysGetHeartPeriod(fd, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

### int mrgSysSetPowerOn(ViSession vi, int value);
    def mrgSysSetPowerOn(self, fd, int_value):
        return self._dll.mrgSysSetPowerOn(fd, int_value)

### int mrgSysGetPowerOn(ViSession vi, int * value);
    def mrgSysGetPowerOn(self, fd):
        result = c_int(-1)
        ret = self._dll.mrgSysGetPowerOn(fd, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value
            
### int mrgSysSetInterface(ViSession vi, int face);
    def mrgSysSetInterface(self, fd, int_face):
        return self._dll.mrgSysSetInterface(fd, int_face)

### int mrgSysGetInterface(ViSession vi, int *face);
    def mrgSysGetInterface(self, fd):
        result = c_int(-1)
        ret = self._dll.mrgSysGetInterface(fd, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

### int mrgSysSetEmergencyStop(ViSession vi, int state);
    def mrgSysSetEmergencyStop(self, fd, int_state):
        return self._dll.mrgSysSetEmergencyStop(fd, int_state)

### int mrgSysGetEmergencyStopState(ViSession vi, int *state);
    def mrgSysGetEmergencyStopState(self, fd):
        result = c_int(-1)
        ret = self._dll.mrgSysGetEmergencyStopState(fd, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

### int mrgSysUpdateFileSearch(ViSession vi, char *fileList);
    def mrgSysUpdateFileSearch(self, fd):
        buf = create_string_buffer(1024)
        ret = self._dll.mrgSysUpdateFileSearch(fd, buf)
        if ret < 0:
            return ""
        else:
            return cbuf_to_string(buf)

### int mrgSysUpdateFileStart(ViSession vi, char *filename);
    def mrgSysUpdateFileStart(self, fd, name):
        return self._dll.mrgSysUpdateFileStart(fd, string_to_charp(name))

### int mrgSysUpdateFirmwareStatusQuery(ViSession vi);
    def mrgSysUpdateFirmwareStatusQuery(self, fd):
        return self._dll.mrgSysUpdateFirmwareStatusQuery(fd)

### int mrgScriptSearch(ViSession vi, int isUdisk, char *fileList);
    def mrgScriptSearch(self, fd, id):
        buf = create_string_buffer(1024)
        ret = self._dll.mrgScriptSearch(fd, id, buf)
        if ret < 0:
            return ""
        else:
            return cbuf_to_string(buf)

### int mrgScriptConfig(ViSession vi, char *filename, int isBoot);
    def mrgScriptConfig(self, fd, filename, isBoot):
        return self._dll.mrgScriptConfig(fd, string_to_charp(filename), isBoot)

### int mrgScriptConfigQuery(ViSession vi, char *filename);
    def mrgScriptConfigQuery(self, fd):
        buf = create_string_buffer(1024)
        ret = self._dll.mrgScriptConfigQuery(fd, buf)
        if ret < 0:
            return ""
        else:
            return cbuf_to_string(buf)

### int mrgScriptRun(ViSession vi);
    def mrgScriptRun(self, fd):
        return self._dll.mrgScriptRun(fd)
    
### int mrgScriptStop(ViSession vi);
    def mrgScriptStop(self, fd):
        return self._dll.mrgScriptStop(fd)
    
### int mrgScriptGetCurrentStates(ViSession vi);
    def mrgScriptGetCurrentStates(self, fd):
        return self._dll.mrgScriptGetCurrentStates(fd)

### int mrgSystemRunCmd(ViSession vi, char *cmd, int isBackground);
    def mrgSystemRunCmd(self, fd, cmd, isBackground):
        return self._dll.mrgSystemRunCmd(fd, string_to_charp(cmd), isBackground)

############################################################
### "storage.h"
############################################################
### int mrgStorageMotionFileQuery(ViSession vi, int type, char* fileList, int len);
    def mrgStorageMotionFileQuery(self, fd, tp):
        buf = create_string_buffer(1024)
        ret = self._dll.mrgStorageMotionFileQuery(fd, tp, buf, 1024)
        if ret < 0:
            return ""
        else:
            return cbuf_to_string(buf)

### int mrgStorageMotionFileDelete(ViSession vi, char* fileName);
    def mrgStorageMotionFileDelete(self, fd, fileName):
        return self._dll.mrgStorageMotionFileDelete(fd, string_to_charp(fileName))

### int mrgStorageMotionFileContextRead(ViSession vi, char* filename, char* context, int wantlen);
    def mrgStorageMotionFileContextRead(self, fd, filename):
        buf = create_string_buffer(1024 * 40)
        ret = self._dll.mrgStorageMotionFileContextRead(fd, string_to_charp(filename), buf, 1024 * 40)
        if ret < 0:
            return ""
        else:
            return cbuf_to_string(buf)

### int mrgStorageMotionFileSaveContext(ViSession vi, char* context, int len, char * saveFileName);
    def mrgStorageMotionFileSaveContext(self, fd, context, saveFileName):
        return self._dll.mrgStorageMotionFileSaveContext(fd, string_to_charp(context), len(context), string_to_charp(saveFileName))

### int mrgStorageWriteFile(ViSession vi, int isUdisk, char *path, char *saveFileName, unsigned char *data, int dataLen);
    def mrgStorageWriteFile(self, fd, isUdisk, path, saveFileName, data):
        return self._dll.mrgStorageWriteFile(fd, isUdisk, string_to_charp(path), string_to_charp(saveFileName), string_to_charp(data), len(data))

### int mrgStorageReadFile(ViSession vi, int isUdisk, char *path, char* filename, unsigned char* context);
    def mrgStorageReadFile(self, fd, isUdisk, path, filename):
        buf = create_string_buffer(1024 * 1024)
        ret = self._dll.mrgStorageReadFile(fd, isUdisk, string_to_charp(path), string_to_charp(filename), buf, 1024 * 1024)
        if ret < 0:
            return ""
        else:
            return cbuf_to_string(buf)


############################################################
### "errorcode.h"
############################################################
### int CALL mrgErrorCodeConfigUpload(ViSession vi, int code, int *type, int *response, int *diagnose, int *enable);
    def mrgErrorCodeConfigUpload(self, fd, code):
        a = c_int(-1)
        b = c_int(-1)
        c = c_int(-1)
        d = c_int(-1)
        ret = self._dll.mrgErrorCodeConfigUpload(fd, code, byref(a), byref(b), byref(c), byref(d))
        if ret < 0:
            return list()
        else:
            return [a.value, b.value, c.value, d.value]

### int CALL mrgErrorCodeConfigDownload(ViSession vi, int code, int type, int response, int diagnose, int enable);
    def mrgErrorCodeConfigDownload(self, fd, int_code, int_type, int_response, int_diagnose, int_enable):
        return self._dll.mrgErrorCodeConfigDownload(fd, int_code, int_type, int_response, int_diagnose, int_enable)

### int CALL mrgErrorLogUpload(ViSession vi, int format, char* errorLog, int len);
    def mrgErrorLogUpload(self, fd, fmt):
        buf = create_string_buffer(1024 * 40)
        ret = self._dll.mrgErrorLogUpload(fd, fmt, buf, 1024 * 40)
        if ret < 0:
            return ""
        else:
            return cbuf_to_string(buf)


### int CALL mrgErrorLogClear(ViSession vi);
    def mrgErrorLogClear(self, fd):
        return self._dll.mrgErrorLogClear(fd)




############################################################
### "project.h"
############################################################
### int mrgSetProjectMode(ViSession vi, int state);
    def mrgSetProjectMode(self, fd, int_state):
        return self._dll.mrgSetProjectMode(fd, int_state)

### int mrgProjectGetXinState(ViSession vi, int index, unsigned int *state)
    def mrgProjectGetXinState(self, fd, index):
        result = c_uint(0)
        ret = self._dll.mrgProjectGetXinState(fd, index, byref(result) )
        if ret < 0:
            return -1
        else:
            return result.value

### int mrgProjectSetYout(ViSession vi, int index, int state);
    def mrgProjectSetYout(self, fd, int_index, int_state):
        return self._dll.mrgProjectSetYout(fd, int_index, int_state)

### int mrgProjectSetSerialNum(ViSession vi, char * serial);
    def mrgProjectSetSerialNum(self, fd, serial):
        return self._dll.mrgProjectSetSerialNum(fd, string_to_charp(serial))

### int mrgProjectGetSerialNum(ViSession vi, char * serial);
    def mrgProjectGetSerialNum(self, fd):
        buf = create_string_buffer(1024)
        ret = self._dll.mrgProjectGetSerialNum(fd, buf)
        if ret < 0:
            return ""
        else:
            return cbuf_to_string(buf)













if __name__ == "__main__":
    """
    """
    gw = MageGateway()
    mrhtList = gw.mrgFindGateWay(0)
    print(mrhtList)