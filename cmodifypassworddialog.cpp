/******************************
 *  Author  :   YangRongBao
 *  Date    :   2021.4
******************************/

#include "cmodifypassworddialog.h"
#include "ui_cmodifypassworddialog.h"

CModifyPasswordDialog::CModifyPasswordDialog(QString account, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CModifyPasswordDialog)
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

    QString fileCode;
    QByteArray userAccount, userPassword, userQuestion1, userAnswer1, userQuestion2, userAnswer2;
    QFile file_user_enc(m_path_users_enc + account + ".enc");
    QDataStream readStream(&file_user_enc);
    file_user_enc.open(QIODevice::ReadOnly);
    readStream >> fileCode >> userAccount >> userPassword >> userQuestion1 >> userAnswer1 >> userQuestion2 >> userAnswer2;
    file_user_enc.close();

    /**
     * 代码区块【文件系统】【结束】
     **************************************************/

    /**************************************************
     * 代码区块【输入框】【开始】
     */

    CEncrypt encrypt;
    ui->lineEditSecretiveQuestion1->setText(encrypt.XOR(userQuestion1, CEncrypt::Model_XOR).data());
    ui->lineEditSecretiveQuestion2->setText(encrypt.XOR(userQuestion2, CEncrypt::Model_XOR).data());
    ui->lineEditNewPassword->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9-@#&/*+]{0,30}"), this));    //用正则表达式来限制输入

    /**
     * 代码区块【输入框】【结束】
     **************************************************/

    /**************************************************
     * 代码区块【修改】【开始】
     */

    connect(ui->toolButtonModify, &QToolButton::clicked, [=](){
        if(isNetwork)
        {
            ;
        }
        else
        {
            QString secretiveAnswer1 = ui->lineEditSecretiveAnswer1->text();
            QString secretiveAnswer2 = ui->lineEditSecretiveAnswer2->text();
            QString newPassword = ui->lineEditNewPassword->text();

            if(secretiveAnswer1 == "" || secretiveAnswer2 == "")
            {
                QMessageBox::warning(this, "提示", "答案不能为空！");
                return;
            }
            if(newPassword == "")
            {
                QMessageBox::warning(this, "提示", "密码不能为空！");
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
                if(m_listUsers[i] == account)
                {
                    CEncrypt myencrypt;
                    if(secretiveAnswer1 != myencrypt.XOR(userAnswer1, CEncrypt::Model_XOR).data())
                    {
                        QMessageBox::warning(this, "提示", "第一个秘问答案输入错误！");
                        return;
                    }
                    else if(secretiveAnswer2 != myencrypt.XOR(userAnswer2, CEncrypt::Model_XOR).data())
                    {
                        QMessageBox::warning(this, "提示", "第二个秘问答案输入错误！");
                        return;
                    }
                    else
                    {
                        QFile file_user_enc(m_path_users_enc + account + ".enc");
                        file_user_enc.remove();
                        file_user_enc.open(QIODevice::NewOnly);
                        file_user_enc.close();

                        QDataStream inputStream(&file_user_enc);
                        file_user_enc.open(QIODevice::WriteOnly);
                        inputStream << m_fileCode
                                    << userAccount
                                    << myencrypt.XOR(newPassword.toLatin1(), CEncrypt::Model_XOR)
                                    << userQuestion1
                                    << userAnswer1
                                    << userQuestion2
                                    << userAnswer2;
                        file_user_enc.close();

                        QMessageBox::information(this, "提示", "修改成功！");
                        emit modified();
                        return;
                    }
                }
                else
                {
                    QMessageBox::information(this, "提示", "修改失败！");
                    return;
                }
            }
        }
    });

    /**
     * 代码区块【修改】【结束】
     **************************************************/
}

CModifyPasswordDialog::~CModifyPasswordDialog()
{
    delete ui;
}
