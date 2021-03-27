/******************************
 *  Author  :   YangRongBao
 *  Date    :   2021.3
******************************/

#include "cmainwindow.h"
#include "cloginwindow.h"

#include <QApplication>
#include <QObject>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication intelligentWindowClient(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");  //此格式支持中文
    QTextCodec::setCodecForLocale(codec);   //设置文件格式

    CLoginWindow loginWindow;
    loginWindow.show();

    CMainWindow *mainWindow = nullptr;

    QObject::connect(&loginWindow, &CLoginWindow::login, [&](QString account)
    {
        mainWindow = new CMainWindow(account);
        loginWindow.hide();
        mainWindow->show();
        QObject::connect(mainWindow, &CMainWindow::logout, [&](){
            mainWindow->hide();
            loginWindow.show();
            delete mainWindow;
        });
    });

    return intelligentWindowClient.exec();
}
