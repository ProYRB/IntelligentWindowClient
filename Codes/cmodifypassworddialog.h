/******************************
 *  Author  :   YangRongBao
 *  Date    :   2021.4
******************************/

#ifndef CMODIFYPASSWORDDIALOG_H
#define CMODIFYPASSWORDDIALOG_H

#include "cstring.h"
#include "cencrypt.h"

#include <QCoreApplication>
#include <QDir>
#include <QDialog>
#include <QFile>
#include <QMessageBox>
#include <QRegExpValidator>
#include <QString>

namespace Ui {
class CModifyPasswordDialog;
}

class CModifyPasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CModifyPasswordDialog(const QString account, QWidget *parent = nullptr);
    ~CModifyPasswordDialog();

protected:

private:
    Ui::CModifyPasswordDialog *ui;

    const QString m_fileCode = "#APQA";
    const QString m_path_users_enc = QCoreApplication::applicationDirPath() + "/system/users/";
    const QString m_path_users_dir = QCoreApplication::applicationDirPath() + "/users/";

    bool isNetwork = false;

    QDir m_dirUsersEnc;

    QStringList m_listUsers;

signals:
    void modified();

};

#endif // CMODIFYPASSWORDDIALOG_H
