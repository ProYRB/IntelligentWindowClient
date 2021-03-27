/******************************
 *  Author  :   YangRongBao
 *  Date    :   2021.3
******************************/

#include "cregisterdialog.h"
#include "ui_cregisterdialog.h"

CRegisterDialog::CRegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CRegisterDialog)
{
    ui->setupUi(this);  //默认代码

    /**************************************************
     * 代码区块【文件系统】【开始】
     */

    m_dirUsersEnc.setPath(m_path_users_enc);
    if(!m_dirUsersEnc.exists())
    {
        m_dirUsersEnc.mkpath(m_path_users_enc);
    }

    /**
     * 代码区块【文件系统】【结束】
     **************************************************/

    /**************************************************
     * 代码区块【输入框】【开始】
     */

    connect(ui->lineEditAccount, &QLineEdit::textChanged, [&](const QString &lineEditString){
        CString checkString(lineEditString);
        if(checkString.CheckChar(checkString.GetStringSize() - 1, CString::Model_Account) == CString::Error_CharOverRange){
            if(checkString.DeleteChar(checkString.GetStringSize() - 1) == CString::Error_None)
            {
                ui->lineEditAccount->setText(checkString.GetString());
            }
            else
            {
                ui->lineEditAccount->clear();
            }
            QMessageBox::warning(this, "提示", "账号只能包含阿拉伯数字和大小写字母！");
        }
    });
    connect(ui->lineEditPassword, &QLineEdit::textChanged, [&](const QString &lineEditString){
        CString checkString(lineEditString);
        if(checkString.CheckChar(checkString.GetStringSize() - 1, CString::Model_Password) == CString::Error_CharOverRange){
            if(checkString.DeleteChar(checkString.GetStringSize() - 1) == CString::Error_None)
            {
                ui->lineEditPassword->setText(checkString.GetString());
            }
            else
            {
                ui->lineEditPassword->clear();
            }
            QMessageBox::warning(this, "提示", "密码只能包含阿拉伯数字、大小写字母以及一部分特殊符号！");
        }
    });
    connect(ui->lineEditPasswordAgain, &QLineEdit::textChanged, [&](const QString &lineEditString){
        CString checkString(lineEditString);
        if(checkString.CheckChar(checkString.GetStringSize() - 1, CString::Model_Password) == CString::Error_CharOverRange){
            if(checkString.DeleteChar(checkString.GetStringSize() - 1) == CString::Error_None)
            {
                ui->lineEditPasswordAgain->setText(checkString.GetString());
            }
            else
            {
                ui->lineEditPasswordAgain->clear();
            }
            QMessageBox::warning(this, "提示", "密码只能包含阿拉伯数字、大小写字母以及一部分特殊符号！");
        }
    });

    /**
     * 代码区块【输入框】【结束】
     **************************************************/

    /**************************************************
     * 代码区块【注册】【开始】
     */

    connect(ui->toolButtonRegister, &QToolButton::clicked, [&](){
        if(isNetwork)
        {
            ;
        }
        else
        {
            QString registerAccount = ui->lineEditAccount->text();
            QString registerPassword = ui->lineEditPassword->text();
            QString registerPasswordAgain = ui->lineEditPasswordAgain->text();
            QString registerSecretiveQuestion1 = ui->lineEditSecretiveQuestion1->text();
            QString registerSecretiveAnswer1 = ui->lineEditSecretiveAnswer1->text();
            QString registerSecretiveQuestion2 = ui->lineEditSecretiveQuestion2->text();
            QString registerSecretiveAnswer2 = ui->lineEditSecretiveAnswer2->text();

            if(registerAccount == "")
            {
                QMessageBox::warning(this, "提示", "账号不能为空！");
                return;
            }
            if(registerPassword == "" || registerPasswordAgain == "")
            {
                QMessageBox::warning(this, "提示", "密码不能为空！");
                return;
            }
            if(registerPassword != registerPasswordAgain)
            {
                QMessageBox::warning(this, "提示", "两次密码输入不一致！");
                return;
            }
            if(registerSecretiveQuestion1 == "" || registerSecretiveQuestion2 == "")
            {
                QMessageBox::warning(this, "提示", "安全问题不能为空！");
                return;
            }
            if(registerSecretiveAnswer1 == "" || registerSecretiveAnswer2 == "")
            {
                QMessageBox::warning(this, "提示", "问题答案不能为空！");
                return;
            }

            QFileInfoList usersFilesInfoList = m_dirUsersEnc.entryInfoList(QDir::Files | QDir::CaseSensitive);         //过滤条件为只限文件并区分大小写

            for (int i = 0; i < usersFilesInfoList.size(); ++i)
            {
                QString fileSuffix = usersFilesInfoList[i].suffix();        //获取后缀名
                if (fileSuffix == "enc")
                {
                    m_listUsers.append(usersFilesInfoList[i].baseName());
                }
            }
            for(int i = 0; i < m_listUsers.size(); ++i)
            {
                if(m_listUsers[i] == registerAccount)
                {
                    QMessageBox::warning(this, "提示", "账号已存在！");
                    return;
                }
            }

            QFile file_user_enc(m_path_users_enc + registerAccount + ".enc");
            file_user_enc.open(QIODevice::NewOnly);
            file_user_enc.close();
            file_user_enc.open(QIODevice::WriteOnly);

            QDataStream inputStream(&file_user_enc);
            CEncrypt encrypt;
            inputStream << m_fileCode
                        << encrypt.XOR(registerAccount.toLatin1(), CEncrypt::Model_XOR)
                        << encrypt.XOR(registerPassword.toLatin1(), CEncrypt::Model_XOR)
                        << encrypt.XOR(registerSecretiveQuestion1.toLatin1(), CEncrypt::Model_XOR)
                        << encrypt.XOR(registerSecretiveAnswer1.toLatin1(), CEncrypt::Model_XOR)
                        << encrypt.XOR(registerSecretiveQuestion2.toLatin1(), CEncrypt::Model_XOR)
                        << encrypt.XOR(registerSecretiveAnswer2.toLatin1(), CEncrypt::Model_XOR);
            file_user_enc.close();

            m_dirUsersEnc.setPath(m_path_users_dir + registerAccount);
            if(m_dirUsersEnc.exists())
            {
                m_dirUsersEnc.removeRecursively();
                m_dirUsersEnc.mkpath(m_path_users_dir + registerAccount);
            }
            else
            {
                m_dirUsersEnc.mkpath(m_path_users_dir + registerAccount);
            }

            QMessageBox::information(this, "提示", "注册成功！");
            emit registered(registerAccount, registerPassword);
        }
    });

    connect(this, &CRegisterDialog::registered, this, &CRegisterDialog::clearLineEdits);

    /**
     * 代码区块【注册】【结束】
     **************************************************/
}

CRegisterDialog::~CRegisterDialog()
{
    delete ui;
}

void CRegisterDialog::clearLineEdits()
{
    ui->lineEditAccount->clear();
    ui->lineEditPassword->clear();
    ui->lineEditPasswordAgain->clear();
    ui->lineEditSecretiveQuestion1->clear();
    ui->lineEditSecretiveAnswer1->clear();
    ui->lineEditSecretiveQuestion2->clear();
    ui->lineEditSecretiveAnswer2->clear();
}
