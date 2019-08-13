# MRH-T开发者工具包使用说明
* Version: 0.0.0.1
* MEGAROBO 
* www.megarobo.tech
* MRH-T开发包用于控制所连接的MRQ驱控器
* 通过驱控器带动电机，使构型和进行期望的运动
* 控制分成以下层次
  * 构型：利用电机轴组成物理构型，如XYZ,XY型运动
  * 电机轴：电机的旋转运动


# 编译生成库
  
## 构建win32库
   在Windows平台使用下面方式进行操作
   
### 使用Visual Studio 2010
* 使用VS打开src/MegaGateway.sln
* 在解决方案窗口右键MegaGateway,选择生成
* 如果下面输出面板提示成功,就会在lib/windows下生成MegaGateway.dll和MegaGateway.lib
* 其中*.dll为动态库，*.lib为静态库
    

### 使用Qt Creator
* 使用QtCreator打开工程,选择src/MegaGateway.pro并打开
* 如果QtCreator有多个版本的Qt, 则选择勾选MingW32版本,其他的取消勾选,并点击Configure Project按钮
* 在QtCreator界面左下角选择Debug版本或者Release版本,然后点击锤子按钮进行编译代码(Ctrl-B)
* 如果编译没有错误,就点击锤子上面的运行按钮(Ctrl-R),关闭弹出的对话框
* 此时已经在lib/windows/下产生了libMegaGateway-1.a和MegaGateway-1.dll文件
* 其中*.dll为动态库，*.a为静态库, -1表示库的主版本
* 使用动态库运行时,依赖lib下的libgcc_s_dw2-1.dll,libstdc++-6.dll,libwinpthread-1.dll
* 使用静态库编译时依赖链接libiphlpapi.a,libmsvcr100.a,libws2_32.a,visa32.lib
    

### 使用MingW终端
* 打开MinGW的msys终端环境(即：双击C:\MinGW\msys\1.0\msys.bat)
* $ cd src/
* $ make clean
* $ make
* 如果没有产生错误
* $ make install
* 此时已经在lib/windows/下产生了libMegaGateway-1.a和MegaGateway-1.dll文件
* 其中*.dll为动态库，*.a为静态库, -1表示库的主版本
* 使用动态库运行时,依赖lib下的libgcc_s_dw2-1.dll,libstdc++-6.dll,libwinpthread-1.dll
* 使用静态库编译时依赖链接libiphlpapi.a,libmsvcr100.a,libws2_32.a,visa32.lib
    
    
## 构建Linux库
* 在Linux平台使用下面方式进行操作
   
### 使用Qt Creator
* 使用QtCreator打开工程,选择src/MegaGateway.pro并打开
* 如果QtCreator有多个版本的Qt, 则选择勾选Desktop Linux版本,其他的取消勾选,并点击Configure Project按钮
* 在QtCreator界面左下角选择Debug版本或者Release版本,然后点击锤子按钮进行编译代码(Ctrl-B)
* 如果编译没有错误,就点击锤子上面的运行按钮(Ctrl-R),关闭弹出的对话框
* 此时已经在lib/linux/下产生了libMegaGateway.so文件
* 修改MegaGateway.pro第8行: "#CONFIG += staticlib" 去掉 CONFIG前的#号
* 点击锤子按钮进行编译代码(Ctrl-B)
* 如果编译没有错误,就点击锤子上面的运行按钮(Ctrl-R),关闭弹出的对话框
* 此时已经在lib/linux/下产生了libMegaGateway.a
* 其中*.so为动态库，*.a为静态库
    

### 终端下编译
* $ cd src/
* $ make clean
* $ make
* 如果没有产生错误
* $ make install
* 此时已经在lib/linux/下产生了libMegaGateway.a和libMegaGateway.so文件
* 其中*.so为动态库，*.a为静态库
    
    
## 嵌入式Linux平台库
* 在Linux平台使用下面方式进行操作
   
### 使用Qt Creator
* 使用QtCreator打开工程,选择src/MegaGateway.pro并打开
* 如果QtCreator有多个版本的Qt, 则选择勾选配置好的嵌入式Linux版本,其他的取消勾选,并点击Configure Project按钮
* 修改MegaGateway.pro第10行: "#DEFINES += ARM_LINUX" 去掉 DEFINES前的#号
* 在QtCreator界面左下角选择Debug版本或者Release版本,然后点击锤子按钮进行编译代码(Ctrl-B)
* 如果编译没有错误,就点击锤子上面的运行按钮(Ctrl-R),关闭弹出的对话框
* 此时已经在lib/armlinux/下产生了libMegaGateway.so文件
* 修改MegaGateway.pro第8行: "#CONFIG += staticlib" 去掉 CONFIG前的#号
* 点击锤子按钮进行编译代码(Ctrl-B)
* 如果编译没有错误,就点击锤子上面的运行按钮(Ctrl-R),关闭弹出的对话框
* 此时已经在lib/armlinux/下产生了libMegaGateway.a
* 其中*.so为动态库，*.a为静态库
    

### 终端下编译
* $ cd src/
* 修改Makefile文件开头CROSS_COMPILE等号右侧的交叉编译器路径
* 并去掉CROSS_COMPILE前的#号
* $ make clean
* $ make
* 如果没有产生错误
* $ make install
* 此时已经在lib/armlinux/下产生了libMegaGateway.a和libMegaGateway.so文件
* 其中*.so为动态库，*.a为静态库
    
    
## 构建android库
* 在Linux平台使用下面方式进行操作
   
### 使用Qt Creator
* 使用QtCreator打开工程,选择src/MegaGateway.pro并打开
* 如果QtCreator有多个版本的Qt, 则选择勾选配置好的嵌入式Linux版本,其他的取消勾选,并点击Configure Project按钮
* 修改MegaGateway.pro第11行: "#DEFINES += ANDROID" 去掉 DEFINES前的#号
* 在QtCreator界面左下角选择Debug版本或者Release版本,然后点击锤子按钮进行编译代码(Ctrl-B)
* 如果编译没有错误,就点击锤子上面的运行按钮(Ctrl-R),关闭弹出的对话框
* 此时已经在lib/android/下产生了libMegaGateway.so文件
* 修改MegaGateway.pro第8行: "#CONFIG += staticlib" 去掉 CONFIG前的#号
* 点击锤子按钮进行编译代码(Ctrl-B)
* 如果编译没有错误,就点击锤子上面的运行按钮(Ctrl-R),关闭弹出的对话框
* 此时已经在lib/android/下产生了libMegaGateway.a
* 其中*.so为动态库，*.a为静态库
* android平台一般使用java的jni使用C语言动态库,一般不会用到静态库
    

# 运行示例
## C语言示例
* 打开example/C_C++目录,
* 可在windows或Linux等平台,使用QtCreator打开.pro文件编译运行或者调试示例程序
* 在windows上也可以使用VS2010等开发工具打开.sln文件来编译运行或者调试示例程序
   

## GUI示例
* QtGUI目录为采用Qt写的GUI示例程序
* 在windows或者linux上使用QtCreater打开pro文件
* 构建，运行
   

## python示例
* 在windows或linux平台,打开example/python
* 可以在终端中输入python3 ./demo_robot.py运行示例程序
* 也可以使用VS Code等开发工具打开运行测试

# Contributor
* nieshizhong