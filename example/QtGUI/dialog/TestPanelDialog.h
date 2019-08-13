#ifndef TESTPANELDIALOG_H
#define TESTPANELDIALOG_H

#include <QDialog>
#include <MegaGateway.h>
#include "xthread.h"

namespace Ui {
class TestPanelDialog;
}

class TestPanelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TestPanelDialog(ViSession vi,int robot,int device, QWidget *parent = 0);
    ~TestPanelDialog();

private slots:
    void slotUpdateFirmware();
    void updateFirmwareEndTips(int ret);

    void slotScanMotionFile();

    void slotImportMotionFile();

    void slotMotionRunStop();

    void slotReadMotionFile();

    void slotSystemPatch();

    void slotScriptDownload();

    void slotScriptUpdateInfo();

    void slotScriptStartStop();

    void slotScriptBoot(bool isBoot);

private:
    Ui::TestPanelDialog *ui;
    ViSession m_vi;
    int     m_robotID;
    int     m_deviceID;

    bool m_isDebugRunFlag;
    XThread *m_threadOpsDebug;

    bool m_isScriptRunning;
};

#endif // TESTPANELDIALOG_H
