/******************************
 *  Author  :   YangRongBao
 *  Date    :   2021.4
******************************/

#include "cloginwindow.h"
#include "cmainwindow.h"
#include "cpainter.h"
#include "cstring.h"

#include <QApplication>
#include <QObject>
#include <QSplashScreen>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication intelligentWindowClient(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");  //此格式支持中文
    QTextCodec::setCodecForLocale(codec);   //设置文件格式

    CPainter painter;
    painter.drawMaze(10, 10, QImage::Format_ARGB32);
    QVector<QImage> enlargeImages;
    for(int i = 0; i < painter.longestMazePathImages().size(); ++i)
    {
        enlargeImages.append(painter.enlargeImage(painter.longestMazePathImages()[i], 5));
    }
    for(int i = 0; i < enlargeImages.size(); ++i)
    {
        enlargeImages[i].save(CString::number(i) + ".png");
    }

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
