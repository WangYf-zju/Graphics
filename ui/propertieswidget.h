#ifndef PROPERTIESWIDGET_H
#define PROPERTIESWIDGET_H

#include <QWidget>
#include <QSpinBox>
#include <QFileDialog>

#include "ModelManager.h"
#include "LightManager.h"
#include "modeladddlg.h"

namespace Ui {
class ModelPropWidget;
class LightPropWidget;
}

class ModelPropWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ModelPropWidget(QWidget *parent = nullptr);
    ~ModelPropWidget();
    void setModel(ModelObject *);
    void updateTexture(int index, QString filepath);

signals:
    void modelChanged(ModelObject &);
signals:
    void textureChanged(unsigned int, QString);

private:
    Ui::ModelPropWidget *ui;
    ModelObject modelTemp;
    ModelAddDlg transDlg;
    void colorPreview(int r, int g, int b);
    void changeColor();
    void blockSignals(bool b);
    glm::mat4 calcModelMatrix(float *);
};

class LightPropWidget : public QWidget
{
    Q_OBJECT

public:
    LightPropWidget(QWidget *parent = nullptr);
    void setLight(LightObject *);
signals:
    void lightChanged(LightObject &);

private:
    Ui::LightPropWidget *ui;
    LightObject lightTemp;
    void blockSignals(bool b);
    void setDirectionalLightMode();
    void setPointLightMode();
    void setSpotLightMode();
};

#endif // PROPERTIESWIDGET_H
