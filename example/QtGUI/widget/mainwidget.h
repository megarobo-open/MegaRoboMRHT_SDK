#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QMenu>
#include <QStandardItemModel>
#include <QWidget>
#include <QTcpSocket>

#include "MegaGateway.h"
#include "xthread.h"
#include "TestPanelDialog.h"
#include "CanDebugDialog.h"

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

private slots:
    void slotSearchGateway();

    void slotOpenClose();

    void slotGatewaySend();

    void slotGatewayQeury();

    void slotSearchRobot();

    void slotSearchDevice();

    void slotRobotStop();

    void slotRobotGoHome();

    void slotViewLineClicked(QModelIndex index);

    void slotViewLineDoubleClicked(QModelIndex index);

    void slotShowContextmenu(const QPoint &pos);

    void soltActionRemove();

    void soltActionClear();

    void loadConfigFile();

    void saveConfigFile();

    void addItemToTableView(QString cmd);

    void on_pushButtonTest_clicked();

    void slotTcpRead();

    void slotTcpError(QAbstractSocket::SocketError);

    void on_pushButton_cantool_clicked();

private:
    bool GatewayOpen();

    bool GatewayClose();

    void GatewayIdentify(bool isOn);

private:
    Ui::MainWidget *ui;

    bool        m_isOpened;     //当前的打开状态
    bool        m_isOldVersion; //是否为旧版本的-T,不支持重启和非U盘升级

    ViSession   m_vi;           //当前句柄
    QString     m_strVisaDesc;  //Visa描述信息,如:TCPIP0::169.254.1.2::...
    QString     m_IDN;          //IDN信息

    QString     m_strIP;        //IP地址
    QString     m_mrhType;      //MRH-T-06N还是MRH-T-29N
    QString     m_mrhSN;        //MRH-T的序列号
    QString     m_mrhVersion;   //MRH-T的版本号

    int         m_robotID;
    QString     m_robotType;

    int         m_deviceID;
    QString     m_deviceType;

    QStandardItemModel* m_model;
    QMenu *m_menu;

    QStringList m_strCmdList;

    XThread *m_threadUpdateInfo;

    void initUpdateInfoThread();
    QString parseRobotType(int robotID);
    QString getRobotCurrentState();

    TestPanelDialog *m_testPanelDialog;
    CanDebugDialog *m_canDebugDialog;

    QTcpSocket *m_tcpClient;
};

#endif // MAINWIDGET_H
