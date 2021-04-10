/******************************
 *  Author  :   YangRongBao
 *  Date    :   2021.4
******************************/

#ifndef CMODIFYSECRETIVEQUESTIONSDIALOG_H
#define CMODIFYSECRETIVEQUESTIONSDIALOG_H

#include "cstring.h"
#include "cencrypt.h"

#include <QCoreApplication>
#include <QDir>
#include <QDialog>
#include <QMessageBox>
#include <QRegExpValidator>

namespace Ui {
class CModifySecretiveQuestionsDialog;
}

class CModifySecretiveQuestionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CModifySecretiveQuestionsDialog(const QString account, QWidget *parent = nullptr);
    ~CModifySecretiveQuestionsDialog();

protected:

private:
    Ui::CModifySecretiveQuestionsDialog *ui;

    const QString m_fileCode = "#APQA";
    const QString m_path_users_enc = QCoreApplication::applicationDirPath() + "/system/users/";
    const QString m_path_users_dir = QCoreApplication::applicationDirPath() + "/users/";

    bool isNetwork = false;

    QDir m_dirUsersEnc;

    QStringList m_listUsers;

signals:
    void modified();

};

#endif // CMODIFYSECRETIVEQUESTIONSDIALOG_H
