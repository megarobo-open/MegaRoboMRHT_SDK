#ifndef  _MEGA_ROBOT_H_
#define  _MEGA_ROBOT_H_

#include "bus.h"

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/*********************** 机器人操作 *******************************/
/*
* 构建一个机器人
* vi :visa设备句柄
* robotType：要构建的机器人类型（MRX-T4,MRX-H2,MRX-DELTA）
* chlist：设备列表 “0@512,1@512,”
* 返回值：0表示执行成功，－1表示失败
*/
EXPORT_API int CALL mrgBuildRobot(ViSession vi, char * robotType, char * chlist, int * robotname);
/*
* 构建一个机器人,机器人的名子由用户指定
* vi :visa设备句柄
* robotType：要构建的机器人类型（MRX-T4,MRX-H2,MRX-DELTA）
* devList：设备列表 “0@512,1@512,”
* robotid: 用户设定的机器人名子
* robotname :返回的机器人名子
* 返回值：0表示执行成功，－1表示失败
*/
EXPORT_API int CALL mrgBuildRobotNamed(ViSession vi, char * robotType, char * devList, int robotid, int * robotname);
/*
* 删除当前机器人
* vi :visa设备句柄
* robotname:机器人名称
* 返回值：0表示执行成功，－1表示执行失败
* 说明：此函数目前只对H2有效
*/
EXPORT_API int CALL mrgDeleteRobot(ViSession vi, int robotname);
/*
* 设置当前机器人的构形的连秆长度  单位:mm
* vi :visa设备句柄
* robotname:机器人名称
* 返回值：小于零表示出错
* 说明：对T4来说: links[0] 基座高度;links[1] 大臂长度 ;links[2] 小臂长度
*  对H2来说: links[0] 宽;links[1] 高 ;links[2] 滑块宽度;links[3] 滑块高度,links[4] 模具类型;links[5] 齿数;
*/
EXPORT_API int CALL mrgSetRobotLinks(ViSession vi, int robotname, float * links, int link_count);
/*
* 获取当前机器人的构形的连秆长度  单位:mm
* vi :visa设备句柄
* robotname:机器人名称
* link_count: 获取到的连秆长度
* 返回值：0成功,否则失败
* 说明：
*/
EXPORT_API int CALL mrgGetRobotLinks(ViSession vi, int robotname, float * links, int *link_count);
/*
* 查询当前机器人的构形
* vi :visa设备句柄
* robotname:机器人名称
* 返回值：小于零表示出错。 0：MRX-T4;1:MRX-AS;2:MRX-H2,3:MRX-DELTA;4:MRX-RAW
* 说明：此函数目前只对H2有效
*/
EXPORT_API int CALL mrgGetRobotType(ViSession vi, int robotname);
/*
* 保存当前系统中所有机器人构形
* vi :visa设备句柄
* 返回值：0表示执行成功，－1表示失败
* 说明：
*/
EXPORT_API int CALL mrgExportRobotConfig(ViSession vi);
/*
* 保存当前系统中所有机器人构形为默认配置文件
* vi :visa设备句柄
* 返回值：0表示执行成功，－1表示失败
* 说明：工程命令，不对外开放
*/
EXPORT_API int CALL mrgExportRobotConfig_default(ViSession vi);
/*
* 恢复上次保存的配置
* vi :visa设备句柄
* 返回值：0表示执行成功，－1表示失败
* 说明：
*/
EXPORT_API int CALL mrgRestoreRobotConfig(ViSession vi);
/*
* 查询导入或导出配置文件的状态
* vi :visa设备句柄
* 返回值：0表示执行完成；1表示正在执行；－1表示执行过程中出错
* 说明：
*/
EXPORT_API int CALL mrgGetRobotConfigState(ViSession vi);
/*
* 设置当前机器人所使用的机械结构的序列号
* vi :visa设备句柄
* robotname:机器人名称
* serial:机械结构序列号
* 返回值：0表示执行成功，－1表示失败
* 说明：此函数目前只对H2有效
*/
EXPORT_API int CALL mrgSetRobotMachineSerialNum(ViSession vi, int robotname, char * sn);
/*
* 查询当前机器人所使用的机械结构的序列号
* vi :visa设备句柄
* robotname:机器人名称
* serial:机械结构序列号
* 返回值：0表示执行正确，否则表示执行失败
* 说明：此函数目前只对H2有效
*/
EXPORT_API int CALL mrgGetRobotMachineSerialNum(ViSession vi, int robotname, char*serial);
/*
* 设置当前机器人构形下的子类型
* vi :visa设备句柄
* robotname:机器人名称
* subtype:子类型。 对于H2来说，0表示小H2，1表示中H2（802x494）；2表示大H2（891x769）
* 返回值：0表示执行成功，－1表示失败
* 说明：此函数目前只对H2有效
*/
EXPORT_API int CALL mrgSetRobotSubType(ViSession vi, int robotname, int subtype);
/*
* 查询当前机器人构形下的子类型
* vi :visa设备句柄
* robotname:机器人名称
* 返回值：子类型。 对于H2来说，0表示小H2，1表示中H2（802x494）；2表示大H2（891x769）
* 说明：此函数目前只对H2有效
*/
EXPORT_API int CALL mrgGetRobotSubType(ViSession vi, int robotname);
/*
* 设置当前机器人的坐标系
* vi :visa设备句柄
* robotname:机器人名称
* coord:坐标系索引（详情参考对应的命令系统）
* 返回值：0表示执行成功，－1表示失败
* 说明：此函数目前只对H2有效
*/
EXPORT_API int CALL mrgSetRobotCoordinateSystem(ViSession vi, int robotname, int coord);
/*
* 查询当前机器人的坐标系
* vi :visa设备句柄
* robotname:机器人名称
* 返回值：坐标系索引，（详情参考对应的命令系统）
* 说明：此函数目前只对H2有效
*/
EXPORT_API int CALL mrgGetRobotCoordinateSystem(ViSession vi, int robotname);
/*
* 查询CAN网络中机器人的个数
* vi :visa设备句柄
* 返回值：返回机器人个数
*/
EXPORT_API int CALL mrgGetRobotCount(ViSession vi);
/*
* 查询CAN网络中所有机器人的名子
* vi :visa设备句柄
* robotnames：输出参数，机器人名称集
* 返回值：大于零,表示返回机器人个数;小于零表示执行出错
* 说明: robotnames是不安全的，请在外部确保robotnames的空间足够
*/
EXPORT_API int CALL mrgGetRobotName(ViSession vi, int *robotnames);
/*
* 查询当前机器人的所使用的设备
* robotname: 机器人名称
* vi :visa设备句柄
* device：设备名称列表
* 返回值：大于零表示返回所使用的设备个数,小于零表示执行出错
* 说明: device是不安全的，请在外部确保device的空间足够
*/
EXPORT_API int CALL mrgGetRobotDevice(ViSession vi, int robotname, int * device);
/*
* 设置当前机器人的项目零点
* vi :visa设备句柄
* robotname:机器人名称
* x,y，z:项目零点值（x,y,z）
* 返回值：0表示执行成功，－1表示失败
* 说明：此函数目前只对H2有效
*/
EXPORT_API int CALL mrgSetRobotProjectZero(ViSession vi, int robotname, float x, float y, float z);
/*
* 查询当前机器人的项目零点
* vi :visa设备句柄
* robotname:机器人名称
* x,y,z: 输出参数，项目零点值
* 返回值：0表示执行成功，－1表示失败,-2表示参数错误
* 说明：此函数目前只对H2有效
*/
EXPORT_API int CALL mrgGetRobotProjectZero(ViSession vi, int robotname, float * x, float *y, float *z);
/*
* 设置当前机器人的校准零点
* vi :visa设备句柄
* robotname:机器人名称
* x,y，z:校准零点值（x,y,z）
* 返回值：0表示执行成功，－1表示失败
* 说明：此函数目前只对H2有效
*/
EXPORT_API int CALL mrgSetRobotAxisZero(ViSession vi, int robotname, float x, float y, float z);
/*
* 查询当前机器人的校准(轴)零点
* vi :visa设备句柄
* robotname:机器人名称
* x,y,z: 输出参数，校准零点值
* 返回值：0表示执行成功，－1表示失败,-2表示参数错误
* 说明：此函数目前只对H2有效
*/
EXPORT_API int CALL mrgGetRobotAxisZero(ViSession vi, int robotname, float * x, float *y, float *z);
/*
* 设置当前机器人的软件限位
* vi :visa设备句柄
* robotname:机器人名称
* type: 正向限位还是负向限位。 0：表示正向限位；1表示负向限位
* x,y，z:限位值（x,y,z）
* 返回值：0表示执行成功，－1表示失败
* 说明：此函数目前只对H2有效
*/
EXPORT_API int CALL mrgSetRobotSoftWareLimit(ViSession vi, int robotname, int type, float x, float y, float z);
/*
* 查询当前机器人的校准零点
* vi :visa设备句柄
* robotname:机器人名称
* type: 正向限位还是负向限位。 0：表示正向限位；1表示负向限位
* x,y,z: 输出参数，校准零点值
* 返回值：0表示执行成功，－1表示失败
* 说明：此函数目前只对H2有效
*/
EXPORT_API int CALL mrgGetRobotSoftWareLimit(ViSession vi, int robotname, int type, float * x, float *y, float *z);
/*
* 设置指定机器人的波表
* vi :visa设备句柄
* robotname: 机器人名称
* wavetable:波表索引值，取值范围0~9
* 返回值：0表示设置成功，否则表示设置失败
*/
EXPORT_API int CALL mrgSetRobotWavetable(ViSession vi, int robotname, int wavetable);
/*
* 查询指定机器人的波表
* vi :visa设备句柄
* robotname: 机器人名称
* wavetable:波表索引值，取值范围0~9
* 返回值：大于0表示波表索引，小于零表示查询失败
*/
EXPORT_API int CALL mrgRobotWavetableQuery(ViSession vi, int robotname);
/*
* 机器人识别
* vi :visa设备句柄
* robotname: 机器人名称
* state:机器人闪灯状态, 1表示闪灯, 0表示不闪灯
* 返回值：成功返回零，否则返回负值
*/
EXPORT_API int CALL mrgSetRobotIdentifyState(ViSession vi, int robotname,int state);
/*
* 启动机器人的运行
* vi :visa设备句柄
* robotname: 机器人名称
* wavetable:波表索引，－1表示使用默认索引（调用mrgSetRobotWavetable设置的波表索引）
* 返回值：0表示启动成功，否则表示启动失败
*/
EXPORT_API int CALL mrgRobotRun(ViSession vi, int robotname, int wavetable);
/*
* 停止机器人的运行
* vi :visa设备句柄
* robotname: 机器人名称
* wavetable:波表索引，－1表示使用默认索引（调用mrgSetRobotWavetable设置的波表索引）
* 返回值：0表示停止成功，否则表示停止失败
*/
EXPORT_API int CALL mrgRobotStop(ViSession vi, int robotname, int wavetable);
/*
* 查询机器人运行状态
* vi :visa设备句柄
* robotname: 机器人名称
* wavetable ：波表索引，－1表示使用默认索引（调用mrgSetRobotWavetable设置的波表索引）
* 返回值：0表示等待成功，－1：表示出错
*/
EXPORT_API int CALL mrgGetRobotStates(ViSession vi, int robotname, int wavetable, char *state);
/*
* 等待机器人的特定波表的ready状态（等待模块设备解算完成）
* vi :visa设备句柄
* robotname: 机器人名称
* wavetable ：波表索引，－1表示使用默认索引（调用mrgSetRobotWavetable设置的波表索引）
* timeout_ms：等待超时时间
* 返回值：0表示等待成功，－1：表示等待过程中出错，－2：表示运行状态出错；－3：表示等待超时
*/
EXPORT_API int CALL mrgRobotWaitReady(ViSession vi, int robotname, int wavetable, int timeout_ms);
/*
* 等待机器人的特定波表的ready状态（等待模块设备解算完成）
* vi :visa设备句柄
* robotname: 机器人名称
* wavetable ：波表索引，－1表示使用默认索引（调用mrgSetRobotWavetable设置的波表索引）
* timeout_ms：等待超时时间
* 返回值：0表示等待成功，－1：表示等待过程中出错，－2：表示运行状态出错；－3：表示等待超时;-4:参数出错
*/
EXPORT_API int CALL mrgRobotWaitEnd(ViSession vi, int robotname, int wavetable, int timeout_ms);
/*
* 机器人从当前位置移动到指定位置（随机移动）
* vi :visa设备句柄
* robotname: 机器人名称
* wavetable ：波表索引，－1表示使用默认索引（调用mrgSetRobotWavetable设置的波表索引）
* x,y,z: 位置坐标
* time : 移动到目标位置期望使用的时间
* timeout_ms:表示等待超时时间,0表示无限等待，－1表示不等待，立即返回
* 返回值：0表示执行成功，－1：表示等待过程中出错，－2：表示运行状态出错；－3：表示等待超时
* 说明：绝对位置移动,末端保持不动
*/
EXPORT_API int CALL mrgRobotMove(ViSession vi, int robotname, int wavetable, float x, float y, float z, float time, int timeout_ms);
/*
* 机器人末端沿指定的方向持续运动
* vi :visa设备句柄
* robotname: 机器人名称
* wavetable ：波表索引，－1表示使用默认索引（调用mrgSetRobotWavetable设置的波表索引）
* x,y,z: 方向向量
* speed : 移动的速度。单位： 度/秒。
* 返回值：0表示执行成功，否则表示过程中出错
* 说明：非阻塞函数
*/
EXPORT_API int CALL mrgRobotMoveOn(ViSession vi, int robotname, int wavetable, float x, float y, float z, float speed);
/*
* 机器人末端沿指定的坐标轴持续运动
* vi :visa设备句柄
* robotname: 机器人名称
* wavetable ：波表索引，－1表示使用默认索引（调用mrgSetRobotWavetable设置的波表索引）
* ax: 坐标轴，0表示X轴;1表示Y轴 ; 2表示Z轴
* speed : 移动的速度。单位： 度/秒。speed的符号决定方向，speed大于零 ，表示正方向。
* 返回值：0表示执行成功，否则表示过程中出错
* 说明：非阻塞函数，此命令只对H2有效
*/
EXPORT_API int CALL mrgRobotAxisMoveOn(ViSession vi, int robotname, int wavetable, int ax, float speed);
/*
* 机器人末端沿指定的坐标轴持续运动(阶跃运行）
* vi :visa设备句柄
* robotname: 机器人名称
* wavetable ：波表索引，－1表示使用默认索引（调用mrgSetRobotWavetable设置的波表索引）
* ax: 坐标轴，0表示X轴;1表示Y轴 ; 2表示Z轴
* cr_time：爬升时间
* cr_speed：爬升的速度
* speed : 移动的保持速度。单位： 度/秒。speed的符号决定方向，speed大于零 ，表示正方向。
* 返回值：0表示执行成功，否则表示过程中出错
* 说明：非阻塞函数,此命令只对H2有效
*/
EXPORT_API int CALL mrgRobotAxisMoveJog(ViSession vi, int robotname, int wavetable, int ax, float cr_time, float cr_speed, float speed);
/*
* 机器人从当前位置移动给定的距离（随机移动）
* vi :visa设备句柄
* robotname: 机器人名称
* wavetable ：波表索引，－1表示使用默认索引（调用mrgSetRobotWavetable设置的波表索引）
* x,y,z: 位置坐标
* time : 移动到目标位置期望使用的时间
* timeout_ms:表示等待超时时间,0表示无限等待，－1表示不等待，立即返回
* 返回值：0表示执行成功，－1：表示等待过程中出错，－2：表示运行状态出错；－3：表示等待超时
* 说明：相对位置移动,末端保持不动
*/
EXPORT_API int CALL mrgRobotRelMove(ViSession vi, int robotname, int wavetable, float x, float y, float z, float time, int timeout_ms);
/*
* 机器人从当前位置移动到指定位置（直线移动）
* vi :visa设备句柄
* robotname: 机器人名称
* wavetable ：波表索引，－1表示使用默认索引（调用mrgSetRobotWavetable设置的波表索引）
* x,y,z: 位置坐标
* time : 移动到目标位置期望使用的时间
* timeout_ms:表示等待超时时间,0表示无限等待，－1表示不等待，立即返回
* 返回值：0表示执行成功，－1：表示等待过程中出错，－2：表示运行状态出错；－3：表示等待超时
* 说明：绝对位置移动,末端保持不动
*/
EXPORT_API int CALL mrgRobotMoveL(ViSession vi, int robotname, int wavetable, float x, float y, float z, float time, int timeout_ms);
/*
* 机器人从当前位置移动给定的距离（直线移动）
* vi :visa设备句柄
* robotname: 机器人名称
* wavetable ：波表索引，－1表示使用默认索引（调用mrgSetRobotWavetable设置的波表索引）
* x,y,z: 位置坐标
* time : 移动到目标位置期望使用的时间
* timeout_ms:表示等待超时时间,0表示无限等待，－1表示不等待，立即返回
* 返回值：0表示执行成功，－1：表示等待过程中出错，－2：表示运行状态出错；－3：表示等待超时
* 说明：相对位置移动,末端保持不动
*/
EXPORT_API int CALL mrgRobotRelMoveL(ViSession vi, int robotname, int wavetable, float x, float y, float z, float time, int timeout_ms);
/*
* 机器人各关节同步运动
* vi :visa设备句柄
* robotname: 机器人名称
* angles: 各轴运动的角度
* count: 轴的个数,也表示angles中有几个有效数据
* time : 移动到目标位置期望使用的时间
* timeout_ms:表示等待超时时间,0表示无限等待，小于零表示不等待，立即返回
* 返回值：0表示执行成功，－1：表示等待过程中出错，－2：表示运行状态出错；－3：表示等待超时
*/
EXPORT_API int CALL mrgRobotMoveJ(ViSession vi, int robotname, float *angles, float time, int count, int timeout_ms);
/*
* 设置机器人当前插值模式
* vi :visa设备句柄
* robotname: 机器人名称
* mode: 插值模式
* 返回值：0表示执行成功，－1：表示出错，
*/
EXPORT_API int CALL mrgSetRobotInterPolateMode(ViSession vi, int robotname, int mode);
/*
* 查询机器人当前插值模式
* vi :visa设备句柄
* robotname: 机器人名称
* mode: 插值模式
* 返回值：0表示执行成功，－1：表示出错，
*/
EXPORT_API int CALL mrgGetRobotInterPolateMode(ViSession vi, int robotname, int* mode);
/*
* 设置机器人当前插值步长
* vi :visa设备句柄
* robotname: 机器人名称
* step: 插值步长
* 返回值：0表示执行成功，－1：表示出错，
*/
EXPORT_API int CALL mrgSetRobotInterPolateStep(ViSession vi, int robotname, float step);
/*
* 查询机器人当前插值步长
* vi :visa设备句柄
* robotname: 机器人名称
* step: 插值步长
* 返回值：0表示执行成功，－1：表示出错，
*/
EXPORT_API int CALL mrgGetRobotInterPolateStep(ViSession vi, int robotname, float* step);
/*
* 设置机器人回零位时使用的波表
* vi :visa设备句柄
* robotname: 机器人名称
* wavetable:波表索引
* 返回值：0表示执行成功，－1：表示出错
*/
EXPORT_API int CALL mrgSetRobotHomeWavetable(ViSession vi, int robotname, int wavetable);
/*
* 查询机器人回零位时使用的波表
* vi :visa设备句柄
* robotname: 机器人名称
* wavetable:返回的波表索引
* 返回值：大于等于0表示查询到的波表索引，小于零：表示出错
*/
EXPORT_API int CALL mrgGetRobotHomeWavetable(ViSession vi, int robotname);
/*
* 机器人回零位操作
* vi :visa设备句柄
* robotname: 机器人名称
* timeout_ms:表示等待超时时间
* 返回值：0表示执行成功，－1：表示等待过程中出错，－2：表示运行状态出错；－3：表示执行超时
* 说明：末端保持不动
*/
EXPORT_API int CALL mrgRobotGoHome(ViSession vi, int robotname, int timeout_ms);
/*
* 机器人回零位操作
* vi :visa设备句柄
* robotname: 机器人名称
* param: 参数，回零位的速度回零位的速度.对于T4来说，单位是mm/s。对于H2来说，度/秒
* timeout_ms:表示等待超时时间,0表示无限等待，小于零表示不等待，立即返回
* 返回值：0表示执行成功，－1：表示等待过程中出错，－2：表示运行状态出错；－3：表示执行超时
* 说明：末端保持不动
*/
EXPORT_API int CALL mrgRobotGoHomeWithParam(ViSession vi, int robotname, float param, int timeout_ms);
/*
* 停止机器人回零位操作
* vi :visa设备句柄
* robotname: 机器人名称
* 返回值：0表示执行成功，-1:表示失败
*/
EXPORT_API int CALL mrgRobotGoHomeStop(ViSession vi, int robotname);
/*
* 等待机器人回零位结束状态（等待运行完成）
* vi :visa设备句柄
* robotname: 机器人名称
* timeout_ms：等待超时时间，为零表示无限等待
* 返回值：0表示等待成功，－1：表示等待过程中出错，－2：表示运行状态出错；－3：表示等待超时
*/
EXPORT_API int CALL mrgRobotWaitHomeEnd(ViSession vi, int robotname, int timeout_ms);
/*
* 获取机器人在原点时的各关节的角度
* vi :visa设备句柄
* robotname: 机器人名称
* 返回值：0表示执行成功，否则表示失败
* 说明：angles是不安全的，请在外部确保angles的空间足够
*/
EXPORT_API int CALL mrgGetRobotHomeAngle(ViSession vi, int robotname, float * angles);
/*
* 获取机器人在零位时，末端的坐标点值
* vi :visa设备句柄
* robotname: 机器人名称
* 返回值：0表示执行成功， －1：表示执行失败
*/
EXPORT_API int CALL mrgGetRobotHomePosition(ViSession vi, int robotname, float * x, float *y, float* z);
/*
* 设置机器人的回零方式
* vi :visa设备句柄
* robotname: 机器人名称
* mode: 回零方式。 0： 先x后y ； 1：先y后x
* 返回值：0表示执行成功，－1：表示出错，
* 说明：此命令目前只对H2有效
*/
EXPORT_API int CALL mrgSetRobotHomeMode(ViSession vi, int robotname, int mode);
/*
* 查询机器人的回零方式
* vi :visa设备句柄
* robotname: 机器人名称
* 返回值：大于等于0表示回零方式，否则表示出错，
* 说明：此命令目前只对H2有效
*/
EXPORT_API int CALL mrgGetRobotHomeMode(ViSession vi, int robotname);
/*
* 查询机器人的是否需要回零
* vi :visa设备句柄
* robotname: 机器人名称
* 返回值：1表示需要回零 ，0表示不需要回零，小于零表示出错，
* 说明：MRHT在构建机器人后,第一件事就是回零.如果不回零,禁止对机器人的操作
*/
EXPORT_API int CALL mrgGetRobotHomeRequire(ViSession vi, int robotname);
/*
* 给指定的机器人加载坐标点
* vi :visa设备句柄
* robotname: 机器人名称
* 返回值：0表示执行成功，否则表示失败
* 说明：在调 用mrgRobotPointLoad（）函数前，请使用此函数清空上次的坐标点
*/
EXPORT_API int CALL mrgRobotPointClear(ViSession vi, int robotname);
/*
* 给指定的机器人加载坐标点
* vi :visa设备句柄
* robotname: 机器人名称
* x,y,z: 坐标点信息
* end: 末端执行器电机转动的角度值
* time: 时间，表示当前点在主时间轴上的位置
* mod :是否插值
* 返回值：0表示执行成功，否则表示失败
* 说明：此函数只是将上位机的坐标点信息下载到MRG中，MRG并未开始解算.
*  另，  在调用此函数开始下发坐标点前，务必使用mrgRobotPointClear()函数，通知机器人清空其缓存中的坐标点。
*/
EXPORT_API int CALL mrgRobotPointLoad(ViSession vi, int robotname, float x, float y, float z, float end, float time, int mod,float step);
/*
* 通知机器人开始解算其缓存中的坐标点，并下发给模块设备，直到模块设备解算完成
* vi :visa设备句柄
* robotname: 机器人名称
* wavetable: 解算到指定的波表。 如果为－1，表示解算到当前机器人默认的波表中
* timeout_ms:表示等待超时时间,0表示无限等待，－1表示不等待，立即返回
* 返回值：0表示执行成功，－1：表示等待过程中出错，－2：表示运行状态出错；－3：表示等待超时
*/
EXPORT_API int CALL mrgRobotPointResolve(ViSession vi, int robotname, int wavetable, int timeout_ms);
/*
* 通知机器人清空PVT缓存
* vi :visa设备句柄
* robotname: 机器人名称
* 返回值：0表示执行成功，否则表示失败
* 说明：在调 用mrgRobotPvtLoad（）函数前，请使用此函数清空上次的PVT
*/
EXPORT_API int CALL mrgRobotPvtClear(ViSession vi, int robotname);
/*
* 给指定的机器人加载坐标点
* vi :visa设备句柄
* robotname: 机器人名称
* p,v,t: pvt信息
* axle: 轴索引
* 返回值：0表示执行成功，否则表示失败
* 说明：此函数只是将上位机的坐标点信息下载到MRG中，MRG并未开始解算.
*  另，  在调用此函数开始下发坐标点前，务必使用mrgRobotPvtClear()函数，通知机器人清空其缓存中的坐标点。
*/
EXPORT_API int CALL mrgRobotPvtLoad(ViSession vi, int robotname, float p, float v, float t, int axle);
/*
* 通知机器人开始下发其缓存中的PVT到模块设备，直到模块设备解算完成
* vi :visa设备句柄
* robotname: 机器人名称
* wavetable: 解算到指定的波表。 如果为－1，表示解算到当前机器人默认的波表中
* timeout_ms:表示等待超时时间,0表示无限等待，－1表示不等待，立即返回
* 返回值：0表示执行成功，－1：表示等待过程中出错，－2：表示运行状态出错；－3：表示等待超时
*/
EXPORT_API int CALL mrgRobotPvtResolve(ViSession vi, int robotname, int wavetable, int timeout_ms);
/*
* 从存储器中，导入运动文件到机器人缓存中
* vi :visa设备句柄
* robotname: 机器人名称
* filename: 点坐标文件名
* 返回值：0表示执行成功，否则表示失败
*/
EXPORT_API int CALL mrgRobotMotionFileImport(ViSession vi, int robotname,char* filename);
/*
* 从本地存储器中，导入运动文件到机器人缓存中
* vi :visa设备句柄
* robotname: 机器人名称
* filename: 点坐标文件名
* 返回值：0表示执行成功，否则表示失败
* 说明 :
*/
EXPORT_API int CALL mrgRobotMotionFileImportLocal(ViSession vi, int robotname, char* filename);
/*
* 从外部存储器中，导入运动文件到机器人缓存中
* vi :visa设备句柄
* robotname: 机器人名称
* filename: 点坐标文件名
* 返回值：0表示执行成功，否则表示失败
* 说明 :
*/
EXPORT_API int CALL mrgRobotMotionFileImportExternal(ViSession vi, int robotname, char* filename);
/*
* 解算当前运动文件内容到模块中
* vi :visa设备句柄
* robotname: 机器人名称
* section:文件中的哪个段，这是个必须的参数. 从零开始计数.
* line：一个段中的哪一行（只针对MFC的文件），line从1开始计数。对于非MFC的文件，不关心line值。line的值为零时,表示本次要解析整个段的数据.
* wavetable : 波表索引。如果不想明确指定波表，可设置 为-1.
* timeout_ms:等等解算完成的超时时间。若timeout_ms＝－1，表示不等待解算完成。timeout_ms ＝ 0，表示无限等待。
* 返回值：0表示执行成功，－1：表示等待过程中出错，－2：表示运行状态出错；－3：表示等待超时
*
*/
EXPORT_API int CALL mrgRobotFileResolve(ViSession vi, int robotname, int section, int line, int wavetable, int timeout_ms);
/*
* 将系统中的运动数据，导出成文件
* vi :visa设备句柄
* robotname: 机器人名称
* location:0表示导出到本地存储（本地文件系统）；1表示导出到外部存储（U盘之类）
* filename：表示导出的文件名
* 返回值：0表示执行正确，否则表示失败。
*/
EXPORT_API int CALL mrgRobotMotionFileExport(ViSession vi, int robotname, int location, char* filename);
/*
* 设置末端执行器类型及相应的设备
* vi :visa设备句柄
* robotname: 机器人名称
* type: 末端执行器类型 0两指爪子;1三指爪子;2 tip头;3 旋转末端
* dev : 末端执行器对应的通道设备 (1@513), 哪个设备的哪个轴
* 返回值：0表示执行成功，－1：表示出错
*/
EXPORT_API int CALL mrgRobotToolSet(ViSession vi, int robotname, int type, char* dev);
/*
* 查询末端执行器类型
* vi :visa设备句柄
* robotname: 机器人名称
* type: 末端执行器类型 0两指爪子;1三指爪子;2 tip头;3 旋转末端
* 返回值：0表示执行成功，－1：表示出错
*/
EXPORT_API int CALL mrgRobotGetToolType(ViSession vi, int robotname, int *ps32Type);
/*
* 等待末端执行器执行完成
* vi :visa设备句柄
* robotname: 机器人名称
* timeout_ms : 末端执行器执行的超时时间，0表示无限等待
* 返回值：0表示执行成功，－1：表示等待过程中出错，－2：表示运行状态出错；－3：表示执行超时
*/
EXPORT_API int CALL mrgRobotWaitToolExeEnd(ViSession vi, int robotname, int timeout_ms);
/*
* 执行末端执行器
* vi :visa设备句柄
* robotname: 机器人名称
* position: 末端执行器电机转动的角度
* time : 末端执行器电机转动的角度所用时间
* timeout_ms : 末端执行器执行的超时时间, 0表示无限等待; -1表示不等待
* 返回值：0表示执行成功，－1：表示等待过程中出错，－2：表示运行状态出错；－3：表示执行超时
*/
EXPORT_API int CALL mrgRobotToolExe(ViSession vi, int robotname, float position, float time, int timeout_ms);
/*
* 停止末端执行器
* vi :visa设备句柄
* robotname: 机器人名称
* 返回值：0表示执行成功，否则表示失败
*/
EXPORT_API int CALL mrgRobotToolStop(ViSession vi, int robotname);
/*
* 中止末端执行器回初始位
* vi :visa设备句柄
* robotname: 机器人名称
* 返回值：0表示执行成功，否则表示失败
*/
EXPORT_API int CALL mrgRobotToolStopGoHome(ViSession vi, int robotname);
/*
* 查询末端执行器执行模式
* vi :visa设备句柄
* robotname: 机器人名称
* mode: 末端执行器执行模式.对于两指 0:正常模式; 1:先张开,再闭合到目标位置; 对于旋转末端,0指0~270范围,1指0~360范围
* 返回值：0表示执行成功，－1：表示出错，
*/
EXPORT_API int CALL mrgRobotToolExeMode(ViSession vi, int robotname, int mode);
/*
* 查询末端执行器执行模式
* vi :visa设备句柄
* robotname: 机器人名称
* mode: 末端执行器执行模式.对于两指 0:正常模式; 1:先张开,再闭合到目标位置; 对于旋转末端,0指0~270范围,1指0~360范围
* 返回值：0表示执行成功，－1：表示出错，
*/
EXPORT_API int CALL mrgRobotToolExeMode_Query(ViSession vi, int robotname, int* mode);
/*
* 末端执行器回初始位
* vi :visa设备句柄
* robotname: 机器人名称
* timeout_ms: 等待的超时时间, 0表示无限等待; -1表示不等待
* 返回值：0表示执行成功，否则表示失败
*/
EXPORT_API int CALL mrgRobotToolGoHome(ViSession vi, int robotname, int timeout_ms);
/*
* 获取机器人末端执行器的位置
* vi :visa设备句柄
* robotname: 机器人名称
* position:末端执行器的位置(相对于机械零点),单位:角度.
* 返回值：0表示执行正确, 否则执行失败
*/
EXPORT_API int CALL mrgGetRobotToolPosition(ViSession vi, int robotname, float * position);
/*
* 获取机器人当前各关节的角度值
* vi :visa设备句柄
* robotname: 机器人名称
* 返回值：大于零 表示返回角度值的个数，小于等于零表示出错
* 注： angles是不安全的，外部一定要保证angles的空间足够
*/
EXPORT_API int CALL mrgGetRobotCurrentAngle(ViSession vi, int robotname, float * angles);
/*
* 获取机器人当前各关节的角度值,相对于零位的角度
* vi :visa设备句柄
* robotname: 机器人名称
* 返回值：大于零 表示返回角度值的个数，小于等于零表示出错
* 说明: 此函数只对T4有效
*     angles是不安全的，请在外部确保angles的空间足够. 对于T4来说,如果处于零位时,各轴角度为 [0,0,0,0]
*/
EXPORT_API int CALL mrgGetRobotCurrentAngleRefZero(ViSession vi, int robotname, float * angles);
/*
* 获取机器人末端的位置坐标
* vi :visa设备句柄
* robotname: 机器人名称
* 返回值：0表示执行成功， －1：表示执行失败
*/
EXPORT_API int CALL mrgGetRobotCurrentPosition(ViSession vi, int robotname, float * x, float *y, float* z);
/*
* 机器人某一个轴回零
* vi :visa设备句柄
* robotname: 机器人名称
* axi :轴索引
* speed: 回零速度,单位:度/秒
* timeout_ms:表示等待回零结束的超时时间. 如果为-1,表示不等待. 0表示无限等待. >0 表示等待的超时时间. 单位:ms
* 返回值：0表示执行成功， －1：表示执行失败
*/
EXPORT_API int CALL mrgRobotJointHome(ViSession vi, int robotname, int axi, float speed, int timeout_ms);
/*
* 控制机器人某一个轴运动
* vi :visa设备句柄
* robotname: 机器人名称
* axi :轴索引
* position: 轴运行的距离,单位: 度
* time : 轴运动时所需要的时间. 单位:秒
* timeout_ms:表示等待回零结束的超时时间. 如果为-1,表示不等待. 0表示无限等待. >0 表示等待的超时时间. 单位:ms
* 返回值：0表示执行成功， －1：表示执行失败
*/
EXPORT_API int CALL mrgRobotJointMove(ViSession vi, int robotname, int axi, float position, float time, int timeout_ms);
/*
* 控制机器人某一个轴持续运动
* vi :visa设备句柄
* robotname: 机器人名称
* axi :轴索引
* speed: 轴运行的速度,单位: 度/秒
* wavetable : 波表
* 返回值：0表示执行成功， －1：表示执行失败
*/
EXPORT_API int CALL mrgRobotJointMoveOn(ViSession vi, int robotname, int axi, float speed);
/*
* 获取机器人各关节的当前角度
* vi :visa设备句柄
* robotname: 机器人名称
* joint:指定机器人的关节索引. 0表示第一个关节(对T4来说,0指的是基座, 对H2来说,0指的是左边的关节). 如果为-1,则表示获取所有的关节
* 返回值：大于零,表示返回的关节角度个数, 否则,出错
* 说明: angle 是不安全的,请确保外部分配足够的空间给angle
*/
EXPORT_API int CALL mrgGetRobotJointAngle(ViSession vi, int robotname, int joint, float *angle);
/*
* 机器人当前的里程数，单位 ：米
* vi :visa设备句柄
* robotname: 机器人名称
* x,y,z ：各坐标轴方向上的里程
* 返回值：0表示执行成功， －1：表示执行失败
*/
EXPORT_API int CALL mrgGetRobotCurrentMileage(ViSession vi, int robotname, float * x, float *y, float* z);
/*
* 获取机器人的目标位置
* vi :visa设备句柄
* robotname: 机器人名称
* x,y,z ：各坐标轴方向上的点
* 返回值：0表示执行成功， －1：表示执行失败
*/
EXPORT_API int CALL mrgGetRobotTargetPosition(ViSession vi, int robotname, float * x, float *y, float* z);
/*
* 获取机器人的当前执行的指令索引
* vi :visa设备句柄
* robotname: 机器人名称
* x,y,z ：各坐标轴方向上的点
* 返回值：0表示执行成功， －1：表示执行失败
* 此命令只对H2有效！！！！！
*/
EXPORT_API int CALL mrgGetRobotCurrentRecord(ViSession vi, int robotname, char *record);
/*
* 机器人当前的错误信息
* vi :visa设备句柄
* name: 机器人名称
* errorCode: 输出错误码
* errorInfo: 输出错误信息
* 返回值：0表示执行成功， －1：表示执行失败
*/
EXPORT_API int CALL mrgGetRobotErrorInfo(ViSession vi, int name, int *errorCode, char *errorInfo);
/*
* 获取机器人腕关节的姿态角度(相对于90度的算法零位)
* vi :visa设备句柄
* robotname: 机器人名称
* angle: 腕关节角度(垂直向下时为零)
* 返回值：零表示执行正确,-1表示执行错误
*/
EXPORT_API int CALL mrgGetRobotWristPose(ViSession vi, int robotname, float *angle);
/*
* 控制机器人腕关节的姿态角度(相对于90度的算法零位)
* vi :visa设备句柄
* robotname: 机器人名称
* wavetable: 波表
* angle: 腕关节角度(垂直向下时为零)
* speed: 速度
* timeout_ms: 表示等待执行的超时时间. 如果为-1,表示不等待. 0表示无限等待. >0 表示等待的超时时间. 单位:ms
* 返回值：零表示执行正确,-1表示执行错误
*/
EXPORT_API int CALL mrgSetRobotWristPose(ViSession vi, int robotname, int wavetable, float angle, float speed, int timeout_ms);
/*
* 机器人的折叠功能(包装位)
* vi :visa设备句柄
* robotname: 机器人名称
* axi0 axi1,axi2,axi3：各轴相对于零点的角度值. axi0:基座; axi1:大臂;axi2:小臂;axi3:腕
* timeout:正数表示等待超时时间, -1表示不等待,0表示无限等待
* 返回值：0表示执行成功， －1：表示执行失败
* 此命令只对T4有效！！！！！
*/
EXPORT_API int CALL mrgSetRobotFold(ViSession vi, int robotname, float axi0, float axi1, float axi2, float axi3, int timeout_ms);
/*
* 获取机器人的折叠状态
* vi :visa设备句柄
* robotname: 机器人名称
* 返回值：1表示折叠完成， 0：还在折叠中; 小于零表示执行出错
* 此命令只对T4有效！！！！！
*/
EXPORT_API int CALL mrgGetRobotFoldState(ViSession vi, int robotname);

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // ! _MEGA_ROBOT_H_


