#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "bus.h"

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/**
 * @brief mrgFindGateWay
 * 查找网关
 * @param mode
 * 通讯模式
 * BUS_VXI
 * BUS_SOCKET
 * BUS_USB
 * @param output
 * 查找到的设备描述符,以逗号分隔
 * @param len
 * 提供的空间容量
 * @return
 * 查找到的设备数量,零或负数表示失败
 */
EXPORT_API int CALL mrgFindGateWay(int mode, char *output, int len);
/**
 * @brief mrgOpenGateWay
 * 打开网关
 * @param mode
 * 通讯模式
 * BUS_VXI
 * BUS_SOCKET
 * BUS_USB
 * @param desc
 * 设备描述符
 * @param timeout_ms
 * 设备通讯时的超时时间
 * @return
 * 成功返回网关句柄,零或负数表示失败
 */
EXPORT_API int CALL mrgOpenGateWay(int mode, char * desc, int timeout_ms);
/**
 * @brief mrgCloseGateWay
 * 关闭网关
 * @param vi
 * 网关句柄
 * @return
 * 0表示执行成功,否则失败
 */
EXPORT_API int CALL mrgCloseGateWay(ViSession  vi);
/**
 * @brief mrgGateWaySendCmd
 * 发送命令给网关
 * @param vi
 * 网关句柄
 * @param cmd
 * SCPI指令信息
 * @param len
 * 指令长度
 * @return
 * >0表示成功发送的字节数,否则失败
 */
EXPORT_API int CALL mrgGateWaySendCmd(ViSession  vi, char* cmd, int len);
/**
 * @brief mrgGateWayRead
 * 网关从接收数据
 * @param vi
 * 网关句柄
 * @param output
 * 接收数据的存放地址
 * @param wantlen
 * 接收数据长度
 * @return
 * >0表示成功接收的字节数,否则失败
 */
EXPORT_API int CALL mrgGateWayRead(ViSession  vi, char * output, int wantlen);
/**
 * @brief mrgGateWayQuery
 * 发送查询命令并获取查询结果
 * @param vi
 * 网关句柄
 * @param cmd
 * 要发送的设置命令
 * @param output
 * 接收数据的存放地址
 * @param wantlen
 * 接收数据长度
 * @return
 * >0表示成功接收的字节数,否则失败
 */
EXPORT_API int CALL mrgGateWayQuery(ViSession  vi, char* cmd, char *output, int wantlen);
/**
 * @brief mrgGateWayIDNQuery
 * 获取网关识别信息
 * @param vi
 * 网关句柄
 * @param idn
 * 输出信息
 * @return
 * 0表示成功,负数表示失败
 */
EXPORT_API int CALL mrgGateWayIDNQuery(ViSession  vi, char *idn);
/**
 * @brief mrgFindDevice
 * 查找连接在网关上的设备
 * @param vi
 * 网关句柄
 * @param timeout_ms
 * 查找超时时间
 * @return
 * 成功返回查找到的设备个数,零或负数表示失败
 */
EXPORT_API int CALL mrgFindDevice(ViSession vi, int timeout_ms);
/**
 * @brief mrgGetDeviceName
 * 获取网关上连接的所有设备名称
 * @param vi
 * 网关句柄
 * @param names
 * 输出设备ID
 * @return
 * 大于零表示设备名称个数, -1表示获取失败, -2表示参数错误
 */
EXPORT_API int CALL mrgGetDeviceName(ViSession vi, int* names);
/**
 * @brief mrgGetDeviceType
 * 获取设备的类型
 * @param vi
 * 网关句柄
 * @param name
 * 设备ID
 * @param ps8Type
 * 输出类型
 * @return
 * 0表示执行成功,－1表示失败,-2表示参数错误
 */
EXPORT_API int CALL mrgGetDeviceType(ViSession vi, int name, char * ps8Type);
/**
 * @brief mrgGetDeviceChannelCount
 * 查询设备的通道个数
 * @param vi
 * 网关句柄
 * @param name
 * 设备ID
 * @return
 * 大于零表示通道个数,小于或等于零表示失败
 */
EXPORT_API int CALL mrgGetDeviceChannelCount(ViSession vi, int name);
/**
 * @brief mrgGetDeviceInfo
 * 获取设备的信息
 * @param vi
 * 网关句柄
 * @param name
 * 设备ID
 * @param ps8Info
 * 输出信息
 * @return
 * 0表示执行成功,－1表示失败,-2表示参数错误
 */
EXPORT_API int CALL mrgGetDeviceInfo(ViSession vi, int name, char * ps8Info);
/**
 * @brief mrgGetDeviceSoftVersion
 * 获取设备的软件版本号
 * @param vi
 * 网关句柄
 * @param name
 * 设备ID
 * @param version
 * 输出信息
 * @return
 * 0表示执行成功,－1表示失败,-2表示参数错误
 */
EXPORT_API int CALL mrgGetDeviceSoftVersion(ViSession vi, int name, char * version);
/**
 * @brief mrgGetDeviceFirmWareHard
 * 查询设备硬件版本号
 * @param vi
 * 网关句柄
 * @param name
 * 设备ID
 * @param buf
 * 输出信息
 * @return
 * 0表示执行成功,－1表示失败,-2表示参数错误
 */
EXPORT_API int CALL mrgGetDeviceFirmWareHard(ViSession vi, int name, char *buf);
/**
 * @brief mrgGetDeviceFirmWareBoot
 * 查询设备Boot版本号
 * @param vi
 * 网关句柄
 * @param name
 * 设备ID
 * @param buf
 * 输出信息
 * @return
 * 0表示执行成功,－1表示失败,-2表示参数错误
 */
EXPORT_API int CALL mrgGetDeviceFirmWareBoot(ViSession vi, int name, char *buf);
/**
 * @brief mrgGetDeviceFirmWareFpga
 * 查询设备FPGA版本号
 * @param vi
 * 网关句柄
 * @param name
 * 设备ID
 * @param buf
 * 输出信息
 * @return
 * 0表示执行成功,－1表示失败,-2表示参数错误
 */
EXPORT_API int CALL mrgGetDeviceFirmWareFpga(ViSession vi, int name, char *buf);
/**
 * @brief mrgGetDeviceSerialNumber
 * 查询设备序列号
 * @param vi
 * 网关句柄
 * @param name
 * 设备ID
 * @param ps8Serial
 * 输出信息
 * @return
 * 0表示执行成功,－1表示失败,-2表示参数错误
 */
EXPORT_API int CALL mrgGetDeviceSerialNumber(ViSession vi, int name, char * ps8Serial);



#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !_DEVICE_H_

