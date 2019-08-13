#include "ui_mainwidget.h"
#include <QTimer>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QFile>
#include <QDebug>
#include <QThread>
#include <QInputDialog>
#include <QFileDialog>
#include <QNetworkProxy>

#include "mainwidget.h"
#include "sysapi.h"

#define CONFIG_FILE  (QApplication::applicationDirPath()+"/conf.txt")

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    m_vi = 0;
    m_IDN = "";
    m_strVisaDesc = "";
    m_robotID = 0;
    m_robotType = "";
    m_deviceID = 0;
    m_deviceType = "";

    m_strIP     = "";
    m_mrhType   = "";
    m_mrhSN     = "";
    m_mrhVersion= "";
    m_isOpened = false;

    m_strCmdList.clear();

    m_model = new QStandardItemModel(ui->tableView);
    ui->tableView->setModel(m_model);
    m_menu = NULL;

    m_threadUpdateInfo = NULL;

    m_testPanelDialog = NULL;
    m_canDebugDialog = NULL;

    connect(ui->pushButtonSearchMRHT, SIGNAL(clicked(bool)), this, SLOT(slotSearchGateway()));
    connect(ui->pushButtonOpenMRHT, SIGNAL(clicked(bool)), this, SLOT(slotOpenClose()));

    connect(ui->pushButton_Identify, &QPushButton::pressed, this,
            [=](){GatewayIdentify(true);});

    connect(ui->pushButton_Identify, &QPushButton::released, this,
            [=](){GatewayIdentify(false);});

    connect(ui->pushButton_Send, SIGNAL(clicked(bool)), this, SLOT(slotGatewaySend()));

    connect(ui->pushButton_searchRobot, SIGNAL(clicked(bool)), this, SLOT(slotSearchRobot()));
    connect(ui->pushButton_goHome, SIGNAL(clicked(bool)), this, SLOT(slotRobotGoHome()));
    connect(ui->pushButton_stop, SIGNAL(clicked(bool)), this, SLOT(slotRobotStop()));

    connect(ui->lineEdit,SIGNAL(returnPressed()),ui->pushButton_Send,SLOT(click()));

    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tableView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(slotViewLineClicked(QModelIndex)));

    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(slotViewLineDoubleClicked(QModelIndex)));

    connect(ui->tableView, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(slotShowContextmenu(const QPoint&)));

    //! load config file
    loadConfigFile();

    //! search gateway
    QTimer::singleShot(1000, this, SLOT(slotSearchGateway()) );

    initUpdateInfoThread();

    m_tcpClient = new QTcpSocket(this);   //实例化tcpClient
    m_tcpClient->setProxy(QNetworkProxy::NoProxy);
    m_tcpClient->abort();                 //取消原有连接
    connect(m_tcpClient, SIGNAL(readyRead()), this, SLOT(slotTcpRead()));
    connect(m_tcpClient, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(slotTcpError(QAbstractSocket::SocketError)));

}

MainWidget::~MainWidget()
{
    if(m_testPanelDialog != NULL)
        delete m_testPanelDialog;

    if(m_canDebugDialog != NULL)
        delete m_canDebugDialog;

    delete m_tcpClient;
    delete m_model;
    delete ui;
}

void MainWidget::slotSearchGateway()
{
    auto lambda = [&]()
    {
        QString strFindDevices = "";
        char buff[4096] = "";

        //! 搜索网络
        memset(buff, 0, sizeof(buff));
        mrgFindGateWay(BUS_VXI, buff, sizeof(buff));
        strFindDevices = QString("%1").arg(buff);

        ui->comboBox_mrht->addItems(strFindDevices.split(',', QString::SkipEmptyParts));
    };

    ui->comboBox_mrht->clear();
    sysShowProgressBar(true);
    XThread *thread = new XThread(lambda);
    connect(thread, &XThread::finished, this,
            [this]()
    {
        sysShowProgressBar(false);
        if( ui->comboBox_mrht->count() != 0 )
        {
            ui->pushButtonOpenMRHT->setEnabled(true);
        }
    });

    thread->start();
    ui->pushButtonOpenMRHT->setEnabled(false);
    if( m_isOpened )
    {
        slotOpenClose();
    }
}

