#ifndef _MRQ_DEVICE_H_
#define _MRQ_DEVICE_H_

#include "bus.h"

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif
        
/*
*MRQ模块识别
*vi :visa设备句柄
*name：设备名称(SEND_ID)
*state：识别状态 0 ON 1 OFF
*返回值：0表示执行成功，－1表示失败
*/
EXPORT_API int CALL mrgMRQIdentify(ViSession vi, int name, int state);
/*
*MRQ工作模式, 工作在Boot 或 APp
*vi :visa设备句柄
*name：设备名称(SEND_ID)
*state：1表示工作在boot模式下,2表示工作在APP模式下
*返回值：0表示执行成功，－1表示失败
*/
EXPORT_API int CALL mrgGetMRQWorkState(ViSession vi, int name, int * state);
/*
*查询设备模块的IO 状态
*vi :visa设备句柄
*name :设备名称
* state : DIO state
* 返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgGetMRQDioState(ViSession vi, int name, unsigned short * pu16State);
/*
* 将指定的设备分在一个组中
* vi :visa设备句柄
* devList :设备名称 "512,513,514"
* groupID : 组ID,由下层返回.
* grouptype: 0:GOUPID1, 1:GROUPID2
* 返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgGetMRQGroup(ViSession vi, char * devList, unsigned int * groupID, int grouptype);
/*
*设置运行状态是否自动上报给微控器
*vi :visa设备句柄
*name：设备名称(SEND_ID)
*ch：通道号
*state：状态 0 ACTIVE 1 QUERY
*返回值：0表示执行成功，－1表示失败
*/

