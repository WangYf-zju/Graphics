#ifndef ADDDLG_H
#define ADDDLG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>

namespace Ui {
class ModelAddDlg;
}

class ModelAddDlg : public QDialog
{
    friend class Content;
    Q_OBJECT

public:
    explicit ModelAddDlg(QWidget *parent = nullptr);
    ~ModelAddDlg();

private:
    Ui::ModelAddDlg *ui;
};

#endif // ADDDLG_H
