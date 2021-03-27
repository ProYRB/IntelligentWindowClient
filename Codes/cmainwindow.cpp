/******************************
 *  Author  :   YangRongBao
 *  Date    :   2021.3
******************************/
#include "cmainwindow.h"
#include "ui_cmainwindow.h"

CMainWindow::CMainWindow(const QString account, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CMainWindow)
    , m_account(new const QString(account))
{
    ui->setupUi(this);  //默认代码


    /** [NetworkSystem] */
    /* [NetworkSystem] **/


    /** [FilesSystem] */
    /* [FilesSystem] **/


    /** [Menu\Set] */
    connect(ui->actionModifyPassword, &QAction::triggered, [&]()
    {
        m_modifyPasswordDialog = new CModifyPasswordDialog(*m_account ,this);
        m_modifyPasswordDialog->exec();
        connect(m_modifyPasswordDialog, &CModifyPasswordDialog::modified, [&]()
        {
            delete m_modifyPasswordDialog;
        });
    });
    connect(ui->actionModifySecretiveQuestions, &QAction::triggered, [&]()
    {
        m_modifySecretiveQuestionsDialog = new CModifySecretiveQuestionsDialog(*m_account ,this);
        m_modifySecretiveQuestionsDialog->exec();
        connect(m_modifySecretiveQuestionsDialog, &CModifySecretiveQuestionsDialog::modified, [&]()
        {
            delete m_modifySecretiveQuestionsDialog;
        });
    });
    connect(ui->actionStyle, &QAction::triggered, [&]()
    {
        qDebug() << "修改样式！";
    });
    connect(ui->actionComponents, &QAction::triggered, [&]()
    {
        qDebug() << "设置插件！";
    });
    /* [Menu\Set] **/


    /** [Menu\Help] */
    connect(ui->actionOPI, &QAction::triggered, []()
    {
        CProductInformationDialog productInformationDialog;
        productInformationDialog.exec();
    });
    connect(ui->actionUpdate, &QAction::triggered, [&]()
    {
        qDebug() << "获取更新！";
    });
    connect(ui->actionRepair, &QAction::triggered, [&]()
    {
        qDebug() << "修复错误！";
    });
    connect(ui->actionFeedback, &QAction::triggered, [&]()
    {
        qDebug() << "提交反馈！";
    });
    /* [Menu\Help] **/


    /** [SidedMenu] */
    ui->stackedWidgetMain->setCurrentIndex(0);
    connect(ui->toolButtonHome, &QToolButton::clicked, [&]()
    {
        ui->stackedWidgetMain->setCurrentIndex(0);
    });
    connect(ui->toolButtonStudy, &QToolButton::clicked, [&]()
    {
        ui->stackedWidgetMain->setCurrentIndex(1);
    });
    connect(ui->toolButtonRecreation, &QToolButton::clicked, [&]()
    {
        ui->stackedWidgetMain->setCurrentIndex(2);
    });
    connect(ui->toolButtonSociology, &QToolButton::clicked, [&]()
    {
        ui->stackedWidgetMain->setCurrentIndex(3);
    });
    connect(ui->toolButtonKit, &QToolButton::clicked, [&]()
    {
        ui->stackedWidgetMain->setCurrentIndex(4);
    });
    connect(ui->toolButtonLogout, &QToolButton::clicked, [&]()
    {
        emit logout();
    });
    /* [SidedMenu] **/


    /** [SidedMenu\Recreation\Gobang] */
    ui->stackedWidgetGobang->setCurrentIndex(0);
    connect(ui->toolButtonGobangPVC, &QToolButton::clicked, [&]()
    {
        ui->stackedWidgetGobang->setCurrentIndex(0);
    });
    connect(ui->toolButtonGobangPVP, &QToolButton::clicked, [&]()
    {
        ui->stackedWidgetGobang->setCurrentIndex(1);
    });
    connect(ui->toolButtonGobangQualifying, &QToolButton::clicked, [&]()
    {
        ui->stackedWidgetGobang->setCurrentIndex(2);
    });
    connect(ui->toolButtonGobangRecord, &QToolButton::clicked, [&]()
    {
        ui->stackedWidgetGobang->setCurrentIndex(3);
    });
    connect(ui->toolButtonGobangSetting, &QToolButton::clicked, [&]()
    {
        ui->stackedWidgetGobang->setCurrentIndex(4);
    });
    connect(ui->pushButtonRestart, &QPushButton::clicked, [&]()
    {
        ui->labelGobangPVCChessboard->restart();
        ui->labelBlackPlayerMessage->setText("");
        ui->labelWhitePlayerMessage->setText("");
//        qDebug() << "=====重新开始=====";
    });
    connect(ui->labelGobangPVCChessboard, &CGobangChessboard::blackWin, [&]()
    {
        ui->labelBlackPlayerMessage->setText("黑棋胜利！");
    });
    connect(ui->labelGobangPVCChessboard, &CGobangChessboard::whiteWin, [&]()
    {
        ui->labelWhitePlayerMessage->setText("白棋胜利！");
    });
    /* [SidedMenu\Recreation\Gobang] **/


    /** [StatusMenu] */
    /* [StatusMenu] **/
}

CMainWindow::~CMainWindow()
{
    delete ui;
}
