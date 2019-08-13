#ifndef MR_PROJECT_H
#define MR_PROJECT_H

#include "bus.h"

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

typedef enum
{
    IOGET_ALL,
    IOGET_X1,
    IOGET_X2,
    IOGET_X3,
    IOGET_X4,
    IOGET_X5,
    IOGET_X6,
    IOGET_X7,
    IOGET_X8,
    IOGET_X9,
    IOGET_X10,
    IOGET_Y1,
    IOGET_Y2,
    IOGET_Y3,
    IOGET_Y4,
    IOGET_STOP,
    IOGET_DB15,
    IOGET_MAXNUM,
}IOGET_INDEX;

typedef enum
{
    IOSET_ALL,
    IOSET_Y1,
    IOSET_Y2,
    IOSET_Y3,
    IOSET_Y4,
    IOSET_READY,
    IOSET_FAULT,
    IOSET_ACK,
    IOSET_MC,
    IOSET_ENABLED,
    IOSET_MAXNUM,
}IOSET_INDEX;
typedef enum
{
    IOMRHT29_RDYEN,
    IOMRHT29_DIX0,
    IOMRHT29_DIX1,
    IOMRHT29_DIX2,
    IOMRHT29_DIX3,
    IOMRHT29_DIX4,
    IOMRHT29_DIX5,
    IOMRHT29_START,
    IOMRHT29_ENABLE,
    IOMRHT29_RESET,
    IOMRHT29_ENABLED,
    IOMRHT29_FAULT,
    IOMRHT29_ACK,
    IOMRHT29_MC,
    IOMRHT29_REV1,
    IOMRHT29_REV2,

    IOMRHT29_XIN0,
    IOMRHT29_XIN1,
    IOMRHT29_XIN2,
    IOMRHT29_XIN3,
    IOMRHT29_XIN4,
    IOMRHT29_XIN5,
    IOMRHT29_XIN6,
    IOMRHT29_XIN7,
    IOMRHT29_XIN8,
    IOMRHT29_XIN9,
    IOMRHT29_Y1,
    IOMRHT29_Y2,
    IOMRHT29_Y3,
    IOMRHT29_Y4,
    IOMRHT29_ESTOP,
}IO_MRHT29_INDEX;
/**
 * @brief mrgSetProjectMode
 * 设置系统是否打开工程模式
 * 只有在工程模式下,才允许读取IO状态
 * @param vi
 * 网关句柄
 * @param state
 * 0->OFF| 1->ON
 * @return
 * 0表示执行成功；－1表示执行失败,-2表示参数错误
 */
EXPORT_API int CALL mrgSetProjectMode(ViSession vi, int state);
/**
 * @brief mrgProjectIOGet
 * 查询外部IO的状态
 * @param vi
 * 网关句柄
 * @param index
 * 定义在枚举IOGET_INDEX中
 * @param strState
 * 输出各个IO的状态,使用逗号分隔. "H,H,L,L"
 * @return
 * 0表示执行成功；－1表示执行失败,-2表示参数错误
 */
EXPORT_API int CALL mrgProjectIOGet(ViSession vi, int index, char *strState);
/**
 * @brief mrgProjectIOGetAll
 * 查询外部IO的状态
 * @param vi
 * 网关句柄
 * @param state
 * 输出IO状态, 每一bit表示一个IO的状态
 * @return
 * 0表示执行成功,－1表示执行失败,-2表示参数错误
 */
EXPORT_API int CALL mrgProjectIOGetAll(ViSession vi,int *state);
/**
 * @brief mrgProjectIOSet
 * 设置系统的外部输出IO的状态
 * 当index为零时,表示要同时写出YOUT,此时的state的每一位,表示一个YOUT的状态,mask为相应的屏蔽码.
 * 对于MRHT-29,同步写出,目前只对扩展YOUT有效,暂不支持DB15.
 * @param vi
 * 网关句柄
 * @param index
 * 定义在枚举IOSET_INDEX中
 * @param state
 * 0表示低 | 1表示高
 * @param mask
 * 屏蔽位. 如果某一位为1表示屏蔽此位,0表示不屏蔽
 * @return
 */
EXPORT_API int CALL mrgProjectIOSet(ViSession vi, int index, int state, int mask);
/**
 * @brief mrgProjectSetSerialNum
 * 设置系统的序列号
 * @param vi
 * 网关句柄
 * @param ps8Serial
 * 输入序列号
 * @return
 * 0表示执行成功,－1表示执行失败,-2表示参数错误
 */
EXPORT_API int CALL mrgProjectSetSerialNum(ViSession vi, char * ps8Serial);
/**
 * @brief mrgProjectGetSerialNum
 * 查询系统的序列号
 * @param vi
 * 网关句柄
 * @param ps8Serial
 * 输出序列号
 * @return
 * 返回序列号的长度
 */
EXPORT_API int CALL mrgProjectGetSerialNum(ViSession vi, char * ps8Serial);
/**
 * @brief mrgWriteDeviceSerial
 * 写入设备序列号
 * 说明: 此函数为工程接口函数，不对外开放
 * @param vi
 * 网关句柄
 * @param name
 * 设备ID
 * @param ps8Serial
 * 输入序列号
 * @return
 * 0表示执行成功,－1表示执行失败,-2表示参数错误
 */
EXPORT_API int CALL mrgWriteDeviceSerial(ViSession  vi, int name, char * ps8Serial);

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif
