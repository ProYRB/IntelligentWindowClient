/******************************
 *  Author  :   YangRongBao
 *  Date    :   2021.3
******************************/

#ifndef CREGISTERDIALOG_H
#define CREGISTERDIALOG_H

#include "cstring.h"
#include "cencrypt.h"

#include <QDir>
#include <QLine>
#include <QFile>
#include <QDebug>
#include <QDialog>
#include <QString>
#include <QLineEdit>
#include <QStringList>
#include <QDataStream>
#include <QMessageBox>
#include <QCoreApplication>

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