void MainWidget::slotOpenClose()
{
    if( !m_isOpened )
    {
        //! 打开
        if( !GatewayOpen() )
        {
            GatewayClose();
            return;
        }

        m_isOpened = true;
        ui->labelIDN->setText(m_IDN);
        ui->pushButtonOpenMRHT->setText("关闭");
        ui->comboBox_mrht->setEnabled(false);
        ui->pushButtonSearchMRHT->setEnabled(false);
        ui->pushButton_Identify->setEnabled(true);

        ui->pushButton_Send->setEnabled(true);
        ui->pushButton_searchRobot->setEnabled(true);
        ui->pushButtonTest->setEnabled(true);
        ui->pushButton_cantool->setEnabled(true);

        if( !m_strIP.isEmpty() )
        {
            m_tcpClient->connectToHost(m_strIP, MEGAROBO_TCP_EXCEPTION_PORT );
            if ( !m_tcpClient->waitForConnected(1000) )
            {
                QMessageBox::critical(this, "错误", "连接网关的异常上报服务失败!!!");
            }
            qDebug() << "TCP Connected" << m_strIP << MEGAROBO_TCP_EXCEPTION_PORT;
        }

        QTimer::singleShot(500, this, SLOT(slotSearchRobot()) );
    }
    else
    {
        //! 关闭
        m_isOpened = false;
        ui->labelIDN->setText("");
        ui->comboBox_mrht->setEnabled(true);
        ui->pushButtonSearchMRHT->setEnabled(true);
        ui->pushButtonOpenMRHT->setText("打开");
        ui->pushButton_Identify->setEnabled(false);

        ui->label_RobotID->setText("");
        ui->label_DeviceID->setText("");
        ui->label_RobotType->setText("");
        ui->label_DeviceType->setText("");
        ui->radioButton_updatePos->setChecked(false);

        ui->pushButton_Send->setEnabled(false);
        ui->pushButton_searchRobot->setEnabled(false);
        ui->radioButton_updatePos->setEnabled(false);
        ui->pushButtonTest->setEnabled(false);
        ui->pushButton_cantool->setEnabled(false);
        ui->pushButton_goHome->setEnabled(false);
        ui->pushButton_stop->setEnabled(false);

        if( !m_strIP.isEmpty() )
        {
            if( m_tcpClient->state() == QAbstractSocket::ConnectedState )
            {
                m_tcpClient->disconnectFromHost();
                if (m_tcpClient->state() == QAbstractSocket::UnconnectedState
                        || m_tcpClient->waitForDisconnected(1000))
                {
                    qDebug() << "TCP Disconnect!"<< MEGAROBO_TCP_EXCEPTION_PORT;
                }
            }
        }

        GatewayClose();
    }
}

void MainWidget::slotGatewaySend()
{
    QString cmd = ui->lineEdit->text();
    if(cmd == "" || !m_isOpened)
    {
        return;
    }

    addItemToTableView(cmd);
    writeFile(CONFIG_FILE, cmd+"\n", true);

    if(cmd.contains("?"))
    {
        slotGatewayQeury();
        return;
    }

    int ret = mrgGateWaySendCmd(m_vi, cmd.toLocal8Bit().data(), cmd.length());
    if(ret <= 0)
    {
        QMessageBox::critical(this, "错误", "发送失败 " + QString::number(ret));
    }
    else
    {
        ui->lineEdit->clear();
    }
    ui->textBrowser->clear();
}

void MainWidget::slotGatewayQeury()
{
    char buff[1024] = "";
    int ret = mrgGateWayQuery(m_vi, ui->lineEdit->text().toLocal8Bit().data(), buff, 1024);
    if(ret <= 0)
    {
        QMessageBox::critical(this, "错误", "查询失败 " + QString::number(ret));
    }
    else
    {
        ui->lineEdit->clear();
        ui->textBrowser->clear();
        ui->textBrowser->setText(QString("%1").arg(buff));
    }
}

