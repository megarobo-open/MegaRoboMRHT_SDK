#include "TestPanelDialog.h"
#include "ui_TestPanelDialog.h"

#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QTimer>

TestPanelDialog::TestPanelDialog(ViSession vi, int robot, int device, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestPanelDialog)
{
    ui->setupUi(this);
    m_vi = vi;
    m_robotID = robot;
    m_deviceID = device;

     m_isDebugRunFlag = false;
     m_threadOpsDebug = NULL;
     m_isScriptRunning = false;

     ui->textBrowser->setHidden(true);

     //隐藏
     ui->spinBox_record->setVisible(false);

     connect(ui->pushButton_system_patch, SIGNAL(pressed()),this,SLOT(slotSystemPatch()));
     connect(ui->pushButton_updateFirmware, SIGNAL(pressed()),this,SLOT(slotUpdateFirmware()));

     connect(ui->pushButton_ScanMotionFile, SIGNAL(pressed()),this,SLOT(slotScanMotionFile()));
     connect(ui->pushButton_ImportMotionFile, SIGNAL(pressed()),this,SLOT(slotImportMotionFile()));
     connect(ui->pushButton_MotionRunStop, SIGNAL(pressed()),this,SLOT(slotMotionRunStop()));
     connect(ui->pushButton_ReadMotionFile, SIGNAL(pressed()),this,SLOT(slotReadMotionFile()));

     connect(ui->toolButton_script_download, SIGNAL(pressed()),this,SLOT(slotScriptDownload()));
     connect(ui->toolButton_script_update, SIGNAL(pressed()),this,SLOT(slotScriptUpdateInfo()));
     connect(ui->toolButton_scriptStartStop, SIGNAL(pressed()),this,SLOT(slotScriptStartStop()));
     connect(ui->checkBox_script_boot, SIGNAL(toggled(bool)),this,SLOT(slotScriptBoot(bool)));

     QTimer::singleShot(500, this, SLOT(slotScanMotionFile()) );
     QTimer::singleShot(500, this, SLOT(slotScriptUpdateInfo()) );
}

TestPanelDialog::~TestPanelDialog()
{
    delete ui;
    if( m_threadOpsDebug != NULL ){
        m_threadOpsDebug->requestInterruption();
        m_threadOpsDebug->wait();
        delete m_threadOpsDebug;
    }
}

void TestPanelDialog::slotUpdateFirmware()
{
    QString absPathFileName = QFileDialog::getOpenFileName(this, "文件对话框", "", "升级文件(*.mrh)");
    qDebug() << absPathFileName;
    if(absPathFileName == "")
        return;

    auto lambda = [=](int &ret)
    {
        time_t tm_begin = time(NULL);

        QFile file(absPathFileName);
        if(!file.open(QIODevice::ReadOnly))
        {
            qDebug() << QString("Can't ReadOnly open the file: %1").arg(absPathFileName);
            return;
        }

        int filesize = file.size();
        QString filename = absPathFileName.split("/", QString::SkipEmptyParts).last();

        qDebug() << "download file start:" << filename << filesize;
        ui->pushButton_updateFirmware->setText("正在下载...");

        unsigned char *ptr = file.map(0, filesize);

        ret = mrgStorageWriteFile(m_vi, 0, (char *)"/media/usb0/",
                                  filename.toLocal8Bit().data(), ptr, filesize);
        file.unmap(ptr);
        file.close();
        qDebug() << "download file end:" << filename << ret;
        if(ret != 0)
        {
            ret = -1;
            return;
        }

        qDebug() << "system update start:" << filename;
        ui->pushButton_updateFirmware->setText("正在升级...");

        ret = mrgSysUpdateFileStart(m_vi, filename.toLocal8Bit().data());
        qDebug() << "system update end:" << ret;

        QString strCmd = "rm -rf /media/usb0/MRH-T*.mrh";
        mrgSystemRunCmd(m_vi, strCmd.toLocal8Bit().data(), 0);
        if(ret != 0)
        {
            ret = -2;
            return;
        }

        qDebug() << "Run time:" << time(NULL) - tm_begin;
        ret = 0;
        return;
    };

    XThread *thread = new XThread(lambda);
    connect(thread, SIGNAL(signalFinishResult(int)), this, SLOT(updateFirmwareEndTips(int)));
    thread->start();

    return;
}

