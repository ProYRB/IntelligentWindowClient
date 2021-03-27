/******************************
 *  Author  :   YangRongBao
 *  Date    :   2021.3
******************************/

#include "cloginwindow.h"
#include "ui_cloginwindow.h"

CLoginWindow::CLoginWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CLoginWindow)
{
    ui->setupUi(this);


    /** [NetworkSystem] */
//    m_tcpSocket = new QTcpSocket(this);
//    m_tcpSocket->connectToHost(QHostAddress(IP_SERVER), 8888);
    /* [NetworkSystem] **/


    /** [FilesSystem] */
    m_dirUsersEnc.setPath(m_path_users_enc);
    if(!m_dirUsersEnc.exists())
    {
        m_dirUsersEnc.mkpath(m_path_users_enc);
    }
    /* [FilesSystem] **/


    /** [WidgetBackground] */
//    QPalette palette;
//    palette.setBrush(QPalette::Background,QBrush(QPixmap(":/image/png/sign.png").scaled(this->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
//    this->setPalette(palette);
    /* [WidgetBackground] **/


    /** [MessagesLabel] */
    /* [MessagesLabel] **/


    /** [LineEdits] */
    connect(ui->lineEditAccount, &QLineEdit::textChanged, [&]()
    {
        this->checkLineEditInput(ui->lineEditAccount, CString::Model_Account);
    });
    connect(ui->lineEditAccount, &QLineEdit::textChanged, [&]()
    {
        this->checkLineEditInput(ui->lineEditAccount, CString::Model_Password);
    });
    /* [LineEdits] **/


    /** [ConnectionState] */
//    connect(m_tcpSocket, &QTcpSocket::connected, [&]()
//    {
//        ui->pushButtonConnectServer->setIcon(QIcon(":/pictures/ico/wifi.ico"));
//    });
//    connect(m_tcpSocket, &QTcpSocket::disconnected, [&]()
//    {
//        ui->pushButtonConnectServer->setIcon(QIcon(":/pictures/ico/stop.ico"));
//    });
//    connect(ui->pushButtonConnectServer, &QPushButton::clicked, [&]()
//    {
//        this->clicked_PushButtonConnectServer(m_tcpSocket->state());
//    });
    /* [ConnectionState] **/


    /** [LoginButton] */
    connect(ui->toolButtonLogin, &QToolButton::clicked, [&]()
    {
//        this->clicked_ToolButtonLogin(m_tcpSocket->state());
        this->clicked_ToolButtonLogin(QTcpSocket::SocketState::UnconnectedState);
    });
    /* [LoginButton] **/


    /** [RegisterAccount] */
    connect(ui->pushButtonRegisterAccount, &QPushButton::clicked, this, &CLoginWindow::clicked_PushButtonRegisterAccount);
    /* [RegisterAccount] **/


    /** [RetrievePassword] */
    connect(ui->pushButtonRetrievePassword, &QPushButton::clicked, this, &CLoginWindow::clicked_PushButtonRetrievePassword);
    /* [RetrievePassword] **/
}

CLoginWindow::~CLoginWindow()
{
    delete ui;
}

void CLoginWindow::checkLineEditInput(QLineEdit *lineEidt, CString::CStringModel model)
{
    CString checkedInput(lineEidt->text());
    if(checkedInput.CheckChar(checkedInput.GetStringSize() - 1, model) == CString::Error_CharOverRange)
    {
        if(checkedInput.DeleteChar(checkedInput.GetStringSize() - 1) == CString::Error_None)
        {
            lineEidt->setText(checkedInput.GetString());
        }
        else
        {
            lineEidt->clear();
        }
        switch(model)
        {
        case CString::Model_Account:
        {
            QMessageBox::warning(this, "提示", "账号只能包含阿拉伯数字和大小写字母！");
            break;
        }
        case CString::Model_Password:
        {
            QMessageBox::warning(this, "提示", "密码只能包含阿拉伯数字、大小写字母以及一部分特殊符号！");
            break;
        }
        }

    }
}

void CLoginWindow::clicked_PushButtonConnectServer(QTcpSocket::SocketState state)
{
    switch(state)
    {
    default:
    {
        break;
    }
    case QTcpSocket::SocketState::ConnectedState:
    {
        m_tcpSocket->disconnectFromHost();
        m_tcpSocket->close();
        break;
    }
    case QTcpSocket::SocketState::UnconnectedState:
    {
        m_tcpSocket->connectToHost(QHostAddress(IP_SERVER), 8888);
        break;
    }
    }
}

void CLoginWindow::clicked_PushButtonRegisterAccount()
{
    CRegisterDialog registerDialog;
    connect(&registerDialog, &CRegisterDialog::registered, [&](QString registerAccount, QString registerPassword)
    {
        ui->lineEditAccount->setText(registerAccount);
        ui->lineEditPassword->setText(registerPassword);
    });
    registerDialog.exec();
}

