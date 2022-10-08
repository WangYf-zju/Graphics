#include "propertieswidget.h"
#include "ui_modelpropwidget.h"
#include "ui_lightpropwidget.h"

ModelPropWidget::ModelPropWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ModelPropWidget)
{
    ui->setupUi(this);
    connect(ui->editName, &QLineEdit::textChanged, this, [&](QString name) {
        modelTemp.name = name.toStdString();
        emit modelChanged(modelTemp);
    });
    connect(ui->btnTrans, &QPushButton::clicked, this, [&]() {});
    connect(ui->spinColorR, &QSpinBox::textChanged, this, [&](QString) { changeColor(); });
    connect(ui->spinColorG, &QSpinBox::textChanged, this, [&](QString) { changeColor(); });
    connect(ui->spinColorB, &QSpinBox::textChanged, this, [&](QString) { changeColor(); });
    connect(ui->spinAlpha, &QDoubleSpinBox::textChanged, this, [&](QString) { changeColor(); });
    connect(ui->btnTexture, &QPushButton::clicked, this, [&]() {
        QFileInfo fi(QString::fromStdString(modelTemp.texture));
        QString file = QFileDialog::getOpenFileName(
            this, 
            QString::fromLocal8Bit("Ñ¡ÔñÎÆÀíÍ¼Ïñ"), 
            fi.absoluteDir().absolutePath(), 
            "Images (*.png *.jpg)");
        if (file != "")
        {
            emit textureChanged(modelTemp.id, file);
            ui->editTexture->setText(file);
        }
    });
    connect(ui->specular, &QSlider::valueChanged, this, [&](int) {
        modelTemp.specular = (float)ui->specular->value() / 100.0f;
        ui->labelSpecular->setText(QString::number(modelTemp.specular, 'f', 2));
        emit modelChanged(modelTemp);
    });
    connect(ui->checkVisible, &QCheckBox::stateChanged, this, [&](int) {
        modelTemp.visible = ui->checkVisible->isChecked();
        emit modelChanged(modelTemp);
    });
}

ModelPropWidget::~ModelPropWidget()
{
    delete ui;
}


void ModelPropWidget::setModel(ModelObject * model)
{
    blockSignals(true);
    if (model != nullptr)
    {
        this->show();
        this->modelTemp = *model;
        glm::vec4 color = model->color;
        ui->editName->setText(QString::fromStdString(model->name));
        int r = (int)(color[0] * 255.0);
        int g = (int)(color[1] * 255.0);
        int b = (int)(color[2] * 255.0);
        ui->spinColorR->setValue(r);
        ui->spinColorG->setValue(g);
        ui->spinColorB->setValue(b);
        ui->spinAlpha->setValue(color[3]);
        colorPreview(r, g, b);
        ui->editTexture->setText(QString::fromStdString(model->texture));
        ui->checkVisible->setChecked(model->visible);
        ui->specular->setValue(model->specular * 100);
        ui->labelSpecular->setText(QString::number(model->specular, 'f', 2));
    }
    else
    {
        this->hide();
    }
    blockSignals(false);
}

void ModelPropWidget::updateTexture(int index, QString filepath)
{
    modelTemp.textureIndex = index;
    modelTemp.texture = filepath.toStdString();
}


void ModelPropWidget::colorPreview(int r, int g, int b)
{
    ui->labelColor->setStyleSheet(
        QString().sprintf("background-color:rgb(%d,%d,%d);", r, g, b));
}


void ModelPropWidget::changeColor()
{
    int r = ui->spinColorR->value();
    int g = ui->spinColorG->value();
    int b = ui->spinColorB->value();
    float a = (float)(ui->spinAlpha->value());
    colorPreview(r, g, b);
    modelTemp.color = glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, a);
    emit modelChanged(modelTemp);
}


void ModelPropWidget::blockSignals(bool b)
{
    ui->editName->blockSignals(b);
    ui->spinColorR->blockSignals(b);
    ui->spinColorG->blockSignals(b);
    ui->spinColorB->blockSignals(b);
    ui->spinAlpha->blockSignals(b);
    ui->editTexture->blockSignals(b);
    ui->checkVisible->blockSignals(b);
}


