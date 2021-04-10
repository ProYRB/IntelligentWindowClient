/******************************
 *  Author  :   YangRongBao
 *  Date    :   2021.4
******************************/

#ifndef CRETRIEVEDIALOG_H
#define CRETRIEVEDIALOG_H

#include "cstring.h"
#include "cencrypt.h"

#include <QCoreApplication>
#include <QDataStream>
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QLineEdit>
#include <QMessageBox>
#include <QRegExpValidator>
#include <QStringList>

namespace Ui {
class CRetrieveDialog;
}

class CRetrieveDialog : public QDialog
{
    Q_OBJECT

signals:
    void retrieved();

public:
    explicit CRetrieveDialog(QWidget *parent = nullptr);
    ~CRetrieveDialog();

protected:

private:
    Ui::CRetrieveDialog *ui;

    const QString m_fileCode = "#APQA";
    const QString m_path_users_enc = QCoreApplication::applicationDirPath() + "/system/users/";
    const QString m_path_users_dir = QCoreApplication::applicationDirPath() + "/users/";

    bool isNetwork = false;

    QString m_password;
    QString m_answer1;
    QString m_answer2;

    QStringList m_listUsers;

private slots:
    void clearLineEdits();
};

#endif // CRETRIEVEDIALOG_H
