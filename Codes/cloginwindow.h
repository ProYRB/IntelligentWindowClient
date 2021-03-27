/******************************
 *  Author  :   YangRongBao
 *  Date    :   2021.3
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

    const QString m_fileCode = "#APQA";
    const QString m_path_users_dir = QCoreApplication::applicationDirPath() + "/users/";
    const QString m_path_users_enc = QCoreApplication::applicationDirPath() + "/system/users/";

    QDir m_dirUsersEnc;

    QTcpSocket *m_tcpSocket = nullptr;

    QStringList m_listUsers;

    CEncrypt m_encrypt;

private slots:
    void checkLineEditInput(QLineEdit *lineEidt, CString::CStringModel model);
    void clicked_PushButtonConnectServer(QTcpSocket::SocketState state);
    void clicked_PushButtonRegisterAccount();
    void clicked_PushButtonRetrievePassword();
    void clicked_ToolButtonLogin(QTcpSocket::SocketState state);

};

#endif // CLOGINWINDOW_H