void TestPanelDialog::updateFirmwareEndTips(int ret)
{
    ui->pushButton_updateFirmware->setText("升级固件");
    if(ret == 0)
    {
        QMessageBox *msgBox = new QMessageBox(this);
        msgBox->setText("\n\t升级成功\t\t \n\n\t是否重启MRH-T？\t \n");
        msgBox->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        if(msgBox->exec() == QMessageBox::Ok)
        {
            //! 重启-T
            mrgSystemRunCmd(m_vi, (char *)"reboot", 0);
        }
    }
    else if(ret == -1)
    {
        QMessageBox::critical(this, "错误", "下载失败!");
    }
    else if(ret == -2)
    {
        QMessageBox::critical(this, "错误", "升级失败!");
    }
    else
    {
        QMessageBox::critical(this, "错误", "失败(" + QString::number(ret) + ")");
    }
    return;
}

void TestPanelDialog::slotScanMotionFile()
{
    auto lambda = [&]()
    {
        QString strFindMotionFiles = "";
        char buff[4096] = "";

        //! 搜索网络
        memset(buff, 0, sizeof(buff));
        mrgStorageMotionFileQuery(m_vi, 0, buff, sizeof(buff));
        strFindMotionFiles = QString("%1").arg(buff);

        if(strFindMotionFiles.toLower().contains(".pvt") || strFindMotionFiles.toLower().contains(".mrp"))
        {
            ui->comboBox_MotionFile->addItems(strFindMotionFiles.split(',', QString::SkipEmptyParts));
        }
    };

    ui->comboBox_MotionFile->clear();
    XThread *thread = new XThread(lambda);
    thread->start();
}

void TestPanelDialog::slotImportMotionFile()
{
    if(m_robotID <= 0)
    {
        QMessageBox::critical(this, "错误", "没有机器人");
        return;
    }

    QString strMotionFileName = ui->comboBox_MotionFile->currentText();
    if(strMotionFileName == "")
    {
        QMessageBox::critical(this, "错误", "选择文件名为空");
        return;
    }
    int ret = mrgRobotMotionFileImport(m_vi, m_robotID, strMotionFileName.toLocal8Bit().data());
    if(ret != 0){
        QMessageBox::critical(this, "错误", "文件导入失败" + QString::number(ret));
    }
}

