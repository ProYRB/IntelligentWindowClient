/******************************
 *  Author  :   YangRongBao
 *  Date    :   2021.4
******************************/

#include "cretrievedialog.h"
#include "ui_cretrievedialog.h"

CRetrieveDialog::CRetrieveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CRetrieveDialog)
{
    ui->setupUi(this);  //默认代码


    /** [LineEdits] */
    ui->lineEditAccount->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9]{0,20}"), this));    //用正则表达式来限制输入
    connect(ui->lineEditAccount, &QLineEdit::textChanged, [&](const QString &lineEditString)
    {
        QFile file_user_enc(m_path_users_enc + lineEditString + ".enc");
        if(file_user_enc.open(QIODevice::ReadOnly))
        {
            QString fileCode;
            QDataStream outputStream(&file_user_enc);
            QByteArray account, password, question1, answer1, question2, answer2;
            outputStream >> fileCode >> account >> password >> question1 >> answer1 >> question2 >> answer2;
            file_user_enc.close();

            CEncrypt encrypt;
            m_password = encrypt.XOR(password, CEncrypt::Model_XOR);
            m_answer1 = encrypt.XOR(answer1, CEncrypt::Model_XOR);
            m_answer2 = encrypt.XOR(answer2, CEncrypt::Model_XOR);
            ui->lineEditSecretiveQuestion1->setText(encrypt.XOR(question1, CEncrypt::Model_XOR).data());
            ui->lineEditSecretiveQuestion2->setText(encrypt.XOR(question2, CEncrypt::Model_XOR).data());
        }
        else
        {
            ui->lineEditSecretiveQuestion1->clear();
            ui->lineEditSecretiveQuestion2->clear();
        }
    });
    /* [LineEdits] **/


    /** [Retrieve] */
    connect(ui->toolButtonRetrieve, &QToolButton::clicked, [&]()
    {
        if(isNetwork)
        {
            ;
        }
        else
        {
            if(ui->lineEditAccount->text() == "")
            {
                QMessageBox::warning(this, "提示", "账号不能为空！");
                return;
            }
            if(ui->lineEditSecretiveAnswer1->text() == "" || ui->lineEditSecretiveAnswer2->text() == "")
            {
                QMessageBox::warning(this, "提示", "回答不能为空！");
                return;
            }
            if(ui->lineEditSecretiveAnswer1->text() == m_answer1 && ui->lineEditSecretiveAnswer2->text() == m_answer2)
            {
                QMessageBox::warning(this, "提示", "身份验证完成\n当前账号密码：" + m_password + "\n请牢记密码！");
                emit retrieved();
            }
            else
            {
                QMessageBox::warning(this, "提示", "答案错误！");
                return;
            }
        }
    });
    connect(this, &CRetrieveDialog::retrieved, this, &CRetrieveDialog::clearLineEdits);
    /* [Retrieve] **/
}

CRetrieveDialog::~CRetrieveDialog()
{
    delete ui;
}
void CRetrieveDialog::clearLineEdits()
{
    ui->lineEditAccount->clear();
    ui->lineEditSecretiveAnswer1->clear();
    ui->lineEditSecretiveAnswer2->clear();
}