void CLoginWindow::clicked_PushButtonRetrievePassword()
{
    CRetrieveDialog retrieveDialog;
    retrieveDialog.exec();
}

void CLoginWindow::clicked_ToolButtonLogin(QTcpSocket::SocketState state)
{
    QString editAccount = ui->lineEditAccount->text();
    QString editPassword = ui->lineEditPassword->text();
    if(editAccount == "")
    {
        QMessageBox::warning(this, "提示", "账号不能为空！");
        return;
    }
    if(editPassword == "")
    {
        QMessageBox::warning(this, "提示", "密码不能为空！");
        return;
    }

    switch(state)
    {
    default:
    {
        break;
    }
    case QTcpSocket::SocketState::ConnectedState:
    {

//加密
        m_tcpSocket->write(m_encrypt.XOR(QString("!BEGIN!").toLatin1(), CEncrypt::Model_XOR));
        m_tcpSocket->write(m_encrypt.XOR(QString("@LOGINACCOUNT@").toLatin1(), CEncrypt::Model_XOR));
        m_tcpSocket->write(m_encrypt.XOR(QString("<{").toLatin1(), CEncrypt::Model_XOR));
        m_tcpSocket->write(m_encrypt.XOR(editAccount.toLatin1(), CEncrypt::Model_XOR));
        m_tcpSocket->write(m_encrypt.XOR(QString("}>").toLatin1(), CEncrypt::Model_XOR));
        m_tcpSocket->write(m_encrypt.XOR(QString("@LOGINPASSWORD@").toLatin1(), CEncrypt::Model_XOR));
        m_tcpSocket->write(m_encrypt.XOR(QString("<{").toLatin1(), CEncrypt::Model_XOR));
        m_tcpSocket->write(m_encrypt.XOR(editPassword.toLatin1(), CEncrypt::Model_XOR));
        m_tcpSocket->write(m_encrypt.XOR(QString("}>").toLatin1(), CEncrypt::Model_XOR));
        m_tcpSocket->write(m_encrypt.XOR(QString("!END!").toLatin1(), CEncrypt::Model_XOR));

//未加密
//            m_tcpSocket->write(QString("!BEGIN!").toLatin1());
//            m_tcpSocket->write(QString("@LOGINACCOUNT@").toLatin1());
//            m_tcpSocket->write(QString("<{").toLatin1());
//            m_tcpSocket->write(editAccount.toLatin1());
//            m_tcpSocket->write(QString("}>").toLatin1());
//            m_tcpSocket->write(QString("@LOGINPASSWORD@").toLatin1());
//            m_tcpSocket->write(QString("<{").toLatin1());
//            m_tcpSocket->write(editPassword.toLatin1());
//            m_tcpSocket->write(QString("}>").toLatin1());
//            m_tcpSocket->write(QString("!END!").toLatin1());

        if(m_tcpSocket->waitForReadyRead(3000))
        {
            QByteArray readData = m_tcpSocket->readAll();
            qDebug() << readData.data();
        }
        else
        {
            qDebug() << "接受信息失败";
        }
        break;
    }
    case QTcpSocket::SocketState::UnconnectedState:
    {
        QFileInfoList usersFileInfoList = m_dirUsersEnc.entryInfoList(QDir::Files | QDir::CaseSensitive);     //过滤条件为只限文件并区分大小写
        for (int i = 0; i < usersFileInfoList.size(); ++i)
        {
            QString suffix = usersFileInfoList[i].suffix();     //获取后缀名
            if (suffix == "enc")
            {
                m_listUsers.append(usersFileInfoList[i].baseName());
            }
        }

        for(int i = 0; i < m_listUsers.size(); ++i)
        {
            if(m_listUsers[i] == editAccount)
            {
                QString fileCode;
                QByteArray account, password, question1, answer1, question2, answer2;
                QFile file_user_enc(m_path_users_enc + m_listUsers[i] + ".enc");
                QDataStream outputStream(&file_user_enc);
                file_user_enc.open(QIODevice::ReadOnly);
                outputStream >> fileCode >> account >> password >> question1 >> answer1 >> question2 >> answer2;
                file_user_enc.close();
                if(editPassword == m_encrypt.XOR(password, CEncrypt::Model_XOR).data())
                {
                    ui->lineEditAccount->setText("");
                    ui->lineEditPassword->setText("");
                    emit login(editAccount);
                    return;
                }
                else
                {
                    QMessageBox::warning(this, "提示", "密码错误！");
                    return;
                }
            }
        }
        QMessageBox::warning(this, "提示", "账号错误！");
        break;
    }
    }
}