void MainWidget::slotSearchRobot()
{
    if(!m_isOpened)
    {
        return;
    }

    int robotNames[128] = {0};
    int ret = mrgGetRobotName(m_vi, robotNames);
    if(ret <= 0)
    {
        QMessageBox::critical(this, "错误", "获取机器人失败 " + QString::number(ret));
        ui->radioButton_updatePos->setEnabled(false);
        ui->pushButton_goHome->setEnabled(false);
        ui->pushButton_stop->setEnabled(false);
        return;
    }

    if(ret == 1)
    {
        m_robotID = robotNames[0];
    }
    else
    {
        QStringList strRobotList;
        for(int i=0; i<ret; i++)
        {
            int robotID = robotNames[i];
            strRobotList << QString("%1:%2").arg(robotID).arg(parseRobotType(robotID));
        }

        QString strRobotName = QInputDialog::getItem(this, tr("机器人列表"), tr("请选择机器人: "), strRobotList, -1, false);
        if(strRobotName == "")
        {
            ui->radioButton_updatePos->setEnabled(false);
            ui->pushButton_goHome->setEnabled(false);
            ui->pushButton_stop->setEnabled(false);
            return;
        }
        m_robotID = strRobotName.split(":", QString::SkipEmptyParts).at(0).toInt();
    }

    m_robotType = parseRobotType(m_robotID);
    ui->label_RobotID->setText( QString("%1").arg(m_robotID));
    ui->label_RobotType->setText( m_robotType );

    ui->radioButton_updatePos->setEnabled(true);
    ui->pushButton_goHome->setEnabled(true);
    ui->pushButton_stop->setEnabled(true);

    slotSearchDevice();
}

QString MainWidget::parseRobotType(int robotID)
{
    QString strRobotType = "";
    if(!m_isOpened || robotID <= 0)
        return strRobotType;

    int ret = mrgGetRobotType(m_vi, robotID);
    switch (ret) {
    case MRX_TYPE_T4:
        strRobotType = "MRX-T4";
        break;
    case MRX_TYPE_H2:
        strRobotType = "MRX-H2";
        break;
    default:
        strRobotType = "UNKOWN";
//        QMessageBox::critical(this, "错误", "机器人类型错误");
        break;
    }
    return strRobotType;
}

void MainWidget::slotSearchDevice()
{
    if(!m_isOpened)
    {
        return;
    }
    int deviceNames[128] = {0};
    int ret;
    ret = mrgGetRobotDevice(m_vi, m_robotID, deviceNames);
    if(ret <= 0)
    {
        QMessageBox::critical(this, "错误", "获取设备失败 " + QString::number(ret));
        return;
    }
    m_deviceID = deviceNames[0];

    char type[128] = "";
    ret = mrgGetDeviceType(m_vi, m_deviceID, type);
    if(0 != ret)
    {
        QMessageBox::critical(this, "错误", "获取设备类型失败");
    }
    m_deviceType = QString("%1").arg(type);
    ui->label_DeviceID->setText(QString("%1").arg(m_deviceID));
    ui->label_DeviceType->setText(m_deviceType);
}

void MainWidget::slotRobotStop()
{
    if(!m_isOpened || m_robotID <= 0)
    {
        return;
    }

    auto lambda = [&]()
    {
        mrgRobotStop(m_vi, m_robotID, WAVETABLE_DEFAULT);
    };

    XThread *thread = new XThread(lambda);
    thread->start();
}

void MainWidget::slotRobotGoHome()
{
    if(!m_isOpened || m_robotID <= 0)
    {
        return;
    }
    auto lambda = [&]()
    {
        ui->pushButton_goHome->setEnabled(false);
        mrgRobotGoHome(m_vi, m_robotID, 0);
        ui->pushButton_goHome->setEnabled(true);
    };

    XThread *thread = new XThread(lambda);
    thread->start();
}

QString MainWidget::getRobotCurrentState()
{
    if(!m_isOpened || m_robotID <= 0)
    {
        return "";
    }
    char args[100];
    char state[100];
    int retlen = 0;
    snprintf(args, 100, "ROBOT:STATe? %d\n", m_robotID);
    if ((retlen = mrgGateWayQuery(m_vi, args, state, 100)) <= 0)
    {
        return "";
    }
    return QString(state);
}

