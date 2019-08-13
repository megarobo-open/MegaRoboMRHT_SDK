#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow *MainWindow::_pBackendProxy = NULL;

void MainWindow::showStatus(QString str)
{
    if( NULL != MainWindow::_pBackendProxy )
    {
        MainWindow::_pBackendProxy->slotUpdateStatus(str);
    }
}

void MainWindow::showProgressBar(bool isRun)
{
    if( NULL != MainWindow::_pBackendProxy )
    {
        MainWindow::_pBackendProxy->slotShowProgressStatus(isRun);
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("镁伽机器人MRH-T测试工具");

    //! register the proxy
    MainWindow::_pBackendProxy = this;

    m_mainWidget = new MainWidget;

    setCentralWidget(m_mainWidget);

    setupStatusBar();
}

MainWindow::~MainWindow()
{
    delete m_mainWidget;
    delete m_progressBar;
    delete m_labelVersion;
    delete ui;
}


void MainWindow::setupStatusBar()
{
    m_progressBar = new QProgressBar;
    m_progressBar->setMaximum(100);
    m_progressBar->setHidden(true);
    m_labelVersion = new QLabel(QString("Version: %1 ").arg(qApp->applicationVersion()));

    ui->statusbar->addPermanentWidget(m_progressBar, 0);
    ui->statusbar->addPermanentWidget(m_labelVersion, 0);
}

void MainWindow::slotUpdateStatus(QString str)
{
    ui->statusbar->setStatusTip(str);
}

void MainWindow::slotShowProgressStatus(bool isRun)
{
    if(isRun) {
        m_progressBar->setHidden(false);
        m_progressBar->setMaximum(0);
    } else {
        m_progressBar->setMaximum(100);
        m_progressBar->setHidden(true);
    }
}