void TestPanelDialog::slotMotionRunStop()
{

    auto func = [this](int &ret)
    {
        int section = ui->spinBox_section->value();
        int line = ui->spinBox_record->value();
        bool isCyclic = ui->checkBox_Cyclic->isChecked();
        bool noBreak = true;
        do
        {
#if 0       //! 默认方式,1.21之前的版本
            ret = mrgRobotFileResolve(m_vi, m_robotID, section, line, 0, 20000);
            if(ret != 0) {
                qDebug() << "mrgRobotFileResolve" << ret;
                return;
            }

            ret = mrgRobotRun(m_vi, m_robotID, 0);
            if(ret != 0) {
                qDebug() << "mrgRobotRun" << ret;
                return;
            }

            while(1)
            {
                char buff[8] = "";
                mrgGetRobotCurrentRecord(m_vi, m_robotID, buff);
                QStringList lst = QString(buff).split(":", QString::SkipEmptyParts);
                if(lst.size() >= 2)
                {
                    ui->lcdNumber_section->display(lst.at(0));
                    ui->lcdNumber_record->display(lst.at(1));
                }

                QThread::msleep(500);
                char state[8] = "";
                mrgGetRobotStates(m_vi, m_robotID, 0, state);
                QString strState = QString(state);
                if(strState == "ERROR")
                {
                    qDebug() << "mrgRobotFileResolve Run:" << strState;
                    return;
                }

                if(strState == "IDLE" || strState == "STOP")
                {
                    break;
                }
                if ( m_threadOpsDebug->isInterruptionRequested() ){
                    mrgRobotStop(m_vi, m_robotID, -1);
                    noBreak = false;
                    break;
                }
            }
#endif

#if 0
            // 1.21之后的版本,永久等待
            ret = mrgRobotFileResolve(m_vi, m_robotID, section, line, 0, 0);
            qDebug() << "mrgRobotFileResolve" << ret;
            if(ret != 0) {
                return;
            }
            if ( m_threadOpsDebug->isInterruptionRequested() ){
                mrgRobotStop(m_vi, m_robotID, -1);
                noBreak = false;
                break;
            }
#else            
            ret = mrgRobotFileResolve(m_vi, m_robotID, section, line, WAVETABLE_DEFAULT, -1);
            if(ret != 0) {
                qDebug() << "mrgRobotFileResolve" << ret;
                return;
            }

            QString strLastStates = "";
            while(1)
            {
                QThread::msleep(100);

                char buff[8] = "";
                mrgGetRobotCurrentRecord(m_vi, m_robotID, buff);
                QStringList lst = QString(buff).split(":", QString::SkipEmptyParts);
                if(lst.size() >= 2)
                {
                    ui->lcdNumber_section->display(lst.at(0));
                    ui->lcdNumber_record->display(lst.at(1));
                }

                char state[8] = "";
                mrgGetRobotStates(m_vi, m_robotID, WAVETABLE_DEFAULT, state);
                QString strState = QString(state);
                if(strState == "ERROR")
                {
                    qDebug() << "mrgRobotFileResolve Run:" << strState;
                    return;
                }

                if( (strState == "IDLE" || strState == "STOP")
                        && (strLastStates == strState) )
                {
                    qDebug() << strState << "Running End";
                    break;
                }

                strLastStates = strState;

                if ( m_threadOpsDebug->isInterruptionRequested() ){
                    mrgRobotStop(m_vi, m_robotID, WAVETABLE_DEFAULT);
                    noBreak = false;
                    break;
                }
            }
#endif

            isCyclic = ui->checkBox_Cyclic->isChecked();
        }while(isCyclic && noBreak);

        ui->pushButton_MotionRunStop->setText("运行");
        m_isDebugRunFlag = false;
    }; //end func


    if(m_isDebugRunFlag) //正在运行
    {
        m_isDebugRunFlag = false;

        if( m_threadOpsDebug != NULL ){
            m_threadOpsDebug->requestInterruption();
            m_threadOpsDebug->wait();
        }
        ui->pushButton_MotionRunStop->setText("运行");
    }
    else
    {
        m_isDebugRunFlag = true;
        ui->pushButton_MotionRunStop->setText("停止");

        if(m_vi <= 0) return;
        m_threadOpsDebug = new XThread(func);
        connect(m_threadOpsDebug,&XThread::finished,[&](){ m_threadOpsDebug = NULL; });
        m_threadOpsDebug->start(QThread::LowestPriority);
    }

}

void TestPanelDialog::slotReadMotionFile()
{
    ui->textBrowser->clear();

    if(ui->textBrowser->isHidden())
    {
        //! 读文件
        int len = 4096 * 1024;
        char *pData = (char *)malloc(len);
        memset(pData, 0, len);

        QString strMotionFileName = ui->comboBox_MotionFile->currentText();
        if(strMotionFileName == "")
        {
            QMessageBox::critical(this, "错误", "选择文件名为空");
            return;
        }

        int ret = mrgStorageMotionFileContextRead(m_vi, strMotionFileName.toLocal8Bit().data(), pData);
        qDebug() << "mrgStorageMotionFileContextRead" << ret;
        QString strFileText = QString(pData);
        free(pData);
        if(ret <= 0){
            QMessageBox::critical(this, "错误", "文件读取失败" + QString::number(ret));
            return;
        }

        ui->textBrowser->setText(strFileText);
        ui->textBrowser->setHidden(false);
        ui->pushButton_ReadMotionFile->setText("关闭文件");
    }
    else
    {
        ui->textBrowser->setHidden(true);
        ui->pushButton_ReadMotionFile->setText("读取文件");
    }
}