EXPORT_API int CALL mrgMRQMotionStateReport(ViSession vi, int name, int ch, int state);
/*
*查询上报状态
*vi :visa设备句柄
*name设备名称(SEND_ID)
*ch：通道号
*ps32State:  0 ACTIVE 1 QUERY；
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQMotionStateReport_Query(ViSession vi, int name, int ch,int *ps32State);
/*
*设置启动运行的触发源
*vi :visa设备句柄
* name:设备名称(SEND_ID)
*ch：通道号
*source:触发源 0:SOFTWARE 1:DIGITALIO 2:CAN 3:ALL
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQMotionTrigSource(ViSession vi, int name, int ch, int source);
/*
*查询启动运行触发源
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*source:触发源 0:SOFTWARE 1:DIGITALIO 2:CAN 3:ALL
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQMotionTrigSource_Query(ViSession vi, int name, int ch, int * source);
/*
*设置电机未运动时发生位移是否上报给微控器
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*state:状态
*返回值：0表示执行成功，－1表示失败
*/
EXPORT_API int CALL mrgMRQMotionOffsetState(ViSession vi, int name, int ch, int state);
/*
*查询电机未运动时发生位移是否上报给微控器状态
*vi :visa设备句柄
*name: 机器人名称
*ch：通道号
*state:状态  0:OFF ; 1: ON
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQMotionOffsetState_Query(ViSession vi, int name, int ch, int *state);
/*
*查询电机未运动时发生的位移
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*displace:位移
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQMotionOffsetValue_Query(ViSession vi, int name, int ch, float *distance);
/*
*查询增量编码器的AB相的计数值
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*返回值：AB相的计数值
*/
EXPORT_API int CALL mrgMRQMotionABCount_Query(ViSession vi, int name, int ch);
/*
*清空增量编码器的AB相的计数值
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*返回值：0表示执行成功，－1表示失败
*/
EXPORT_API int CALL mrgMRQMotionABCountClear(ViSession vi, int name, int ch);
/*
*查询通道当前的PVT运行时间
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*time: PVT的运行时间
*返回值: 0表示成功,否则表示失败
*/
EXPORT_API int CALL mrgMRQMotionRuntime_Query(ViSession vi, int name, int ch,float* pf32Time);
/*
*设置电机是否反向
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*state：是否反向
*返回值：0表示执行成功，－1表示失败
* 说明: 暂不支持此功能
*/
EXPORT_API int CALL mrgMRQMotionReverse(ViSession vi, int name, int state);
/*
*查询电机反向开关状态
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*reverse: 0表示没有反向；1表示反向；
*返回值：0表示执行成功；－1表示失败,-2表示参数错误
* 说明: 暂不支持此功能
*/
EXPORT_API int CALL mrgMRQMotionReverse_Query(ViSession vi, int name,int * reverse);
/*
*微调
*vi :visa设备句柄
*name: 设备名称(SEND_ID)
*ch：通道号
*position:位置
* time : 移动到目标位置期望使用的时间
* timeout_ms:等待超时时间。-1表示不等待运行结束；0表示无限等待
*返回值：0表示执行成功，－1表示失败
*/
EXPORT_API int CALL mrgMRQAdjust(ViSession vi, int name, int ch, int wavetable,float position, float time, int timeout_ms);
/*
*时钟同步
*vi :visa设备句柄
*name_list:设备名列表
*time:同步的时间
*返回值：0表示执行成功，－1表示失败
*/
EXPORT_API int CALL mrgMRQClockSync(ViSession vi, char *name_list, float time);
/*
*设置电机的步距角
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*stepangle:电机的步距角,0->1.8度，1->0.9度，2->15度 3->7.5度
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQMotorStepAngle(ViSession vi, int name, int ch, int stepangle);
/*
*查询电机的步距角
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*stepangle:电机的步距角,0->1.8度，1->0.9度，2->15度 3->7.5度
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQMotorStepAngle_Query(ViSession vi, int name, int ch, int *stepangle);
/*
*设置电机的运动类型
*vi :visa设备句柄
*name: 设备名称(SEND_ID)
*ch：通道号
*type:运动类型 0->旋转运动  1->直线运动
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQMotorMotionType(ViSession vi, int name, int ch, int type);
/*
*查询电机的运动类型
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*type:运动类型  0->旋转运动  1->直线运动
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQMotorMotionType_Query(ViSession vi, int name, int ch, int *type);
/*
*设置电机运动时的单位
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*unit:电机运动时的单位;0:ANGLE 1:RADIAN 2:MILLIMETER
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQMotorPositionUnit(ViSession vi, int name, int ch, int unit);
/*
*查询电机运动时的单位
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*unit:电机运动时的单位;0:ANGLE 1:RADIAN 2:MILLIMETER
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQMotorPositionUnit_Query(ViSession vi, int name, int ch, int *unit);
/*
*设置电机旋转运动时的速比
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*a:电机输出轴
*b:减速机输出轴
*返回值：0表示执行成功，－1表示失败
*/
EXPORT_API int CALL mrgMRQMotorGearRatio(ViSession vi, int name, int ch, int a, int b);
/*
*查询电机旋转运动时的速比
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*a:电机输出轴
*b:减速机输出轴
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQMotorGearRatio_Query(ViSession vi, int name, int ch, int *a, int *b);
/*
*设置电机直线运动时的导程
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*millimeter:电机直线运动时的导程
*返回值：0表示执行成功，－1表示失败
*/
EXPORT_API int CALL mrgMRQMotorLead(ViSession vi, int name, int ch, float millimeter);
/*
*查询电机直线运动时的导程
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*millimeter:电机直线运动时的导程
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQMotorLead_Query(ViSession vi, int name, int ch, float *millimeter);
/*
*设置电机的尺寸
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*size:电机的尺寸(8|11|14|17|23|24)
*返回值：0表示执行成功，－1表示失败
*/
EXPORT_API int CALL mrgMRQMotorSize(ViSession vi, int name, int ch, int size);
/*
*查询电机的尺寸
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*size:电机的尺寸(8|11|14|17|23|24)
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQMotorSize_Query(ViSession vi, int name, int ch, int *size);
/*
*设置电机的额定电压
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*volt:电压值
*返回值：0表示执行成功，－1表示失败
*/
EXPORT_API int CALL mrgMRQMotorVoltate(ViSession vi, int name, int ch, int volt);
/*
*查询电机的额定电压
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*volt:电压值
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQMotorVoltage_Query(ViSession vi, int name, int ch, int *volt);
/*
*设置电机的额定电流
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*current:额定电流
*返回值：0表示执行成功，－1表示失败
*/
EXPORT_API int CALL mrgMRQMotorCurrent(ViSession vi, int name, int ch, float current);
/*
*查询电机的额定电流
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*current:额定电流
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQMotorCurrent_Query(ViSession vi, int name, int ch, float *current);
/*
*设置电机的反向间隙
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*lash:电机的反向间隙
*返回值：0表示执行成功，－1表示失败
*/
EXPORT_API int CALL mrgMRQMotorBackLash(ViSession vi, int name, int ch, float lash);
/*
*查询电机的反向间隙
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*lash:电机的反向间隙
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQMotorBackLash_Query(ViSession vi, int name, int ch, float *lash);
/*
*设置电机报警灯闪烁状态
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*isOn: 是否打开,0表示关闭,1表示打开
*返回值：0表示执行成功，－1表示失败
*/
EXPORT_API int CALL mrgMRQMotorAlarmLed(ViSession vi, int name, int ch, int isOn);
/*
*查询电机报警灯闪烁状态
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*返回值：0表示执行关闭,1表示打开,-1表示错误
*/
EXPORT_API int CALL mrgMRQMotorAlarmLed_Query(ViSession vi, int name, int ch);
/*
*PVT配置命令
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*state:PVT 的配置状态：0: END ;1: CLEAR
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTConfig(ViSession vi, int name, int ch, int wavetable, int state);
/*
*下发PVT
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*p:PVT 点的位置
*v:PVT 点的速度
*t:PVT 点的时间值
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTValue(ViSession vi, int name, int ch, int wavetable, float p, float v, float t);
/*
*设置当前PVT的状态
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*state:PVT的状态  0:RESET ,1:STOP ,2:RUN ,3:PREPARE,4:EMERGSTOP
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTState(ViSession vi, int name, int ch, int wavetable, int state);
/*
*查询当前PVT下发的状态
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*state1:PVT下发的状态   0:POWERON; 1:IDLE;2:CALCING;3:CALCEND; 4:STANDBY,5:RUNNING,6:ERROR,7:RESERVE;
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTState_Query(ViSession vi, int name, int ch, int wavetable, int *state1);
/*
*等待当前PVT的状态
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*state:期望等待的状态   0:POWERON; 1:IDLE;2:CALCING;3:CALCEND; 4:STANDBY,5:RUNNING,6:ERROR;
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTStateWait(ViSession vi, int name, int ch, int wavetable, int state, int timeout_ms);
/*
*PVT下载
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*state:  
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int  mrgMRQPVTLoad(ViSession vi, int name, int ch, int wavetable, float * p, float * v, float *t, int step, int line);

/*
*等待当前PVT的解算结束状态或运行结束状态
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*返回值：0表示执行成功，－1表示失败
*/
EXPORT_API int CALL mrgMRQPVTStateWaitEnd(ViSession vi, int name, int ch, int wavetable,int timeout_ms);
/*
*运行PVT
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*timeout_ms:等待运行结束的超时时间,-1表示不等待. 0表示无限等待.
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTRun(ViSession vi, int name, int ch, int wavetable, int timeout_ms);
/*
*设置S曲线的加减速占比，两段一起，千分之
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*speedup:加速段占比
*speedcut:减速段占比
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTTimeScale(ViSession vi, int name, int ch, int wavetable, int speedup, int speedcut);
/*
*查询S曲线的加减速占比，两段一起，千分之
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*speedup:加速段占比
*speedcut:减速段占比
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTTimeScale_Query(ViSession vi, int name, int ch, int wavetable, int* speedup, int* speedcut);
/*
*设置循环模式下，PVT的循环次数
*vi :visa设备句柄
*name:设备名称（SEND_ID）
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*cycle:循环次数
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTCycle(ViSession vi, int name, int ch, int wavetable, unsigned int cycle);
/*
*查询循环模式下，PVT的循环次数
*vi :visa设备句柄
*name:设备名称（SEND_ID）
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*cycle:循环次数
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTCycle_Query(ViSession vi, int name, int ch, int wavetable, unsigned int *cycle);
/*
*设置FIFO模式下，PVT的缓冲时间
*vi :visa设备句柄
*name: 设备名称（SEND_ID）
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*time:缓冲时间，单位：ms
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTFifoBufferTime(ViSession vi, int name, int ch, int wavetable, unsigned int time);
/*
*查询FIFO模式下，PVT的缓冲时间
*vi :visa设备句柄
*name:设备名称（SEND_ID）
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*time:缓冲时间，单位：ms
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTFifoBufferTime_Query(ViSession vi, int name, int ch, int wavetable, unsigned int *time);
/*
*查询模式,包括执行模式,规划模式,运动模式
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*exe:执行模式：0: CYCLE; 1: FIFO
*plan:轨迹规划方式：0: CUBICPOLY; 1:TRAPEZOID; 2:SCURVE
*motion:运动模式： 0: PVT; 1: LVT_CORRECT ; 2: LVT_NOCORRECT
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTModeConfig_Query(ViSession vi, int name, int ch, 
								int wavetable, int *exe, int *plan, int *motion);
/*
*设置模式,包括执行模式,规划模式,运动模式
*vi :visa设备句柄
*name: 设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*exe:执行模式：0: CYCLE; 1: FIFO
*plan:轨迹规划方式：0: CUBICPOLY; 1:TRAPEZOID; 2:SCURVE
*motion:运动模式： 0: PVT; 1: LVT_CORRECT ; 2: LVT_NOCORRECT
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTModeConfig(ViSession vi, int name, int ch, 
							int wavetable, int exe, int plan, int motion);
/*
*设置执行模式,循环或者FIFO
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*mode:执行模式， 0 ：循环模式	1：FIFO模式
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTModeExe(ViSession vi, int name, int ch, int wavetable, int mode);
/*
*查询执行模式,循环或者FIFO
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*mode:执行模式；0：循环模式； 1：FIFO模式
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTModeExe_Query(ViSession vi, int name, int ch, int wavetable, int *mode);
/*
*设置规划模式:三次插值,梯形插值,或S曲线
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*pattern:规划模式 0 - 2  CUBICPOLY|TRAPEZOID｜SCURVE
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTModePlan(ViSession vi, int name, int ch, int wavetable, int mode);
/*
*查询规划模式:三次插值,梯形插值,或S曲线
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*mode:规划模式 0 - 2  CUBICPOLY|TRAPEZOID｜SCURVE
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTModePlan_Query(ViSession vi, int name, int ch, int wavetable, int *mode);
/*
*设置运动模式:PVT或者LVT
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*pattern:模式 0 - 2 PVT|LVT_CORRECT|LVT_NOCORRECT
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTModeMotion(ViSession vi, int name, int ch, int wavetable, int pattern);
/*
*查询运动模式:PVT或者LVT
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*mode:运动模式 0 - 2 PVT|LVT_CORRECT|LVT_NOCORRECT
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTModeMotion_Query(ViSession vi, int name, int ch, int wavetable, int *pattern);
/*
*设置LVT模式下进行时间调整的占比
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*duty:占空比  0:1/4 ;1:1/8;  2: 1/16; 3: 1/32
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTModifyDuty(ViSession vi, int name, int ch, int wavetable, int duty);
/*
*查询LVT模式下进行时间调整的占比
*vi :visa设备句柄
*name:设备名称（SEND_ID）
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*duty:占空比
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTModifyDuty_Query(ViSession vi, int name, int ch, int wavetable, int *duty);
/*
*设置是否为速度保持
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*state:是否末速度保持。 0不保持；1保持
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTEndState(ViSession vi, int name, int ch, int wavetable, int pattern);
/*
*查询是否为速度保持
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*state:是否末速度保持
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTEndState_Query(ViSession vi, int name, int ch, int wavetable, int *pattern);

/*
*设置急停方式,立即停止或者减速停止
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*type:急停方式;  0:立即停止；1：减速停止
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTStopMode(ViSession vi, int name, int ch, int wavetable, int type);
/*
*查询急停方式,立即停止或者减速停止
*vi :visa设备句柄
*name:设备名称（SEND_ID）
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*mode:急停方式,0:立即停止； 1：减速停止
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTStopMode_Query(ViSession vi, int name, int ch, int wavetable, int *mode);
/*
*设置急停时间
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*time:急停的时间
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTStopTime(ViSession vi, int name, int ch, int wavetable, float time);
/*
*查询急停时间
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*time:急停的时间
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTStopTime_Query(ViSession vi, int name, int ch, int wavetable, float *time);
/*
*设置减速停止时的减速距离
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*distance:减速距离
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTStopDistance(ViSession vi, int name, int ch, int wavetable, float distance);
/*
*查询减速停止时的减速距离
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*distance:减速距离
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTStopDistance_Query(ViSession vi, int name, int ch, int  wavetable, float *distance);
/*
*设置波表的起始地址
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*address:波表起始地址
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTWavetableAddress(ViSession vi, int name, int ch, int wavetable, unsigned int address);
/*
*查询波表的起始地址
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*address:波表起始地址
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTWavetableAddress_Query(ViSession vi, int name, int ch, int  wavetable, unsigned int * pf32Address);
/*
*设置波表的大小
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*size:波表大小
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTWavetableSize(ViSession vi, int name, int ch, int wavetable, unsigned int size);

/*
*查询波表的大小
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*size:波表大小
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQPVTWavetableSize_Query(ViSession vi, int name,int ch, int  wavetable, unsigned int * pu32sSize);
/*
*查询失步的状态,阈值及失步后的反应
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*state1:失步的状态 0:禁止 ；1：使能
*threshold:失步的阈值
*resp:编码器步数偏差超过阈值后的响应方式 0:NONE;1:ALARM;2:STOP;3:ALARM&STOP
*返回值：0表示执行成功，－1表示失败
*/
EXPORT_API int CALL mrgMRQLostStepLineConfig_Query(ViSession vi, int name, int ch, int wavetable, int *ps32State,unsigned short *pu16Threshold, int *ps32Resp);
/*
*设置失步的状态,阈值及失步后的反应
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*state:失步的告警状态 0:禁止 ；1：使能
*threshold:失步的阈值
*resp:编码器步数偏差超过阈值后的响应方式  0:NONE;1:ALARM;2:STOP;3:ALARM&STOP
*返回值：0表示执行成功，－1表示失败
*/
EXPORT_API int CALL mrgMRQLostStepLineConfig(ViSession vi, int name, int ch, int wavetable, int state, unsigned short threshold, int resp);
/*
*设置线间失步告警状态
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*state:警告状态
*返回值：0表示执行成功，－1表示失败
*/
EXPORT_API int CALL mrgMRQLostStepState(ViSession vi, int name, int ch, int wavetable, int state);
/*
*查询线间失步告警状态
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*state:警告状态  0:禁止 ；1：使能
*返回值：0表示执行成功，－1表示失败
*/
EXPORT_API int CALL mrgMRQLostStepState_Query(ViSession vi, int name, int ch, int wavetable, int *ps32State);
/*
*设置线间失步阈值
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*value:线间失步阈值
*返回值：0表示执行成功，－1表示失败
*/
EXPORT_API int CALL mrgMRQLostStepThreshold(ViSession vi, int name, int ch, int wavetable, unsigned short value);
/*
*查询线间失步阈值
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*value:线间失步阈值
*返回值：0表示执行成功，－1表示失败
*/
EXPORT_API int CALL mrgMRQLostStepThreshold_Query(ViSession vi, int name, int ch, int wavetable, unsigned short *pu16Value);

