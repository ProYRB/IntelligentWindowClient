/******************************
 *  Author  :   YangRongBao
 *  Date    :   2021.4
******************************/

#ifndef CREGISTERDIALOG_H
#define CREGISTERDIALOG_H

#include "cstring.h"
#include "cencrypt.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDialog>
#include <QDir>
#include <QDataStream>
#include <QFile>
#include <QLine>
#include <QLineEdit>
#include <QMessageBox>
#include <QRegExpValidator>
#include <QString>
#include <QStringList>

namespace Ui {
class CRegisterDialog;
}

class CRegisterDialog : public QDialog
{
    Q_OBJECT

signals:
    void registered(QString registerAccount, QString registerPassword);

public:
    explicit CRegisterDialog(QWidget *parent = nullptr);
    ~CRegisterDialog();

protected:

private:
    Ui::CRegisterDialog *ui;

    const QString m_fileCode = "#APQA";
    const QString m_path_users_enc = QCoreApplication::applicationDirPath() + "/system/users/";
    const QString m_path_users_dir = QCoreApplication::applicationDirPath() + "/users/";

    bool isNetwork = false;

    QDir m_dirUsersEnc;

    QStringList m_listUsers;

private slots:
    void clearLineEdits();
};

#endif // CREGISTERDIALOG_H
