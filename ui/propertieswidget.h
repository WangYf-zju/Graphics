#ifndef PROPERTIESWIDGET_H
#define PROPERTIESWIDGET_H

#include <QWidget>
#include <QSpinBox>
#include <QFileDialog>

#include "ModelManager.h"

namespace Ui {
class PropertiesWidget;
}

class PropertiesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PropertiesWidget(QWidget *parent = nullptr);
    ~PropertiesWidget();
    void setModel(ModelObject *);
    void updateTexture(int index, QString filepath);

signals:
    void modelChanged(ModelObject &);
signals:
    void textureChanged(unsigned int, QString);

private:
    Ui::PropertiesWidget *ui;
    ModelObject modelTemp;
    void colorPreview(int r, int g, int b);
    void changeColor();
    void blockSignals(bool b);
};

#endif // PROPERTIESWIDGET_H