/*
*设置当步数偏差超过LOSTNUM后的响应方式
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*state:状态0 - 3 NONE|ALARM|STOP|ALARM&STOP
*返回值：0表示执行成功，－1表示失败
*/
EXPORT_API int CALL mrgMRQLostStepResponse(ViSession vi, int name, int ch, int wavetable, int resp);
/*
*查询当步数偏差超过LOSTNUM后的响应方式
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*wavetable:波表索引，取值范围： 0~9 MAIN|SMALL|P1|P2|P3|P4|P5|P6|P7|P8
*resp:  0:NONE;1:ALARM;2:STOP;3:ALARM&STOP
*返回值：0表示执行成功，－1表示失败
*/
EXPORT_API int mrgMRQLostStepResponse_Query(ViSession vi, int name, int ch, int wavetable, int *ps32Resp);
/*
*查询上报功能配置
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*funs: 0 ~ 5 TORQUE|CYCLE|SGALL|SGSE|DIST|ABSEN
*buf:返回的功能
*返回值：0表示执行成功，－1表示失败
*/
EXPORT_API int CALL mrgMRQReportConfig_Query(ViSession vi, int name, int ch, int funs, int *ps32State,float *pf32Period);

/*
*设置上报功能配置
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*state:状态on/off
*funs: 0 ~ 5 TORQUE|CYCLE|SGALL|SGSE|DIST|ABSEN
*period:指定类型数据的上报周期
*返回值：0表示执行成功，－1表示失败
*/
EXPORT_API int CALL mrgMRQReportConfig(ViSession vi, int name, int ch, int funs, int state, float period);
/*
*设置上报状态
*vi :visa设备句柄
*name: 设备名称(SEND_ID)
*ch：通道号
*funs: 0 ~ 5 TORQUE|CYCLE|SGALL|SGSE|DIST|ABSEN
*state:状态on/off
*返回值：0表示执行成功，－1表示失败
*/
EXPORT_API int CALL mrgMRQReportState(ViSession vi, int name, int ch, int funs, int state);
/*
*查询上报状态
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*funs: 0 ~ 5 TORQUE|CYCLE|SGALL|SGSE|DIST|ABSEN
*state1:状态on/off 0->OFF  1->ON
*返回值：0表示执行成功，－1表示失败
*/
EXPORT_API int CALL mrgMRQReportState_Query(ViSession vi, int name, int ch, int funs, int *ps32State);
/*
*设置上报周期
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*funs: 0 ~ 5 TORQUE|CYCLE|SGALL|SGSE|DIST|ABSEN
*period:指定类型数据的上报周期,单位：ms
*返回值：0表示执行成功，－1表示失败
*/
EXPORT_API int CALL mrgMRQReportPeriod(ViSession vi, int name, int ch, int funs, int period);
/*
*查询上报周期
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*func: 0 ~ 5 TORQUE|CYCLE|SGALL|SGSE|DIST|ABSEN
*period:指定类型数据的上报周期
*返回值：0表示执行成功，－1表示失败
*/
EXPORT_API int CALL mrgMRQReportPeriod_Query(ViSession vi, int name, int ch, int func, int *pf32Period);
/*
*查询自动上报数据
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*func: 0 ~ 5 TORQUE|CYCLE|SGALL|SGSE|DIST|ABSEN
*data : 返回数据的存储区
*返回值：实际返回的数据个数
*说明：此处的查询，每次只返回一个数据
*/
EXPORT_API int CALL mrgMRQReportData_Query(ViSession vi, int name, int ch, int func, unsigned int *pu32Data);
/*
*查询自动上报数据队列中的数据
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*func: 0 ~ 5 TORQUE|CYCLE|SGALL|SGSE|DIST|ABSEN
*pu32Data : 返回数据的存储区
*返回值：实际返回的数据个数
*/
EXPORT_API int CALL mrgMRQReportQueue_Query(ViSession vi, int name, int ch, int func, unsigned int *pu32Data);
/*
*设置触发输入的模式,码型触发或电平触发
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*mode:触发输入的模式,0:码型触发；1：电平触发
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQTriggerMode(ViSession vi, int name, int ch, int mode);

/*
*查询触发输入的模式,码型触发或电平触发
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*ps32Mode:触发输入的模式. 0:码型触发；1：电平触发
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQTriggerMode_Query(ViSession vi, int name, int ch, int *ps32Mode);
/*
*查询电平触发配置
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*trig:电平触发编号： 0:TRIGL 或 1:TRIGR
*ps32State:开关状态
*ps32ype:触发类型，0：无类型；1：上升沿；2；低电平；3：下降沿；4：高电平
*pf32Period:采样周期，
*ps32Response:触动触发后的反应 0：NONE，1：ALARM;2:STOP ;3:ALARM&STOP
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQTriggerLevelConfig_Query(ViSession vi, int name, int ch, int trig, int * ps32State, int * ps32ype, float *pf32Period,int * ps32Response);
/*
*设置电平触发配置
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*trig:电平触发编号： TRIGL 或 TRIGR
*state:开关状态
*type:触发类型，0：无类型；1：上升沿；2；低电平；3：下降沿；4：高电平
*period:采样周期，
*response:触动触发后的反应 0：NONE，1：ALARM;2:STOP ;3:ALARM&STOP
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQTriggerLevelConfig(ViSession vi, int name, int devList, int trig, int state, int type, float period, int response);
/*
*设置电平触发，打开或关闭
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*trig:电平触发编号： TRIGL 或 TRIGR
*state:状态
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQTriggerLevelState(ViSession vi, int name, int ch, int trig, int state);
/*
*查询电平触发，打开或关闭
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*trig:电平触发编号： TRIGL 或 TRIGR
*state:状态. 0:关闭； 1：打开
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQTriggerLevelState_Query(ViSession vi, int name, int ch, int trig, int *ps32State);
/*
*设置触发电平类型
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*trig:电平触发编号： TRIGL 或 TRIGR
*type:电平触发类型： 0 - 4 RESERVE|LOW|RISE|FALL|HIGH
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQTriggerLevelType(ViSession vi, int name, int ch, int trig, int type);
/*
*查询触发电平类型
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*trig:电平触发编号： TRIGL 或 TRIGR
*type:电平触发类型： 0:RESERVE; 1:LOW; 2: RISE; 3:FALL;4:HIGH
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQTriggerLevelType_Query(ViSession vi, int name, int ch, int trig, int *ps32Type);

/*
*设置触发电平响应
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*trig:电平触发编号： TRIGL 或 TRIGR
*resp:电平触发的响应方式：0 - 3 NONE|ALARM|STOP|ALARM&STOP
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQTriggerLevelResponse(ViSession vi, int name, int ch, int trig, int response);
/*
*查询触发电平响应
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*trig:电平触发编号： TRIGL 或 TRIGR
*resp:电平触发的响应方式：0: NONE; 1:ALARM;2:STOP; 3:ALARM&STOP
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQTriggerLevelResponse_Query(ViSession vi, int name, int ch, int trig, int *ps32Resp);
/*
*设置触发电平采样周期
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*trig:电平触发编号： TRIGL 或 TRIGR
*period:采样周期,单位：秒
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQTriggerLevelPeriod(ViSession vi, int name, int ch, int trig, float period);
/*
*查询触发电平采样周期
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*trig:电平触发编号： TRIGL 或 TRIGR
*period:采样周期
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQTriggerLevelPeriod_Query(ViSession vi, int name, int ch, int trig, float *pf32Period);
/*
*查询驱动板配置
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*state:开关状态。 0：禁止；1：使能
*microstep:微步
*current:驱动器的驱动电流
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQDriverConfig_Query(ViSession vi, int name, int ch, int *ps32State,int *ps32Microstep,float* pf32Current );
/*
*设置驱动板配置
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*state:开关状态。 0：禁止；1：使能
*microstep:微步
*current:驱动器的驱动电流
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQDriverConfig(ViSession vi, int name, int ch, int state, int microstep, float current);
/*
*查询驱动板类型
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*type:驱动板的类型  0:D17  ; 1: D23
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQDriverType_Query(ViSession vi, int name, int ch, int *ps32Type);
/*
*设置驱动板电流
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*current:驱动板电流
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQDriverCurrent(ViSession vi, int name, int ch, float current);
/*
*查询驱动板电流
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*current:驱动板电流
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQDriverCurrent_Query(ViSession vi, int name, int ch, float *ps32Current);
/*
*设置驱动板空闲电流
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*current:驱动板空闲电流
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQDriverIdleCurrent(ViSession vi, int name, int ch, float current);
/*
*查询驱动板空闲电流
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*current:驱动板空闲电流
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQDriverIdleCurrent_Query(ViSession vi, int name, int ch, float *pf32Current);
/*
*设置电机微步数
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*microstep:电机的微步数
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQDriverMicroStep(ViSession vi, int name, int devList, int microstep);
/*
*查询电机微步数
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*microstep:电机的微步数
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQDriverMicroStep_Query(ViSession vi, int name, int devList, int *ps32Microstep);
/*
*设置驱动开关状态
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*state:状态on/off. 0:OFF; 1:ON
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQDriverState(ViSession vi, int name, int ch, int state);
/*
*查询驱动开关状态
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*state1:状态on/off
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQDriverState_Query(ViSession vi, int name, int ch, int *ps32State);
/*
*设置驱动的寄存器值
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*regIndex： 寄存器地址
*value:寄存器值
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQDriverRegisterValue(ViSession vi, int name, int ch,int regIndex, unsigned int value);
/*
*查询驱动的寄存器值
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*regIndex： 寄存器地址
*value:寄存器值
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQDriverRegisterValue_Query(ViSession vi, int name, int ch, int regIndex, unsigned int *pu32Value);
/*
*设置驱动器TUNING功能的开关状态
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*state:开关状态 0：OFF；1：ON
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQDriverTuningState(ViSession vi, int name, int ch, int state);
/*
*查询驱动器TUNING功能的开关状态
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*state:开关状态 0：OFF；1：ON
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQDriverTuningState_Query(ViSession vi, int name, int ch, int *ps32State);
/*
*设置驱动器TUNING功能的最小电流比 
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*ratio:最小电流比 0：1/2；1：1/4
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQDriverTuningMinCurrent(ViSession vi, int name, int ch, int ratio);
/*
*查询驱动器TUNING功能的最小电流比 
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*ratio:最小电流比 0：1/2；1：1/4
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQDriverTuningMinCurrent_Query(ViSession vi, int name, int ch, int *ps32Ratio);
/*
*设置驱动器TUNING功能的能效上下限
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*limitUp:能效上限
*limitDown：能效下限
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQDriverTuningEfficLimit(ViSession vi, int name, int ch, int limitUp,int limitDown);
/*
*查询驱动器TUNING功能的能效上下限
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*limitUp:能效上限
*limitDown：能效下限
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQDriverTuningEfficLimit_Query(ViSession vi, int name, int ch, int *ps32LimitUp, int *ps32LimitDown);
/*
*设置驱动器TUNING功能的电流上升和下降速度
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*speedUp:电流上升速度 0：表示每整步增加一个单位的电流；
                    1：表示每整步增加两个单位的电流
                    2：表示每整步增加4个单位的电流
                    3：表示每整步增加8个单位的电流
*speedDown：电流下降速度  0：表示每32整步减小一个单位的电流；
                        1：表示每8整步减小一个单位的电流；
                        2：表示每2整步减小一个单位的电流；
                        3：表示每1整步减小一个单位的电流；
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQDriverTuningCurrentRegulate(ViSession vi, int name, int ch, int speedUp, int speedDown);
/*
*查询驱动器TUNING功能的电流上升和下降速度
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*speedUp:电流上升速度 0：表示每整步增加一个单位的电流；
                    1：表示每整步增加两个单位的电流
                    2：表示每整步增加4个单位的电流
                    3：表示每整步增加8个单位的电流
*speedDown：电流下降速度  0：表示每32整步减小一个单位的电流；
                        1：表示每8整步减小一个单位的电流；
                        2：表示每2整步减小一个单位的电流；
                        3：表示每1整步减小一个单位的电流；
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQDriverTuningCurrentRegulate_Query(ViSession vi, int name, int ch, int *ps32SpeedUp, int *ps32SpeedDown);

/*
*查询编码器的状态,类型,线数及通道个数.状态为OFF时,后面的参数可以省略
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*state:状态  0: NONE; 1: OFF;  2：ON
*type:编码器的类型： 0:INCREMENTAL 或 1: ABSOLUTE
*linenum:增量型编码器光电码盘一周的线数： 500、 1000、 1024、 2000、 2048、 4000、 4096 或 5000
*channelnum:增量型编码器的通道数： 1 或 3
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQEncoderConfig_Query(ViSession vi, int name, int ch, int *ps32State,int *ps32Type,int * ps32LineNum,int *ps32ChanNum);
/*
*设置编码器的状态,类型,线数及通道个数.状态为OFF时,后面的参数可以省略
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*state:状态  0: NONE; 1: OFF;  2：ON
*type:编码器的类型： 0:INCREMENTAL 或 1: ABSOLUTE
*linenum:增量型编码器光电码盘一周的线数： 500、 1000、 1024、 2000、 2048、 4000、 4096 或 5000
*channelnum:增量型编码器的通道数： 1 或 3
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQEncoderConfig(ViSession vi, int name, int ch, int state, int type, int linenum, int channelnum);
/*
*设置编码器线数
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*num:编码器线数
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQEncoderLineNum(ViSession vi, int name, int ch, int num);
/*
*查询编码器线数
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*num:编码器线数
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQEncoderLineNum_Query(ViSession vi, int name, int ch, int *ps32LineNum);
/*
*设置编码器通道
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*channelnum:增量型编码器的通道数： 1 或 3
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQEncoderChannelNum(ViSession vi, int name, int ch, int channelnum);
/*
*查询编码器通道
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*channelnum:增量型编码器的通道数： 1 或 3
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQEncoderChannelNum_Query(ViSession vi, int name, int ch, int *ps32ChannelNum);
/*
*设置编码器类型
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*type:编码器的类型： INCREMENTAL 或 ABSOLUTE
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQEncoderType(ViSession vi, int name, int ch, int type);
/*
*查询编码器类型
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*type:编码器的类型： 0:INCREMENTAL 或 1:ABSOLUTE
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQEncoderType_Query(ViSession vi, int name, int ch, int *ps32Type);
/*
*设置编码器信号的倍乘
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*multiple:倍乘 0 - 2 "SINGLE","DOUBLE","QUADRUPLE"
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQEncoderMultiple(ViSession vi, int name, int ch, int multiple);
/*
*查询编码器信号的倍乘
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*multiple:倍乘 0 - 2 "SINGLE","DOUBLE","QUADRUPLE"
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQEncoderMultiple_Query(ViSession vi, int name, int ch, int *ps32Multiple);
/*
*设置编码器状态
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*state:编码器的状态：0: NONE;1: OFF ;2: ON
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQEncoderState(ViSession vi, int name, int ch, int state);
/*
*查询编码器状态
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*state1:编码器的状态： 0:NONE; 1:OFF ;2: ON
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQEncoderState_Query(ViSession vi, int name, int ch, int*ps32State);
/*
*设置LVT模式下编码器反馈比
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*feed:编码器反馈比
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQEncoderFeedback(ViSession vi, int name, int ch, int value);
/*
*查询LVT模式下编码器反馈比
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*feed:编码器反馈比
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQEncoderFeedback_Query(ViSession vi, int name, int ch, int *ps32Value);
/*
*设置编码器方向 
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*value:编码器方向。0表示编码器逆时针转动，数值增加；1表示编码器逆时针转，数据减小
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQEncoderDirection(ViSession vi, int name, int ch, int value);
/*
*查询编码器方向
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*value:编码器方向。0表示编码器逆时针转动，数值增加；1表示编码器逆时针转，数据减小
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQEncoderDirection_Query(ViSession vi, int name, int ch, int *ps32Value);
/*
* 设置绝对值编码器的报警状态
* vi :visa设备句柄
* name:设备名称(SEND_ID)
* ch：通道号
* state:编码器报警状态 0:OFF; 1:ON
* 返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQAbsEncoderAlarmState(ViSession vi, int name, int ch, int state);
/*
* 查询绝对值编码器的报警状态
* vi :visa设备句柄
* name:设备名称(SEND_ID)
* ch：通道号
* state:编码器报警状态 0:OFF; 1:ON
* 返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQAbsEncoderAlarmState_Query(ViSession vi, int name, int ch, int *ps32State);
/*
* 设置绝对值编码器的报警上限
* vi :visa设备句柄
* name:设备名称(SEND_ID)
* ch：通道号
* value:编码器报警上限值(编码器线数)
* 返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQAbsEncoderAlarmUpLimit(ViSession vi, int name, int ch, int value);
/*
* 查绝对值编码器的报警上限
* vi :visa设备句柄
* name:设备名称(SEND_ID)
* ch：通道号
* value:编码器报警上限值(编码器线数)
* 返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQAbsEncoderAlarmUpLimit_Query(ViSession vi, int name, int ch, int *ps32Value);
/*
* 设置绝对值编码器的报警下限
* vi :visa设备句柄
* name:设备名称(SEND_ID)
* ch：通道号
* value:编码器报警下限值(编码器线数)
* 返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQAbsEncoderAlarmDownLimit(ViSession vi, int name, int ch, int value);
/*
* 查绝对值编码器的报警下限
* vi :visa设备句柄
* name:设备名称(SEND_ID)
* ch：通道号
* value:编码器报警下限值(编码器线数)
* 返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQAbsEncoderAlarmDownLimit_Query(ViSession vi, int name, int ch, int *ps32Value);
/*
* 设置绝对值编码器的报警响应类型
* vi :visa设备句柄
* name:设备名称(SEND_ID)
* ch：通道号
* value:响应类型 0:NONE;1:ALARM;2:STOP;3:ALARM&STOP
* 返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQAbsEncoderAlarmResponse(ViSession vi, int name, int ch, int value);
/*
* 查绝对值编码器的报警响应类型
* vi :visa设备句柄
* name:设备名称(SEND_ID)
* ch：通道号
* value:响应类型 0:NONE;1:ALARM;2:STOP;3:ALARM&STOP
* 返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQAbsEncoderAlarmResponse_Query(ViSession vi, int name, int ch, int *ps32Value);
/*
* 设置绝对值编码器的零位值
* vi :visa设备句柄
* name:设备名称(SEND_ID)
* ch：通道号
* value:编码器零位值(编码器线数)
* 返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQAbsEncoderZeroValue(ViSession vi, int name, int ch, int value);
/*
* 查绝对值编码器的零位值
* vi :visa设备句柄
* name:设备名称(SEND_ID)
* ch：通道号
* value:编码器零位值(编码器线数)
* 返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQAbsEncoderZeroValue_Query(ViSession vi, int name, int ch, int *ps32Value);
/*
*设置串口应用配置，配置校验位、数据位、停止位
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*uart : 1:UART1; 2:UART2
* baud: 9600,115200,2000000,2500000
*parity:RS232 的校验方式： N(ONE)、 E(VEN) 或 O(DD)
*wordlen:RS232 的数据长度： 8 或 9
*stopbit:RS232 数据帧中停止位的位数： 0:1个停止位；1：2个停止位；
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQUartConfig(ViSession vi, int num, int name, int baud,char parity, int wordlen, int stopbit);
/*
*查询串口应用配置，配置校验位、数据位、停止位
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*uart : 1:UART1; 2:UART2
*baud: 9600,115200,2000000,2500000
*parity:RS232 的校验方式： N(ONE)、 E(VEN) 或 O(DD)
*wordlen:RS232 的数据长度： 8 或 9
*stopbit:RS232 数据帧中停止位的位数： 0:1个停止位；1：2个停止位；
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQUartConfig_Query(ViSession vi, int uart, int name, int *ps8Baud,char * ps8Parity,int * ps8Wordlen,int * ps8Stopbit);
/*
*设置串口硬件控制流
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*uart : 1:UART1; 2:UART2
*mode:RS232 的流控制方式：  0 - 3 NONE、 RTS、 CTS 或 RTS&CTS
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQUartFlowctrl(ViSession vi, int uart, int name, int mode);
/*
*查询串口硬件控制流
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*uart : 1:UART1; 2:UART2
*mode:RS232 的流控制方式： NONE、 RTS、 CTS 或 RTS&CTS
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQUartFlowctrl_Query(ViSession vi, int uart, int name, int *ps32Mode);
/*
*设置串口传感器状态，打开或关闭
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*uart : 1:UART1; 2:UART2
*port:传感器所在端口号： 1， 2， 3 或 4
*state:打开或关闭
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQUartSensorState(ViSession vi, int uart, int port, int name, int state);
/*
*查询串口传感器状态，打开或关闭
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*uart : 1:UART1; 2:UART2
*port:传感器所在端口号： 1， 2， 3 或 4
*state:打开或关闭  0:关闭； 1：打开
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQUartSensorState_Query(ViSession vi, int uart, int port, int name, int *ps32State);
/*
*设置传感器配置，数据帧头、帧长度、周期内接收的帧数、切换周期
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*uart : 1:UART1; 2:UART2
*port:传感器所在端口号： 1， 2， 3 或 4
*sof:数据帧头
*framelen:帧长度
*num2:帧数
*period:周期
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQUartSensorConfAll(ViSession vi, int uart, int port, int name, int sof, int framelen, int num2, int period);
/*
*查询传感器配置，数据帧头、帧长度、周期内接收的帧数、切换周期
**vi :visa设备句柄
*name:设备名称(SEND_ID)
*uart : 1:UART1; 2:UART2
*port:传感器所在端口号： 1， 2， 3 或 4
*sof:数据帧头
*framelen:帧长度
*num2:帧数
*period:周期
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQUartSensorConfAll_Query(ViSession vi, int uart, int port, int name, int *ps32Sof, int* ps32Framelen, int* ps32Framenum, int* ps32Period);
/*
*设置数据帧头
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*uart : 1:UART1; 2:UART2
*port:传感器所在端口号： 1， 2， 3 或 4
*sof:数据帧头
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQUartSensorConfSof(ViSession vi, int uart, int port, int name, int sof);
/*
*查询数据帧头
*vi :visa设备句柄
*name :设备名称(SEND_ID)
*uart : 1:UART1; 2:UART2
*port:传感器所在端口号： 1， 2， 3 或 4
* sof : 数据帧头
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQUartSensorConfSof_Query(ViSession vi, int uart, int port, int name, int *ps32Sof);
/*
*设置帧长度
*vi :visa设备句柄
*name :设备名称(SEND_ID)
*uart : 1:UART1; 2:UART2
*port:传感器所在端口号： 1， 2， 3 或 4
*len:帧长度
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQUartSensorConfFrameLen(ViSession vi, int uart, int port, int name, int len);
/*
*查询帧长度
*vi :visa设备句柄
*name :设备名称(SEND_ID)
*uart : 1:UART1; 2:UART2
*port:传感器所在端口号： 1， 2， 3 或 4
*len:帧长度
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQUartSensorConfFrameLen_Query(ViSession vi, int uart, int port, int name, int *ps32Len);
/*
*设置帧个数
*vi :visa设备句柄
*name : 设备名称(SEND_ID)
*uart : 1:UART1; 2:UART2
*port:传感器所在端口号： 1， 2， 3 或 4
*num2:帧个数
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQUartSensorConfRecvNum(ViSession vi, int uart, int port, int name, int frameNum);
/*
*查询帧个数
*vi :visa设备句柄
*name :设备名称(SEND_ID)
*uart : 1:UART1; 2:UART2
*port:传感器所在端口号： 1， 2， 3 或 4
*num2:帧个数
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQUartSensorConfRecvNum_Query(ViSession vi, int uart, int port, int name, int *ps32FrameNum);
/*
*设置周期
*vi :visa设备句柄
*name :设备名称(SEND_ID)
*uart : 1:UART1; 2:UART2
*port:传感器所在端口号： 1， 2， 3 或 4
*period:周期,单位:毫秒
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQUartSensorConfPeriod(ViSession vi, int uart, int port, int name, int period);
/*
*查询周期
*vi :visa设备句柄
*name : 设备名称(SEND_ID)
*uart : 1:UART1; 2:UART2
*port:传感器所在端口号： 1， 2， 3 或 4
*period:周期,单位:毫秒
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQUartSensorConfPeriod_Query(ViSession vi, int uart,int port, int name, int *ps32Period);
/*
*查询传感器数据
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*uart : 1:UART1; 2:UART2
*port:传感器所在端口号： 1， 2， 3 或 4
*buf:数据
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQUartSensorData_Query(ViSession vi, int uart, int port, int name, char *buf);
/*
*查询测距报警的状态
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*state:状态  0:禁止； 1：使能
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQDistanceAlarmState_Query(ViSession vi, int name, int ch, int *ps32State);
/*
*设置测距报警的状态
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*state1:状态
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQDistanceAlarmState(ViSession vi, int name, int ch, int state);
/*
*设置测距报警的响应距离
*alarm:ALARm1,ALARm2,ALARm3
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*distance:测距报警的响应距离
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQDistanceAlarm(ViSession vi, int num, int name, int ch, float distance);
/*
*查询测距报警的响应距离
*vi :visa设备句柄
*alarm:ALARm1,ALARm2,ALARm3
*name:设备名称(SEND_ID)
*ch：通道号
*distance:测距报警的响应距离
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQDistanceAlarm_Query(ViSession vi, int num, int name, int ch, float *pf32Distance);
/*
*查询驱动板类型（只支持10轴）
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*ps32Type:驱动板类型
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQNewDriverType_Query(ViSession vi, int name, int ch, int *ps32Type);
/*
*设置驱动板电流（只支持10轴）
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*current:驱动板电流
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQNewDriverCurrent(ViSession vi, int name, float current);
/*
*查询驱动板电流（只支持10轴）
*vi :visa设备句柄
*name: 设备名称(SEND_ID)
*current:驱动板电流
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQNewDriverCurrent_Query(ViSession vi, int name, float *ps32Current);
/*
*设置电机微步数（只支持10轴）
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*microstep:电机的微步数
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQNewDriverMicrosteps(ViSession vi, int name, int microstep);
/*
*查询电机微步数（只支持10轴）
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*microstep:电机的微步数
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQNewDriverMicrosteps_Query(ViSession vi, int name, int *ps32Microstep);
/*
*查询驱动开关状态（只支持10轴）
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*state:状态  0：禁止； 1：使能
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQNewDriverState_Query(ViSession vi, int name, int ch, int *ps32State);
/*
*设置驱动开关状态（只支持10轴）
*vi :visa设备句柄
*name:设备名称(SEND_ID)
*ch：通道号
*state:状态
*返回值：0表示执行成功，－1表示失败,-2表示参数错误
*/
EXPORT_API int CALL mrgMRQNewDriverState(ViSession vi, int name, int ch, int state);

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif

