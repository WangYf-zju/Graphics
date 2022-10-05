#include "modeladddlg.h"
#include "ui_modeladddlg.h"

#define OK_TEXT QString::fromLocal8Bit("确认")
#define CANCEL_TEXT QString::fromLocal8Bit("取消")

ModelAddDlg::ModelAddDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModelAddDlg)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(OK_TEXT);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(CANCEL_TEXT);
}

ModelAddDlg::~ModelAddDlg()
{
    delete ui;
}
