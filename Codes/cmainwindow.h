/******************************
 *  Author  :   YangRongBao
 *  Date    :   2021.3
******************************/

#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include "cmodifypassworddialog.h"
#include "cproductinformationdialog.h"
#include "cmodifysecretivequestionsdialog.h"

#include <QDebug>
#include <QTcpSocket>
#include <QMainWindow>
#include <QCoreApplication>

QT_BEGIN_NAMESPACE
namespace Ui { class CMainWindow; }
QT_END_NAMESPACE

class CMainWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void logout();

public:
    explicit CMainWindow(const QString account, QWidget *parent = nullptr);
    ~CMainWindow();

private:
    Ui::CMainWindow *ui;

    const QString *m_account = nullptr;

    const QString path_users_dir = QCoreApplication::applicationDirPath() + "/users/";

    bool isNetwork = false;

    CModifyPasswordDialog *m_modifyPasswordDialog = nullptr;
    CModifySecretiveQuestionsDialog *m_modifySecretiveQuestionsDialog = nullptr;

};

#endif // CMAINWINDOW_H