void MainWidget::slotTcpRead()
{
    while(1)
    {
        QByteArray buffer = m_tcpClient->readLine();
        if(buffer.isEmpty())
        {
            break;
        }
        QString strInfo = QString(buffer).split(",", QString::SkipEmptyParts).join("<br>");

        QMessageBox::critical(this, "错误", QString("<center>控制器异常:") + strInfo + "</center>");
    }
}

void MainWidget::slotTcpError(QAbstractSocket::SocketError)
{
    m_tcpClient->disconnectFromHost();
    QMessageBox::critical(this, "错误", QString("TCP出错,连接中断(%1)").arg(m_tcpClient->errorString()) );
}

bool MainWidget::GatewayOpen()
{
    m_strVisaDesc = ui->comboBox_mrht->currentText();
    if( m_strVisaDesc.contains("TCPIP") )
    {
        m_strIP = m_strVisaDesc.split("::", QString::SkipEmptyParts).at(1);
    }

    m_vi = mrgOpenGateWay(BUS_VXI, m_strVisaDesc.toLocal8Bit().data(), 500);
    if(m_vi <= 0)
    {
        QMessageBox::critical(this, "错误", "打开网关失败 " + QString::number(m_vi));
        m_vi = 0;
        return false;
    }

    char idn[128] = "";
    mrgGateWayIDNQuery(m_vi, idn);
    m_IDN = QString("%1").arg(idn);
    if(m_IDN.isEmpty() || m_IDN.split(",",QString::SkipEmptyParts).length() != 4)
    {
        QMessageBox::critical(this, "错误", "获取IDN失败");
        return false;
    }

    m_mrhType = m_IDN.split(",",QString::SkipEmptyParts).at(1);
    m_mrhSN = m_IDN.split(",",QString::SkipEmptyParts).at(2);
    m_mrhVersion = m_IDN.split(",",QString::SkipEmptyParts).at(3);

    int MajorVer = m_mrhVersion.split(".",QString::SkipEmptyParts).at(2).toInt();
    int MinVer = m_mrhVersion.split(".",QString::SkipEmptyParts).at(3).toInt();
    if((MajorVer == 1 && MinVer < 22) || (MajorVer == 2 && MinVer < 6))
    {
        //MRH-T-06N版本小于1.22, MRH-T-29N版本小于2.06
        m_isOldVersion = true;
    }
    else
    {
        m_isOldVersion = false;
    }

    return true;
}

bool MainWidget::GatewayClose()
{
    if(m_vi > 0)
    {
        mrgCloseGateWay(m_vi);
    }

    m_vi = 0;
    m_IDN = "";
    m_strVisaDesc = "";
    m_strIP         = "";
    m_mrhType       = "";
    m_mrhSN         = "";
    m_mrhVersion    = "";

    return true;
}

void MainWidget::GatewayIdentify(bool isOn)
{
    if(!m_isOpened)
    {
        return;
    }

    mrgIdentify(m_vi, isOn?1:0);
    qDebug() << "slotGatewayIdentify:" << isOn;
}

void MainWidget::initUpdateInfoThread()
{
    auto lambda = [&]()
    {
        while(1)
        {
            //! 如果取消打钩或关闭网关或没有构建机器人, 就不更新位置信息
            if ( !ui->radioButton_updatePos->isChecked() || m_vi <= 0 || m_robotID <= 0)
            {
                QThread::msleep(3000);
                ui->doubleSpinBox_posX->setValue(0);
                ui->doubleSpinBox_posY->setValue(0);
                ui->doubleSpinBox_posZ->setValue(0);
                continue;
            }

            float fx = -1, fy = -1, fz = -1;
            int ret = mrgGetRobotCurrentPosition(m_vi, m_robotID, &fx, &fy, &fz);
            if(ret != 0)
            {
                fx = -1;
                fy = -1;
                fz = -1;
            }
            ui->doubleSpinBox_posX->setValue(fx);
            ui->doubleSpinBox_posY->setValue(fy);
            ui->doubleSpinBox_posZ->setValue(fz);

            ui->label_robotState->setText(getRobotCurrentState());

//            int code = 0;
//            char errBuf[1024] = "";
//            mrgGetRobotErrorInfo(m_vi, m_robotID, &code, errBuf);
//            if(code != 0)
//            {
//                ui->label_ErrorMsg->setText("ERROR[" + QString::number(code) + "]\n" + QString(errBuf));
//            }
//            else
//            {
//                ui->label_ErrorMsg->setText( " " );
//            }

            QThread::msleep(500);
        }
    };

    m_threadUpdateInfo = new XThread(lambda);
    m_threadUpdateInfo->start();
}

