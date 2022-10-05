#include "propertieswidget.h"
#include "ui_propertieswidget.h"

PropertiesWidget::PropertiesWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PropertiesWidget)
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
    connect(ui->checkVisible, &QCheckBox::stateChanged, this, [&](int) {
        modelTemp.visible = ui->checkVisible->isChecked();
        emit modelChanged(modelTemp);
    });
}

PropertiesWidget::~PropertiesWidget()
{
    delete ui;
}


void PropertiesWidget::setModel(ModelObject * model)
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
    }
    else
    {
        this->hide();
    }
    blockSignals(false);
}

void PropertiesWidget::updateTexture(int index, QString filepath)
{
    modelTemp.textureIndex = index;
    modelTemp.texture = filepath.toStdString();
}


void PropertiesWidget::colorPreview(int r, int g, int b)
{
    ui->labelColor->setStyleSheet(
        QString().sprintf("background-color:rgb(%d,%d,%d);", r, g, b));
}


void PropertiesWidget::changeColor()
{
    int r = ui->spinColorR->value();
    int g = ui->spinColorG->value();
    int b = ui->spinColorB->value();
    float a = (float)(ui->spinAlpha->value());
    colorPreview(r, g, b);
    modelTemp.color = glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, a);
    emit modelChanged(modelTemp);
}


void PropertiesWidget::blockSignals(bool b)
{
    ui->editName->blockSignals(b);
    ui->spinColorR->blockSignals(b);
    ui->spinColorG->blockSignals(b);
    ui->spinColorB->blockSignals(b);
    ui->spinAlpha->blockSignals(b);
    ui->editTexture->blockSignals(b);
    ui->checkVisible->blockSignals(b);
}
