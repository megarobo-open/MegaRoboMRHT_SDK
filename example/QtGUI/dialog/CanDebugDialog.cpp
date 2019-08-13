#include "CanDebugDialog.h"
#include "ui_CanDebugDialog.h"
#include "sysapi.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QNetworkProxy>

#define MEGAROBO_TCP_CAN_PORT 5588

CanDebugDialog::CanDebugDialog(ViSession vi, QString strIP, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CanDebugDialog)
{
    ui->setupUi(this);
    m_vi = vi;

    m_isRealTimeSave = false;
    m_strIP     = strIP;

    m_isShowing = false;
    m_strCmdList.clear();

    m_strFileName = QApplication::applicationDirPath() + "/CanData.can";

    m_model = new QStandardItemModel(ui->tableView);
    ui->tableView->setModel(m_model);
    m_menu = NULL;

    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tableView, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(slotShowContextmenu(const QPoint&)));

    ui->lineEdit_CanID->setText(QString::number(0x80000001, 16));

    //! 设置输入框必须为十六进制的内容
    QRegExp regExp("[a-fA-F0-9]{16}");
    ui->lineEdit_CanData ->setValidator(new QRegExpValidator(regExp, this));
    ui->lineEdit_CanData->setValidator(new QRegExpValidator(regExp, this));

    connect(ui->lineEdit_CanData,SIGNAL(returnPressed()),this,SLOT(on_toolButton_CanSend_clicked()));
    connect(ui->checkBox_save,SIGNAL(toggled(bool)),this,SLOT(slotRealtimeSaveStateChanged(bool)));

    m_tcpClient = new QTcpSocket(this);   //实例化tcpClient
    m_tcpClient->setProxy(QNetworkProxy::NoProxy);
    m_tcpClient->abort();                 //取消原有连接
    connect(m_tcpClient, SIGNAL(readyRead()), this, SLOT(slotTcpRead()));
    connect(m_tcpClient, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(slotTcpError(QAbstractSocket::SocketError)));


    QString strCmd = ":DEBUG:CAN:MONI ON\n";
    mrgGateWaySendCmd(m_vi, strCmd.toLocal8Bit().data(), strCmd.length());
    qDebug() << "Can Monitor Start";

    QTimer::singleShot(1000, this, SLOT(on_toolButton_StartStopShow_clicked()) );
}

CanDebugDialog::~CanDebugDialog()
{
    closeEvent(NULL);

    if(m_tcpClient != NULL)
        delete m_tcpClient;
    if(m_model != NULL)
        delete m_model;
    delete ui;
}

void CanDebugDialog::closeEvent(QCloseEvent *e)
{
    (void)e;

    if( m_tcpClient->state() == QAbstractSocket::ConnectedState )
    {
        m_tcpClient->disconnectFromHost();
        if (m_tcpClient->state() == QAbstractSocket::UnconnectedState
                || m_tcpClient->waitForDisconnected(1000))
        {
            qDebug() << "TCP Disconnect! 5588";
        }
    }

    QString strCmd = ":DEBUG:CAN:MONI OFF\n";
    mrgGateWaySendCmd(m_vi, strCmd.toLocal8Bit().data(), strCmd.length());
    qDebug() << "Can Monitor Stop";
}

void CanDebugDialog::on_toolButton_clear_clicked()
{
    soltActionClear();
}

void CanDebugDialog::on_toolButton_StartStopShow_clicked()
{
    if( m_strIP.isEmpty() )
        return;

    if(!m_isShowing)
    {
        //实时显示
        m_tcpClient->connectToHost(m_strIP, MEGAROBO_TCP_CAN_PORT );
        if ( !m_tcpClient->waitForConnected(1000) )
        {
            QMessageBox::critical(this, "错误", "连接网关的异常上报服务失败!!!");
        }
        qDebug() << "TCP Connected" << m_strIP << MEGAROBO_TCP_CAN_PORT;
        ui->toolButton_StartStopShow->setText("暂停");
        ui->toolButton_save->setEnabled(false);
        ui->checkBox_save->setEnabled(false);
        m_isShowing = true;
    }
    else
    {
        if( m_tcpClient->state() == QAbstractSocket::ConnectedState )
        {
            m_tcpClient->disconnectFromHost();
            if (m_tcpClient->state() == QAbstractSocket::UnconnectedState
                    || m_tcpClient->waitForDisconnected(1000))
            {
                qDebug() << "TCP Disconnect! 5588";
            }
        }
        ui->toolButton_StartStopShow->setText("继续");
        ui->toolButton_save->setEnabled(true);
        ui->checkBox_save->setEnabled(true);
        m_isShowing = false;
    }
}