LightPropWidget::LightPropWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LightPropWidget)
{
    ui->setupUi(this);
    connect(ui->X, &QDoubleSpinBox::textChanged, this, [&](QString) {
        lightTemp.position.x = ui->X->value();
        emit lightChanged(lightTemp);
    });
    connect(ui->Y, &QDoubleSpinBox::textChanged, this, [&](QString) {
        lightTemp.position.y = ui->Y->value();
        emit lightChanged(lightTemp);
    });
    connect(ui->Z, &QDoubleSpinBox::textChanged, this, [&](QString) {
        lightTemp.position.z = ui->Z->value();
        emit lightChanged(lightTemp);
    });
    connect(ui->direX, &QDoubleSpinBox::textChanged, this, [&](QString) {
        lightTemp.direction.x = ui->direX->value();
        emit lightChanged(lightTemp);
    });
    connect(ui->direY, &QDoubleSpinBox::textChanged, this, [&](QString) {
        lightTemp.direction.y = ui->direY->value();
        emit lightChanged(lightTemp);
    });
    connect(ui->direZ, &QDoubleSpinBox::textChanged, this, [&](QString) {
        lightTemp.direction.z = ui->direZ->value();
        emit lightChanged(lightTemp);
    });
    connect(ui->R, &QSpinBox::textChanged, this, [&](QString) {
        lightTemp.ambient.x = (float)ui->R->value() / 255.0f;
        emit lightChanged(lightTemp);
    });
    connect(ui->G, &QSpinBox::textChanged, this, [&](QString) {
        lightTemp.ambient.y = (float)ui->G->value() / 255.0f;
        emit lightChanged(lightTemp);
    });
    connect(ui->B, &QSpinBox::textChanged, this, [&](QString) {
        lightTemp.ambient.z = (float)ui->B->value() / 255.0f;
        emit lightChanged(lightTemp);
    });
    connect(ui->Kq, &QDoubleSpinBox::textChanged, this, [&](QString) {
        lightTemp.attenuation.x = ui->Kq->value();
        emit lightChanged(lightTemp);
    });
    connect(ui->Kl, &QDoubleSpinBox::textChanged, this, [&](QString) {
        lightTemp.attenuation.y = ui->Kl->value();
        emit lightChanged(lightTemp);
    });
    connect(ui->Kc, &QDoubleSpinBox::textChanged, this, [&](QString) {
        lightTemp.attenuation.z = ui->Kc->value();
        emit lightChanged(lightTemp);
    });
    connect(ui->cutOff, &QDoubleSpinBox::textChanged, this, [&](QString) {
        lightTemp.cutOff = ui->cutOff->value();
        emit lightChanged(lightTemp);
    });
    connect(ui->outerCutOff, &QDoubleSpinBox::textChanged, this, [&](QString) {
        lightTemp.outerCutOff = ui->outerCutOff->value();
        emit lightChanged(lightTemp);
    });
    connect(ui->checkEnable, &QCheckBox::stateChanged, this, [&](int) {
        lightTemp.isEnable = ui->checkEnable->isChecked();
        emit lightChanged(lightTemp);
    });
    connect(ui->checkVisible, &QCheckBox::stateChanged, this, [&](int) {
        lightTemp.visible = ui->checkVisible->isChecked();
        emit lightChanged(lightTemp);
    });
    connect(ui->diffuse, &QSlider::valueChanged, this, [&](int) {
        lightTemp.diffuse = (float)ui->diffuse->value() / 100.0f;
        ui->labelDiffuse->setText(QString::number(lightTemp.diffuse, 'f', 2));
        emit lightChanged(lightTemp);
    });
    connect(ui->specular, &QSlider::valueChanged, this, [&](int) {
        lightTemp.specular = (float)ui->specular->value() / 100.0f;
        ui->labelSpecular->setText(QString::number(lightTemp.specular, 'f', 2));
        emit lightChanged(lightTemp);
    });
}

void LightPropWidget::setLight(LightObject * light)
{
    blockSignals(true);
    if (light != nullptr)
    {
        ui->checkEnable->setChecked(light->isEnable);
        ui->X->setValue(light->position.x);
        ui->Y->setValue(light->position.y);
        ui->Z->setValue(light->position.z);
        ui->direX->setValue(light->direction.x);
        ui->direY->setValue(light->direction.y);
        ui->direZ->setValue(light->direction.z);
        ui->R->setValue(light->ambient.x * 255);
        ui->G->setValue(light->ambient.y * 255);
        ui->B->setValue(light->ambient.z * 255);
        ui->Kq->setValue(light->attenuation.x);
        ui->Kl->setValue(light->attenuation.y);
        ui->Kc->setValue(light->attenuation.z);
        ui->diffuse->setValue(light->diffuse * 100);
        ui->specular->setValue(light->specular * 100);
        ui->labelDiffuse->setText(QString::number(light->diffuse, 'f', 2));
        ui->labelSpecular->setText(QString::number(light->specular, 'f', 2));
        ui->cutOff->setValue(light->cutOff);
        ui->outerCutOff->setValue(light->outerCutOff);
        ui->checkVisible->setChecked(light->visible);
        if (light->type == DIRECTIONAL_LIGHT)
            setDirectionalLightMode();
        else if (light->type == POINT_LIGHT)
            setPointLightMode();
        else if (light->type == SPOT_LIGHT)
            setSpotLightMode();
        lightTemp = *light;
    }
    blockSignals(false);
}

void LightPropWidget::blockSignals(bool b)
{
    ui->checkEnable->blockSignals(b);
    ui->X->blockSignals(b);
    ui->Y->blockSignals(b);
    ui->Z->blockSignals(b);
    ui->direX->blockSignals(b);
    ui->direY->blockSignals(b);
    ui->direZ->blockSignals(b);
    ui->R->blockSignals(b);
    ui->G->blockSignals(b);
    ui->B->blockSignals(b);
    ui->diffuse->blockSignals(b);
    ui->specular->blockSignals(b);
    ui->Kq->blockSignals(b);
    ui->Kl->blockSignals(b);
    ui->Kc->blockSignals(b);
    ui->cutOff->blockSignals(b);
    ui->outerCutOff->blockSignals(b);
    ui->checkVisible->blockSignals(b);
}

void LightPropWidget::setDirectionalLightMode()
{
    ui->X->setDisabled(true);
    ui->Y->setDisabled(true);
    ui->Z->setDisabled(true);
    ui->direX->setDisabled(false);
    ui->direY->setDisabled(false);
    ui->direZ->setDisabled(false);
    ui->Kq->setDisabled(true);
    ui->Kl->setDisabled(true);
    ui->Kc->setDisabled(true);
    ui->cutOff->setDisabled(true);
    ui->outerCutOff->setDisabled(true);
}

void LightPropWidget::setPointLightMode()
{
    ui->X->setDisabled(false);
    ui->Y->setDisabled(false);
    ui->Z->setDisabled(false);
    ui->direX->setDisabled(true);
    ui->direY->setDisabled(true);
    ui->direZ->setDisabled(true);
    ui->Kq->setDisabled(false);
    ui->Kl->setDisabled(false);
    ui->Kc->setDisabled(false);
    ui->cutOff->setDisabled(true);
    ui->outerCutOff->setDisabled(true);
}

void LightPropWidget::setSpotLightMode()
{
    ui->X->setDisabled(false);
    ui->Y->setDisabled(false);
    ui->Z->setDisabled(false);
    ui->direX->setDisabled(false);
    ui->direY->setDisabled(false);
    ui->direZ->setDisabled(false);
    ui->Kq->setDisabled(false);
    ui->Kl->setDisabled(false);
    ui->Kc->setDisabled(false);
    ui->cutOff->setDisabled(false);
    ui->outerCutOff->setDisabled(false);
}