void TestPanelDialog::slotSystemPatch()
{
#if 01
    int ret;
    QStringList lstAbsPathFileName;
    QString strAbsPath = QFileDialog::getExistingDirectory(this, "选择文件夹", "");
    QDir dir(strAbsPath);
    if(strAbsPath.isEmpty())
        return;
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        lstAbsPathFileName << fileInfo.filePath();
    }

    qDebug() << lstAbsPathFileName;
    if(lstAbsPathFileName.isEmpty())
        return;

    //! 下载文件
    QString targetAbsPath = "/home/megarobo/update/update.src/";
    foreach (QString absPathFileName, lstAbsPathFileName)
    {
        QFile file(absPathFileName);
        if(!file.open(QIODevice::ReadOnly))
        {
            qDebug() << QString("Can't ReadOnly open the file: %1").arg(absPathFileName);
            return;
        }

        int filesize = file.size();
        QString filename = absPathFileName.split("/", QString::SkipEmptyParts).last();

        qDebug() << "download file start:" << filename << "size:" << filesize;
        unsigned char *ptr = file.map(0, filesize);
        ret = mrgStorageWriteFile(m_vi, 0, targetAbsPath.toLocal8Bit().data(),
                                  filename.toLocal8Bit().data(), ptr, filesize);
        file.unmap(ptr);
        file.close();
        qDebug() << "download file end:" << filename << ((ret!=0)?"ERROR":"OK");
        if(ret != 0)
        {
            ret = -1;
            return;
        }
    }

    //! 执行更新操作
    QString strCmd = "/bin/bash /home/megarobo/update/sysupdate.sh";
    ret = mrgSystemRunCmd(m_vi, strCmd.toLocal8Bit().data(), 0);
    if(ret == 0)
    {
        char *textBuff = (char *)malloc(1024 * 1024);
        mrgStorageReadFile(m_vi, 0, (char *)"/home/megarobo/update/", (char *)"log", (unsigned char*)textBuff);
        ui->textBrowser->setText(QString(textBuff));
        free(textBuff);
        ui->textBrowser->show();
        QMessageBox::critical(this, "错误", "打补丁出错!");
        ui->textBrowser->hide();
        return;
    }
    else
    {
        char *textBuff = (char *)malloc(1024 * 1024);
        mrgStorageReadFile(m_vi, 0, (char *)"/home/megarobo/update/", (char *)"history", (unsigned char*)textBuff);

        ui->textBrowser->setText(QString(textBuff));
        free(textBuff);
        ui->textBrowser->show();
        QMessageBox::information(this, "提示", "成功");
        ui->textBrowser->hide();
    }
#endif

}

