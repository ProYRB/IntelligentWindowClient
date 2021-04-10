/******************************
 *  Author  :   YangRongBao
 *  Date    :   2021.4
******************************/

#ifndef CLOGINWINDOW_H
#define CLOGINWINDOW_H

#include "cstring.h"
#include "cencrypt.h"
#include "cregisterdialog.h"
#include "cretrievedialog.h"

#include <QCoreApplication>
#include <QDataStream>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QHostAddress>
#include <QMessageBox>
#include <QRegExpValidator>
#include <QPalette>
#include <QString>
#include <QStringList>
#include <QTcpSocket>
#include <QWidget>

#define IP_SERVER "192.168.11.100"
#define PORT_SERVER 8888

namespace Ui {
class CLoginWindow;
}

class CLoginWindow : public QWidget
{
    Q_OBJECT

signals:
    void login(QString account);   //登录成功

public:
    explicit CLoginWindow(QWidget *parent = nullptr);
    ~CLoginWindow();

protected:

private:
    Ui::CLoginWindow *ui;

    const QString m_fileHead = "#APQA";
    const QString m_applicationDirPath = QCoreApplication::applicationDirPath();
    const QString m_path_users_dir = m_applicationDirPath + "/users/";
    const QString m_path_system_users_enc = m_applicationDirPath + "/system/users/";

    QDir m_userEncDir;
    QStringList m_usersList;
    QTcpSocket *m_tcpSocket = nullptr;

    CEncrypt m_encrypt;

private slots:
    void clickedPushButtonConnectServer(QTcpSocket::SocketState state);
    void clickedPushButtonRegisterAccount();
    void clickedPushButtonRetrievePassword();
    void clickedToolButtonLogin(QTcpSocket::SocketState state);

};

#endif // CLOGINWINDOW_H
