/******************************
 *  Author  :   YangRongBao
 *  Date    :   2021.3
******************************/

#include "cproductinformationdialog.h"
#include "ui_cproductinformationdialog.h"

CProductInformationDialog::CProductInformationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CProductInformationDialog)
{
    ui->setupUi(this);  //默认代码
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);   //可以取消窗口右上角的"?"【其他窗口不一定适应】
}

CProductInformationDialog::~CProductInformationDialog()
{
    delete ui;
}