void TestPanelDialog::slotScriptDownload()
{
    QStringList lstAbsPathFileName = QFileDialog::getOpenFileNames(this, "文件对话框", "", "所有文件(*.*)");
    qDebug() << lstAbsPathFileName;
    if(lstAbsPathFileName.isEmpty())
        return;

    auto lambda = [=](int &ret)
    {
         QString targetAbsPath = "/home/megarobo/MRH-T/script/";

        foreach (QString absPathFileName, lstAbsPathFileName)
        {
            QFile file(absPathFileName);
            if(!file.open(QIODevice::ReadOnly))
            {
                qDebug() << QString("Can't ReadOnly open the file: %1").arg(absPathFileName);
                return;
            }

            int filesize = file.size();
            QString filename = absPathFileName.split("/", QString::SkipEmptyParts).last();

            qDebug() << "download file start:" << filename << "size:" << filesize;
            unsigned char *ptr = file.map(0, filesize);
            ret = mrgStorageWriteFile(m_vi, 0, targetAbsPath.toLocal8Bit().data(),
                                      filename.toLocal8Bit().data(), ptr, filesize);
            file.unmap(ptr);
            file.close();
            qDebug() << "download file end:" << filename << ((ret!=0)?"ERROR":"OK");
            if(ret != 0)
            {
                ret = -1;
                return;
            }
        }
        ret = 0;
        return;
    };

    XThread *thread = new XThread(lambda);
    thread->start();
}

void TestPanelDialog::slotScriptUpdateInfo()
{
    char buff[4096] = "";

    QString targetAbsPath = "/home/megarobo/MRH-T/script/";
    int len = sizeof(buff);
    mrgStorageDirectoryEnum(m_vi, 0, targetAbsPath.toLocal8Bit().data(), buff, &len);
    qDebug() << "script list:" << QString( buff );
    ui->comboBox_script_filelist->clear();
    ui->comboBox_script_filelist->addItems( QString(buff).split("\n", QString::SkipEmptyParts));

    mrgScriptGetConfig(m_vi, buff);
    ui->comboBox_script_filelist->setCurrentText(QString(buff));

    int ret = mrgScriptGetCurrentStates(m_vi);
    if(ret < 0)
        return;

    qDebug() << "script status:" << ret;
    ui->label_script_running_status->setText( (ret==1) ? "RUNNING" : "STOP");
}

void TestPanelDialog::slotScriptStartStop()
{
    int ret = 0;

    qDebug() << "m_isScriptRunning:" << m_isScriptRunning;

    if( !m_isScriptRunning )
    {
        QString strConfigName = ui->comboBox_script_filelist->currentText();
        bool isBoot = ui->checkBox_script_boot->isChecked();

        if(strConfigName.right(3) != ".py")
        {
            QMessageBox::critical(this, "错误", "配置项不是.py文件");
            return;
        }

        ret = mrgScriptSetConfig(m_vi, strConfigName.toLocal8Bit().data(), isBoot?1:0);
        if(ret!=0)
        {
            QMessageBox::critical(this, "错误", "配置失败");
            return;
        }

        ret = mrgScriptRun(m_vi);
        if(ret!=0)
        {
            QMessageBox::critical(this, "错误", "运行失败");
            return;
        }

        m_isScriptRunning = true;
        ui->toolButton_scriptStartStop->setText("停止");
    }
    else
    {
        ret = mrgScriptStop(m_vi);
        if(ret!=0)
        {
            QMessageBox::critical(this, "错误", "停止失败");
            return;
        }
        m_isScriptRunning = false;
        ui->toolButton_scriptStartStop->setText("运行");
    }

    slotScriptUpdateInfo();
}

void TestPanelDialog::slotScriptBoot(bool isBoot)
{
    disconnect(ui->checkBox_script_boot, SIGNAL(toggled(bool)),this,SLOT(slotScriptBoot(bool)));
    QString strConfigName = ui->comboBox_script_filelist->currentText();
    if(strConfigName.right(3) != ".py")
    {
        QMessageBox::critical(this, "错误", "配置项不是.py文件");
        ui->checkBox_script_boot->setChecked(!isBoot);
        return;
    }

    int ret = mrgScriptSetConfig(m_vi, strConfigName.toLocal8Bit().data(), isBoot?1:0);
    if(ret!=0)
    {
        QMessageBox::critical(this, "错误", "配置失败");
        ui->checkBox_script_boot->setChecked(!isBoot);
        return;
    }
    connect(ui->checkBox_script_boot, SIGNAL(toggled(bool)),this,SLOT(slotScriptBoot(bool)));
}
