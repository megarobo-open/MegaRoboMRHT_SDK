#ifndef _MEGA_STORAGE_H
#define _MEGA_STORAGE_H

#include "bus.h"

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif

/**
 * @brief mrgStorageMotionFileQuery
 * 查询存储器中的运动文件
 * @param vi
 * 网关句柄
 * @param type
 * 0表示本机, 1表示U盘
 * @param ps8FileList
 * 输出文件列表
 * @param len
 * ps8FileList的容量
 * @return
 * 大于零表示返回实际的字节数, 小于零表示出错
 */
EXPORT_API int CALL mrgStorageMotionFileQuery(ViSession vi, int type, char* ps8FileList, int len);
/**
 * @brief mrgStorageMotionFileDelete
 * 删除本地存储器中的运动文件
 * @param vi
 * 网关句柄
 * @param fileName
 * 输入文件名
 * @return
 * 大于零表示返回实际的字节数, 小于零表示出错
 */
EXPORT_API int CALL mrgStorageMotionFileDelete(ViSession vi, char* fileName);
/**
 * @brief mrgStorageMotionFileContextRead
 * 读取运动文件内容到上位机
 * 在此操作之前可先获取文件大小,确定需要提供的内存容量
 * @param vi
 * 网关句柄
 * @param ps8FileName
 * 输入文件名
 * @param ps8Context
 * 输出读取到的内容, 确保空间足够
 * @return
 * 返回实际的文件长度
 */
EXPORT_API int CALL  mrgStorageMotionFileContextRead(ViSession vi, char* ps8FileName, char* ps8Context);
/**
 * @brief mrgStorageMotionFileSaveContext
 * 写运动文件内容到网关上
 * @param vi
 * 网关句柄
 * @param ps8Context
 * 写的文件内容
 * @param len
 * 文件内容长度
 * @param ps8SaveFileName
 * 在网关上保存的文件名
 * @return
 * 0表示成功, 否则写入失败
 */
EXPORT_API int CALL mrgStorageMotionFileSaveContext(ViSession vi, char* ps8Context, int len, char * ps8SaveFileName);
/**
 * @brief mrgStorageWriteFile
 * 写文件到网关
 * @param vi
 * 网关句柄
 * @param isUdisk
 * 是否为U盘,0表示网关内,1表示U盘
 * @param ps8Path
 * 保存目录的绝对路径 如"/home/test"
 * @param ps8SaveFileName
 * 保存的文件名, 如"123.txt"
 * @param pu8Data
 * 要写的内容
 * @param dataLen
 * 要写的内容长度
 * @return
 * 0表示成功, 负数表示失败
 */
EXPORT_API int CALL mrgStorageWriteFile(ViSession vi, int isUdisk, char *ps8Path, char *ps8SaveFileName, unsigned char *pu8Data, int dataLen);
/**
 * @brief mrgStorageReadFile
 * 读取文件到上位机
 * 在此操作之前可先获取文件大小,确定需要提供的内存容量
 * @param vi
 * 网关句柄
 * @param isUdisk
 * 是否为U盘,0表示网关内,1表示U盘
 * @param ps8Path
 * 所在目录的绝对路径 如"/home/test"
 * @param ps8Filename
 * 文件名
 * @param ps8Context
 * 读取后保存数据的空间, 确保足够
 * @return
 * 返回实际的文件长度
 */
EXPORT_API int CALL mrgStorageReadFile(ViSession vi, int isUdisk, char *ps8Path, char* ps8Filename, unsigned char* ps8Context);
/**
 * @brief mrgStorageDirectoryEnum
 * 获取目录文件列表,可用于判断文件是否存在
 * @param vi
 * 网关句柄
 * @param isUdisk
 * 是否为U盘,0表示网关内,1表示U盘
 * @param ps8Path
 * 所在目录的绝对路径 如"/home/test"
 * @param ps8FileList
 * 输出文件列表
 * @param len
 * 输入ps8FileList的长度,输出实际的长度
 * @return
 * 返回实际的文件长度
 */
EXPORT_API int CALL mrgStorageDirectoryEnum(ViSession vi, int isUdisk, const char *ps8Path, char* ps8FileList, int *len);
/**
 * @brief mrgStorageGetFileSize
 * 获取文件大小
 * @param vi
 * 网关句柄
 * @param isUdisk
 * 是否为U盘,0表示网关内,1表示U盘
 * @param ps8Path
 * 所在目录的绝对路径 如"/home/test"
 * @param ps8Filename
 * 文件名
 * @return
 * -1表示失败,成功返回文件大小
 */
EXPORT_API int CALL mrgStorageGetFileSize(ViSession vi, int isUdisk, const char *ps8Path, char* ps8Filename);


EXPORT_API int CALL mrgStorageWriteBigFile(ViSession vi, char *ps8Path, char *ps8SaveFileName,unsigned char *pu8Data, int dataLen);

EXPORT_API int CALL mrgStorageReadBigFile(ViSession vi, char *ps8Path, char* ps8Filename, unsigned char* ps8Context, int contextLen);


#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif

#endif // !MR_SYSTEM_H
