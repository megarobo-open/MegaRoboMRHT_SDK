#ifndef MR_SYSTEM_H
#define MR_SYSTEM_H
#include "bus.h"

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif
	
/**
 * @brief mrgIdentify
 * 设置识别网关识别状态
 * 当设置为打开常亮后记得恢复到闪烁状态,否则可能会误认为是死机
 * @param vi
 * 网关句柄
 * @param state
 * 识别状态
 * 1:ON表示LED1常亮; 0:OFF时LED1闪烁
 * @return
 * 0表示执行成功,－1表示失败
 */
EXPORT_API int CALL mrgIdentify(ViSession vi, int state);
/**
 * @brief mrgSysGetProjectSoftVersion
 * MRH-T 软件工程版本号
 * @param vi
 * 网关句柄
 * @param version
 * 输出版本号
 * @return
 * 0表示执行成功,－1表示失败
 */
EXPORT_API int CALL mrgSysGetProjectSoftVersion(ViSession vi, char * version);
/**
 * @brief mrgSysGetSoftVersion
 * 查询 MRH-T 软件版本号
 * @param vi
 * 网关句柄
 * @param version
 * 输出版本号
 * @return
 * 0表示执行成功,－1表示失败
 */
EXPORT_API int CALL mrgSysGetSoftVersion(ViSession vi, char * version);
/**
 * @brief mrgSysGetBackBoardSoftVersion
 * 查询 MRH-T 背板的软件版本号
 * @param vi
 * 网关句柄
 * @param version
 * 输出版本号
 * @return
 * 0表示执行成功,－1表示失败
 */
EXPORT_API int CALL mrgSysGetBackBoardSoftVersion(ViSession vi, char * version);
/**
 * @brief mrgSysGetBackBoardHardVersion
 * 查询 MRH-T 背板的硬件版本号
 * @param vi
 * 网关句柄
 * @param version
 * 输出版本号
 * @return
 * 0表示执行成功,－1表示失败
 */
EXPORT_API int CALL mrgSysGetBackBoardHardVersion(ViSession vi, char * version);
/**
 * @brief mrgSysStartCan
 * 启动MARH的CAN口
 * @param vi
 * 网关句柄
 * @return
 * 0表示执行成功,－1表示失败
 */
EXPORT_API int CALL mrgSysStartCan(ViSession vi);
/**
 * @brief mrgSysResetCan
 * 复位MRH的CAN口
 * @param vi
 * 网关句柄
 * @return
 * 0表示执行成功, －1表示失败
 */
EXPORT_API int CALL mrgSysResetCan(ViSession vi);
/**
 * @brief mrgSysSetCanConfig
 * 配置MRH的CAN口波特率
 * @param vi
 * 网关句柄
 * @param baud
 * 输入波特率 1000000,800000,500000,250000,125000,100000
 * @return
 * 0表示执行成功，－1表示失败
 */
EXPORT_API int CALL mrgSysSetCanConfig(ViSession vi, int baud);
/**
 * @brief mrgSysGetCanConfig
 * 获取MRH的CAN口波特率
 * @param vi
 * 网关句柄
 * @param baud
 * 输出波特率
 * @return
 * 0表示执行成功，－1表示失败
 */
EXPORT_API int CALL mrgSysGetCanConfig(ViSession vi, int* baud);
/**
 * @brief mrgSysSetRunMode
 * 切换 MRH-T 的模式
 * @param vi
 * 网关句柄
 * @param mode
 * MRH-T 的模式，取值范围: [0, 1]
 * @return
 * 0表示执行成功，－1表示失败
 */
EXPORT_API int CALL mrgSysSetRunMode(ViSession vi, int mode);
/**
 * @brief mrgSysGetRunMode
 * 查询MRH-T的运行模式
 * @param vi
 * 网关句柄
 * @return
 * 当前运行模式
 */
EXPORT_API int CALL mrgSysGetRunMode(ViSession vi);
/**
 * @brief mrgSysSetInstructionMode
 * 设置指令表的模式
 * @param vi
 * 网关句柄
 * @param mode
 * 指令表执行模式, 取值范围,[0, 1]: 0表示按行执行,1表示按段执行
 * @return
 * 0表示执行成功,-1表示执行失败
 */
EXPORT_API int CALL mrgSysSetInstructionMode(ViSession vi, int mode);
/**
 * @brief mrgSysGetInstructionMode
 * 查询指令表的模式
 * @param vi
 * 网关句柄
 * @param mode
 * 输出指令表执行模式, 取值范围,[0, 1]: 0表示按行执行,1表示按段执行
 * @return
 * 0表示执行成功,-1表示执行失败
 */
EXPORT_API int CALL mrgSysGetInstructionMode(ViSession vi, int* mode);
/**
 * @brief mrgSysWifiScan
 * 扫描可用的WIFI
 * @param vi
 * 网关句柄
 * @param result
 * 输出可用的WIFI名称列表
 * @param len
 * result的大小
 * @return
 * 返回实际接收到的数据长度
 */
EXPORT_API int CALL mrgSysWifiScan(ViSession vi, char* result, int len);
/**
 * @brief mrgSysWifiConfig
 * 配置WIFI
 * @param vi
 * 网关句柄
 * @param name
 * WIFi名称
 * @param passwd
 * 密码
 * @return
 * 返回实际接收到的数据长度
 */
EXPORT_API int CALL mrgSysWifiConfig(ViSession vi, char* name, char* passwd);
/**
 * @brief mrgSysWifiUpDown
 * @param vi
 * 网关句柄
 * @param state
 * 0表示停止, 1表示启用
 * @return
 * 0表示执行成功,否则表示执行失败
 */
EXPORT_API int CALL mrgSysWifiUpDown(ViSession vi, int state);
/**
 * @brief mrgSysWifiCheckState
 * 检查WIFI的连接状态
 * @param vi
 * 网关句柄
 * @param timeout_ms
 * 超时时间，单位ms毫秒
 * @return
 * 0:连接超时或未连接; 1:连接成功
 */
EXPORT_API int CALL mrgSysWifiCheckState(ViSession vi, int timeout_ms);
/**
 * @brief mrgSysGetWifiConnected
 * 查询当前已连接上的WIFI名称
 * @param vi
 * 网关句柄
 * @param name
 * WIFI名
 * @return
 * 0表示执行成功,否则表示执行失败
 */
EXPORT_API int CALL mrgSysGetWifiConnected(ViSession vi, char* name);
/**
 * @brief mrgSysSetHeartPeriod
 * 设置系统的心跳时间
 * @param vi
 * 网关句柄
 * @param value
 * 输入心跳时间,单位:秒
 * @return
 * 0表示执行成功,-1表示执行失败
 */
EXPORT_API int CALL mrgSysSetHeartPeriod(ViSession vi, int value);
/**
 * @brief mrgSysGetHeartPeriod
 * 查询系统的心跳时间
 * @param vi
 * 网关句柄
 * @param value
 * 输出心跳时间,单位:秒
 * @return
 * 0表示执行成功,-1表示执行失败
 */
EXPORT_API int CALL mrgSysGetHeartPeriod(ViSession vi, int * value);
/**
 * @brief mrgSysSetPowerOn
 * 设置系统上电类型
 * @param vi
 * 网关句柄
 * @param value
 * 输入上电类型 0:DEFAULT; 1:LAST
 * @return
 * 0表示执行成功,-1表示执行失败
 */
EXPORT_API int CALL mrgSysSetPowerOn(ViSession vi, int value);
/**
 * @brief mrgSysGetPowerOn
 * 查询系统上电类型
 * @param vi
 * 网关句柄
 * @param value
 * 输出上电类型 0:DEFAULT; 1:LAST
 * @return
 * 0表示执行成功,-1表示执行失败
 */
EXPORT_API int CALL mrgSysGetPowerOn(ViSession vi, int * value);
/**
 * @brief mrgSysSetInterface
 * 设置系统控制接口
 * @param vi
 * 网关句柄
 * @param ifce
 * 输入接口类型
 * Interface_LAN
 * Interface_USB
 * Interface_IO
 * @return
 * 0表示执行成功,-1表示执行失败
 */
EXPORT_API int CALL mrgSysSetInterface(ViSession vi, int ifce);
/**
 * @brief mrgSysGetInterface
 * 查询系统控制接口
 * @param vi
 * 网关句柄
 * @param ifce
 * 输出接口类型
 * Interface_LAN
 * Interface_USB
 * Interface_IO
 * @return
 * 0表示执行成功, -1表示执行失败
 */
