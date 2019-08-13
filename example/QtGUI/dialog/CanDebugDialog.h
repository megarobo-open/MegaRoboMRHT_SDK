#ifndef CANDEBUGDIALOG_H
#define CANDEBUGDIALOG_H

#include <QDialog>
#include <MegaGateway.h>
#include <QStandardItemModel>
#include <QMenu>
#include <QTcpSocket>
#include "xthread.h"

namespace Ui {
class CanDebugDialog;
}

class CanDebugDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CanDebugDialog(ViSession vi, QString strIP, QWidget *parent = 0);
    ~CanDebugDialog();

    void closeEvent(QCloseEvent *e);

private slots:
    void on_toolButton_clear_clicked();

    void on_toolButton_StartStopShow_clicked();

    void on_toolButton_save_clicked();

    void slotRealtimeSaveStateChanged(bool isChecked);

    void soltActionClear();
    void addItemToTableView(QString cmd);
    void slotShowContextmenu(const QPoint &pos);
    void appendConfigFile(QString str);

    void slotTcpRead();
    void slotTcpError(QAbstractSocket::SocketError);

    void on_toolButton_CanReset_clicked();

    void on_toolButton_CanSend_clicked();

private:
    Ui::CanDebugDialog *ui;

    ViSession m_vi;
    QString     m_strIP;        //IP地址

    bool    m_isRealTimeSave;
    QTcpSocket *m_tcpClient;


    QStandardItemModel* m_model;
    QMenu *m_menu;
    QStringList m_strCmdList;

    bool m_isShowing;
    QString m_strFileName;
};

#endif // CANDEBUGDIALOG_H