void MainWidget::loadConfigFile()
{
    QString config = readFile(CONFIG_FILE);
    m_strCmdList = config.split("\n", QString::SkipEmptyParts);

    saveConfigFile();

    foreach (QString line, m_strCmdList) {
        addItemToTableView(line);
    }
}

void MainWidget::saveConfigFile()
{
    removeListSame(m_strCmdList);
    writeFile(CONFIG_FILE, m_strCmdList.join('\n') + "\n");
}

void MainWidget::slotViewLineClicked(QModelIndex index)
{
    QString str = index.data().toString();
    ui->lineEdit->setText(str);
}

void MainWidget::slotViewLineDoubleClicked(QModelIndex index)
{
    QString str = index.data().toString();
    ui->lineEdit->setText(str);
    ui->pushButton_Send->click();
}

void MainWidget::slotShowContextmenu(const QPoint& pos)
{
    Q_UNUSED(pos);
    if(!((ui->tableView->selectionModel()->selectedIndexes()).empty()))
    {
        if(m_menu != NULL)
            delete m_menu;

        m_menu = new QMenu(ui->tableView);
        QAction *actionRemove = m_menu->addAction(tr("删除"));
        QAction *actionClear = m_menu->addAction(tr("清空"));

        connect(actionRemove, SIGNAL(triggered(bool)), this, SLOT(soltActionRemove()));
        connect(actionClear, SIGNAL(triggered(bool)), this, SLOT(soltActionClear()));

        m_menu->exec(QCursor::pos());
        ui->tableView->selectionModel()->clear();
    }
}

//! 列表删除一条数据
void MainWidget::soltActionRemove()
{
    int curRow = ui->tableView->currentIndex().row();
    m_model->removeRow(curRow);
//    m_strCmdList.removeOne( m_model->index(curRow, 0).data().toString() );
    m_strCmdList.removeAt(curRow);
    saveConfigFile();
}

//! 清空列表
void MainWidget::soltActionClear()
{
    m_model->clear();
    m_strCmdList.clear();
    saveConfigFile();
}

//! 列表增加一条数据
void MainWidget::addItemToTableView(QString cmd)
{
    if(!m_strCmdList.contains(cmd))
    {
        m_strCmdList << cmd;
        saveConfigFile();
    }

    bool isExist = false;
    int maxRow = m_model->rowCount();
    for(int index=0; index<maxRow; index++)
    {
        if(cmd == m_model->index(index, 0).data().toString())
        {
            isExist = true;
        }
    }
    if(!isExist)
    {
        QStandardItem *item = new QStandardItem(cmd);
        m_model->appendRow(item);
    }
}

void MainWidget::on_pushButtonTest_clicked()
{
    if(!m_isOpened)
    {
        QMessageBox::warning(this,"警告","没有打开网关");
        return;
    }

    if(m_testPanelDialog != NULL)
    {
        delete m_testPanelDialog;
        m_testPanelDialog = NULL;
    }

    m_testPanelDialog = new TestPanelDialog(m_vi, m_robotID, m_deviceID, this);
//    m_testPanelDialog->exec();
    m_testPanelDialog->show();
}


void MainWidget::on_pushButton_cantool_clicked()
{
    if(!m_isOpened)
    {
        QMessageBox::warning(this,"警告","没有打开网关");
        return;
    }

    if(m_canDebugDialog != NULL)
    {
        delete m_canDebugDialog;
        m_canDebugDialog = NULL;
    }

    m_canDebugDialog = new CanDebugDialog(m_vi, m_strIP, this);
    m_canDebugDialog->show();
}
