#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QProgressBar>

#include "mainwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static MainWindow *_pBackendProxy;
    static void showStatus(QString str);
    static void showProgressBar(bool isRun);

private:
    Ui::MainWindow *ui;

    MainWidget *m_mainWidget;

    QLabel *m_labelVersion;
    QProgressBar *m_progressBar;

    void setupStatusBar();

    void slotShowProgressStatus(bool isRun);
    void slotUpdateStatus(QString str);
};

#endif // MAINWINDOW_H