EXPORT_API int CALL mrgSysGetInterface(ViSession vi, int * ifce);
/**
 * @brief mrgSysSetDateTime
 * 设置网关当前系统时间
 * @param vi
 * 网关句柄
 * @param ps8Date
 * 要设置的时间: "2019-08-08 14:25:30"
 * @return
 * 0表示执行成功,-1表示执行失败
 */
EXPORT_API int CALL mrgSysSetDateTime(ViSession vi, char *ps8Date);
/**
 * @brief mrgSysGetDateTime
 * 获取系统当前时间
 * @param vi
 * 网关句柄
 * @param ps8Date
 * 输出获取到的时间
 * @return
 * 0表示执行成功,-1表示执行失败
 */
EXPORT_API int CALL mrgSysGetDateTime(ViSession vi, char *ps8Date);
/**
 * @brief mrgSysSetEmergencyStopState
 * 设置紧急停止
 * 说明: 紧急停止时,会停止当前系统CAN网络中的所有设备,并设置紧急停止标志位
 * 在紧急标志位被清除前,不接受任何CAN帧
 * 当state为OFF时,只是清除标志位
 * @param vi
 * 网关句柄
 * @param state
 * 0->OFF | 1->ON
 * @return
 * 0表示执行成功；－1表示执行失败
 */
EXPORT_API int CALL mrgSysSetEmergencyStopState(ViSession vi, int state);
/**
 * @brief mrgSysGetEmergencyStopState
 * 获取急停状态,主要用来清除标志位
 * 说明: 紧急停止按钮被按下时,会自动改变此状态
 * @param vi
 * 网关句柄
 * @param state
 * 输出状态: 0->OFF | 1->ON
 * @return
 * 0表示执行成功,－1表示执行失败
 */
EXPORT_API int CALL mrgSysGetEmergencyStopState(ViSession vi, int *state);
/**
 * @brief mrgSysUpdateFileSearch
 * 搜索并获取升级固件列表
 * @param vi
 * 网关句柄
 * @param fileList
 * 输出搜索到的升级固件列表
 * @return
 * 成功返回0,失败返回-1,返回1表示没有U盘
 */
EXPORT_API int CALL mrgSysUpdateFileSearch(ViSession vi, char *fileList);
/**
 * @brief mrgSysUpdateFileStart
 * 开始更新固件
 * @param vi
 * 网关句柄
 * @param filename
 * 输入要更新的固件名
 * @return
 * 失败返负数,成功返0,超时返1
 */
EXPORT_API int CALL mrgSysUpdateFileStart(ViSession vi, char *filename);
/**
 * @brief mrgSysUpdateFirmwareStatusQuery
 * 查询当前升级状态
 * @param vi
 * 网关句柄
 * @return
 * 成功返回0，失败返回负数，正在升级返回1
 */
EXPORT_API int CALL mrgSysUpdateFirmwareStatusQuery(ViSession vi);
/**
 * @brief mrgScriptSearch
 * 获取MRHT上的py脚本列表
 * @param vi
 * 网关句柄
 * @param isUdisk
 * 1为U盘,0为MRH-T内部
 * @param fileList
 * 输出文件列表
 * @return
 * 成功返回0,-1表示失败,返回-2表示没有文件，返回-3表示没有U盘
 */
EXPORT_API int CALL mrgScriptSearch(ViSession vi, int isUdisk, char *fileList);
/**
 * @brief mrgScriptSetConfig
 * 配置脚本及设置是否开机自启动
 * 如果配置的文件名为U盘中的文件名,则会将U盘中的文件导入到MRHT内部并配置为当前脚本
 * @param vi
 * 网关句柄
 * @param filename
 * 要配置的文件名
 * @param isBoot
 * 设置为开机启动,0为不启动,1为启动
 * @return
 * 成功返回0, 失败返回-1
 */
EXPORT_API int CALL mrgScriptSetConfig(ViSession vi, char *filename, int isBoot);
/**
 * @brief mrgScriptGetConfig
 * 查询当前配置的脚本
 * @param vi
 * 网关句柄
 * @param filename
 * 输出查询到的配置脚本名字
 * @return
 * 成功返回0, 失败返回-1
 */
