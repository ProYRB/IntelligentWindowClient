/******************************
 *  Author  :   YangRongBao
 *  Date    :   2021.4
******************************/

#include "cmodifysecretivequestionsdialog.h"
#include "ui_cmodifysecretivequestionsdialog.h"

CModifySecretiveQuestionsDialog::CModifySecretiveQuestionsDialog(const QString account, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CModifySecretiveQuestionsDialog)
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
    connect(ui->lineEditPassword, &QLineEdit::textChanged, [&](const QString &lineEditString){
        CString checkString(lineEditString);
//        if(checkString.CheckChar(checkString.GetStringSize() - 1, CString::Model_Password) == CString::Error_NullChar){
//            if(checkString.DeleteChar(checkString.GetStringSize() - 1) == CString::Error_None)
//            {
//                ui->lineEditPassword->setText(checkString.GetQString());
//            }
//            else
//            {
//                ui->lineEditPassword->clear();
//            }
//            QMessageBox::warning(this, "提示", "密码只能包含阿拉伯数字、大小写字母以及一部分特殊符号！");
//        }
    });

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
            QString linePassword = ui->lineEditPassword->text();
            QString secretiveQuestion1 = ui->lineEditSecretiveQuestion1->text();
            QString secretiveAnswer1 = ui->lineEditSecretiveAnswer1->text();
            QString secretiveQuestion2 = ui->lineEditSecretiveQuestion2->text();
            QString secretiveAnswer2 = ui->lineEditSecretiveAnswer2->text();

            if(linePassword == "")
            {
                QMessageBox::warning(this, "提示", "密码不能为空！");
                return;
            }
            if(secretiveQuestion1 == "" || secretiveQuestion2 == "")
            {
                QMessageBox::warning(this, "提示", "秘问不能为空！");
                return;
            }
            if(secretiveAnswer1 == "" || secretiveAnswer2 == "")
            {
                QMessageBox::warning(this, "提示", "答案不能为空！");
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
                    if(linePassword != myencrypt.XOR(userPassword, CEncrypt::Model_XOR).data())
                    {
                        QMessageBox::warning(this, "提示", "密码输入错误！");
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
                                    << userPassword
                                    << myencrypt.XOR(secretiveQuestion1.toLatin1(), CEncrypt::Model_XOR)
                                    << myencrypt.XOR(secretiveAnswer1.toLatin1(), CEncrypt::Model_XOR)
                                    << myencrypt.XOR(secretiveQuestion2.toLatin1(), CEncrypt::Model_XOR)
                                    << myencrypt.XOR(secretiveAnswer2.toLatin1(), CEncrypt::Model_XOR);
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

CModifySecretiveQuestionsDialog::~CModifySecretiveQuestionsDialog()
{
    delete ui;
}
