#ifndef _BUS_H_
#define _BUS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "platform.h"

/**
 * @brief busFindDevice
 * 查找设备
 * @param mode
 * 查找方式: BUS_VXI,BUS_SOCKET,BUS_USB
 * @param output
 * 输出查找到的设备描述列表
 * @param len
 * 输出空间的大小
 * @return
 * 查找到的设备数量,零或负数表示失败
 */
int busFindDevice(int mode, char *output, size_t len);
/**
 * @brief busOpenDevice
 * 打开设备
 * @param mode
 * 通信方式: BUS_VXI,BUS_SOCKET,BUS_USB
 * @param devDesc
 * 设备描述
 * @param timeout_ms
 * 通信超时时间,单位毫秒
 * @return
 * 返回设备句柄,零或负数表示失败
 */
int busOpenDevice(int mode, char *devDesc, size_t timeout_ms);
/**
 * @brief busWrite
 * 写数据
 * @param fd
 * 设备句柄
 * @param buf
 * 要写的数据
 * @param len
 * 要写的数据长度
 * @return
 * 实际写入的长度,零或负数表示失败
 */
size_t busWrite(unsigned long fd, char *buf, size_t len);
/**
 * @brief busRead
 * 接收数据
 * @param fd
 * 设备句柄
 * @param buf
 * 接收数据的空间
 * @param len
 * 接收数据的空间长度
 * @return
 * 实际接收的大小,零或负数表示失败
 */
size_t busRead(unsigned long fd, char *buf, size_t len);
/**
 * @brief busQuery
 * 指令查询,相当于捆绑的发送和接收
 * @param fd
 * 设备句柄
 * @param input
 * 发送的命令
 * @param inputlen
 * 发送命令的长度
 * @param output
 * 接收的信息
 * @param wantlen
 * 需要接收的长度
 * @return
 * 返回接收到的长度,零或负数表示失败
 */
size_t busQuery(unsigned long fd, char *input, size_t inputlen, char* output, size_t wantlen);
/**
 * @brief busCloseDevice
 * 关闭设备
 * @param fd
 * 设备句柄
 * @return
 * 0表示成功,否则失败
 */
int busCloseDevice(unsigned long fd);

#ifdef __cplusplus
}
#endif

#endif // !_BUS_H_