void CanDebugDialog::on_toolButton_save_clicked()
{
    QString strFileName = QFileDialog::getSaveFileName(this, "保存文件", "", "日志(*.can)");
    if(!strFileName.isEmpty())
    {
        writeFile(strFileName, m_strCmdList.join('\n') + "\n");
    }

}

void CanDebugDialog::slotRealtimeSaveStateChanged(bool isChecked)
{
    disconnect(ui->checkBox_save,SIGNAL(toggled(bool)),this,SLOT(slotRealtimeSaveStateChanged(bool)));

    if(isChecked)
    {
        QString strFileName = QFileDialog::getSaveFileName(this, "保存文件", "", "日志(*.can)");
        if(strFileName.isEmpty())
        {
            m_strFileName = "";
            return;
        }
        m_strFileName = strFileName;
    }
    else
    {
        m_strFileName = "";
    }
    m_isRealTimeSave = isChecked;

    connect(ui->checkBox_save,SIGNAL(toggled(bool)),this,SLOT(slotRealtimeSaveStateChanged(bool)));
}

//! 清空列表
void CanDebugDialog::soltActionClear()
{
    m_model->clear();
    m_strCmdList.clear();
    writeFile(m_strFileName, "");
}
//! 列表增加一条数据
void CanDebugDialog::addItemToTableView(QString cmd)
{
    m_strCmdList << cmd;
    appendConfigFile(cmd);
    QStandardItem *item = new QStandardItem(cmd);
    m_model->appendRow(item);
}

void CanDebugDialog::slotShowContextmenu(const QPoint& pos)
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

void CanDebugDialog::appendConfigFile(QString str)
{
    if(m_isRealTimeSave)
    {
        writeFile(m_strFileName, str + "\n", true);
    }
}

void CanDebugDialog::slotTcpRead()
{
    while(1)
    {
        QByteArray buffer = m_tcpClient->readLine();
        if(buffer.isEmpty())
        {
            break;
        }
        this->addItemToTableView(QString(buffer).trimmed());
        ui->tableView->scrollToBottom();
        QThread::msleep(1);
    }
}

void CanDebugDialog::slotTcpError(QAbstractSocket::SocketError)
{
    m_tcpClient->disconnectFromHost();
    QMessageBox::critical(this, "错误", QString("TCP出错,连接中断(%1)").arg(m_tcpClient->errorString()) );
}

void CanDebugDialog::on_toolButton_CanReset_clicked()
{
    mrgSysResetCan(m_vi);
}

void CanDebugDialog::on_toolButton_CanSend_clicked()
{
    QString strText = ui->lineEdit_CanData->text();
    if(strText.length()%2 != 0)
    {
        QMessageBox::warning(this, "警告", "输入的数据长度错误");
        return;
    }

    QString strData;
    for(int i=0; i<strText.length(); i+=2)
    {
        strData = strText.mid(i, 2) + strData;
    }

    qDebug() << strData;

    bool isOk;
    long long llNum = strData.toLongLong(&isOk, 16);

    QString strCmd = QString("DEBUG:CAN:DATa %1,%2,%3\n").arg(ui->lineEdit_CanID->text()).arg(llNum).arg(strData.length()/2);
    int ret = mrgGateWaySendCmd(m_vi, strCmd.toLocal8Bit().data(), strCmd.length());
    qDebug() << strCmd << ret;
    if(ret < 0)
        QMessageBox::critical(this, "错误", "发送失败");
}
