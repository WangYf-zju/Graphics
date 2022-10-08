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
    friend class ModelPropWidget;
    Q_OBJECT

public:
    explicit ModelAddDlg(QWidget *parent = nullptr);
    ~ModelAddDlg();
    void setCubeMode(QString title = "");
    void setCylinderMode(QString title = "");
    void setSphereMode(QString title = "");

private:
    Ui::ModelAddDlg *ui;
};

#endif // ADDDLG_H
