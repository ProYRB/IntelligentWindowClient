/******************************
 *  Author  :   YangRongBao
 *  Date    :   2021.3
******************************/

#ifndef CPRODUCTINFORMATIONDIALOG_H
#define CPRODUCTINFORMATIONDIALOG_H

#include <QDialog>

namespace Ui {
class CProductInformationDialog;
}

class CProductInformationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CProductInformationDialog(QWidget *parent = nullptr);
    ~CProductInformationDialog();

private:
    Ui::CProductInformationDialog *ui;

    bool isNetwork = false;
};

#endif // CPRODUCTINFORMATIONDIALOG_H
