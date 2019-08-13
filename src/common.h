#ifndef COMMON_H
#define COMMON_H

char *__GetFileName__(const char *file);

void be_printf(char *fmt, ...);

void print_buffer(char* msg, unsigned char* pBuff, unsigned long ulLen);

#define MRG_LOG(fmt, ...) \
    do{  \
    char tmp[1024] = {0}; \
    snprintf(tmp,sizeof(tmp),"%s(%d):[%s] %s",__GetFileName__(__FILE__),__LINE__,__FUNCTION__, fmt); \
    be_printf(tmp, ##__VA_ARGS__ );  \
    }while(0)

/**
 * @brief strToIntList
 * 拆分字符串到数组
 * @param str 输入需要拆分的字符串
 * @param split 分隔符
 * @param output 输出结果
 * @return 数量
 */
int splitStringToIntArray(const char *str, const char *split, int *output);
/**
 * @brief splitStringToFloatArray
 * 拆分字符串到数组
 * @param str 输入需要拆分的字符串
 * @param split 分隔符
 * @param output 输出结果
 * @return 数量
 */
int splitStringToFloatArray(const char *str, const char *split, float *output);
/**
 * @brief splitString
 * 拆分字符串到数组
 * @param str 输入需要拆分的字符串
 * @param split 分隔符
 * @param output 输出结果
 * @return 数量
 */
int splitString(const char *str, const char *split, char output[][64]);

/**
 * @brief scp_send
 * 使用ssh将本地文件拷贝到远程主机
 * @param ip
 * 远程主机IP
 * @param localfile
 * 本地要发送的文件名,绝对路径
 * @param remotefile
 * 远程主机保存的文件名,绝对路径
 * @return
 * 0表示成功,负数表示失败
 */
int scp_send(const char *ip,const char *localfile,const char *remotefile);



int scp_recv(const char *ip,const char *remotefile,const char *localfile);






unsigned short makeCRC16(unsigned char *pu8Buf, unsigned int u32Len);

#endif // COMMON_H