EXPORT_API int CALL mrgScriptGetConfig(ViSession vi, char *filename);
/**
 * @brief mrgScriptRun
 * 运行当前配置的脚本
 * @param vi
 * 网关句柄
 * @return
 * 成功返回0, 失败返回-1
 */
EXPORT_API int CALL mrgScriptRun(ViSession vi);
/**
 * @brief mrgScriptStop
 * 停止运行当前配置的脚本
 * @param vi
 * 网关句柄
 * @return
 * 成功返回0, 失败返回-1
 */
EXPORT_API int CALL mrgScriptStop(ViSession vi);
/**
 * @brief mrgScriptGetCurrentStates
 * 获取脚本的当前运行状态
 * @param vi
 * 网关句柄
 * @return
 * 失败返回-1，运行返回1，停止返回0
 */
EXPORT_API int CALL mrgScriptGetCurrentStates(ViSession vi);
/**
 * @brief mrgSystemRunCmd
 * 执行Shell命令
 * @param vi
 * 网关句柄
 * @param cmd
 * 输入要执行的shell命令
 * @param isBackground
 * 是否后台执行
 * 0表示阻塞执行时间短命令,如创建文件修改权限等
 * 1表示将命令放到后台执行,如重启网卡,启动MCT等
 * @return
 * 0表示成功,-1表示失败
 */
EXPORT_API int CALL mrgSystemRunCmd(ViSession vi, char *cmd, int isBackground);
/**
 * @brief mrgSystemSetMRQConfig
 * 设置系统中配置的MRQ信息
 * 只有当MRH-T连接一个MRQ时，系统才会检查MRQ信息
 * @param vi
 * 网关句柄
 * @param version
 * 输入版本信息
 * @param sn
 * 输入序列号信息
 * @return
 * 0表示成功,-1表示失败
 */
EXPORT_API int CALL mrgSystemSetMRQConfig(ViSession vi, const char *version, const char *sn);
/**
 * @brief mrgSystemGetMRQConfig
 * 获取系统中配置的MRQ信息
 * @param vi
 * 网关句柄
 * @param version
 * 输出版本信息
 * @param sn
 * 输出序列号信息
 * @return
 * 0表示成功,-1表示失败
 */
EXPORT_API int CALL mrgSystemGetMRQConfig(ViSession vi, char *version, char *sn);
/**
 * @brief mrgSysErrorSetAlertConf
 * 设置错误发生后报警灯的配置状态
 * @param vi
 * 网关句柄
 * @param errorCode
 * 错误码
 * @param isAlert
 * 设置报警灯是否闪烁,1表示闪烁,0表示不闪烁
 * @return
 * 成功返回0,失败返回-1
 */
EXPORT_API int CALL mrgSysErrorSetAlertConf(ViSession vi, unsigned int errorCode, int isAlert);
/**
 * @brief mrgSysErrorGetAlertConf
 * 查询错误发生后报警灯的配置状态
 * @param vi
 * 网关句柄
 * @param errorCode
 * 错误码
 * @return
 * 返回0表示不关闭输出级,返回1表示关闭输出级,失败返回负数
 */
EXPORT_API int CALL mrgSysErrorGetAlertConf(ViSession vi, unsigned int errorCode);
/**
 * @brief mrgSysErrorSetOutputConf
 * 设置错误输出级配置
 * @param vi
 * 网关句柄
 * @param errorCode
 * 错误码
 * @param isCloseOutput
 * 是否关闭输出级,1表示关闭,0表示不关闭
 * @return
 * 成功返回0,失败返回-1
 */
EXPORT_API int CALL mrgSysErrorSetOutputConf(ViSession vi, unsigned int errorCode, int isCloseOutput);
/**
 * @brief mrgSysErrorGetOutputConf
 * 查询错误输出级配置
 * @param vi
 * 网关句柄
 * @param errorCode
 * 错误码
 * @return
 * 返回0表示不关闭输出级,返回1表示关闭输出级,失败返回负数
 */
EXPORT_API int CALL mrgSysErrorGetOutputConf(ViSession vi, unsigned int errorCode);
/**
 * @brief mrgSystemErrorAck
 * 系统错误确认,在用户处理完错误后执行此操作
 * @param vi
 * 网关句柄
 * @return
 * 成功返回0,失败返回-1
 */
EXPORT_API int CALL mrgSystemErrorAck(ViSession vi);


#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !MR_SYSTEM_H

