#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_content.h"

#include "controls.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();
    ui->labelModels->focus();
    ui->widget->openModelList();
    connect(ui->labelModels, &NavLabel::clicked, ui->widget, [&]() {
        ui->labelModels->focus();
        ui->widget->openModelList();
    });
    connect(ui->labelLights, &NavLabel::clicked, ui->widget, [&]() {
        ui->labelLights->focus();
        ui->widget->openLightList();
    });
    connect(ui->widget->ui->modelWidget, &NavDockWidget::closed, this, [&]() {
        ui->labelModels->blur();
    });
    connect(ui->widget->ui->lightWidget, &NavDockWidget::closed, this, [&]() {
        ui->labelLights->blur();
    });
    connect(ui->save, &QAction::triggered, this, [&](bool) {
        QString filename = QFileDialog::getSaveFileName(this, tr("保存文件"), ".", tr("json(*.json)"));
        auto m = ui->widget->ui->openGLWidget->getModelManager();
        m->save_json_file(filename);

    });
    connect(ui->open, &QAction::triggered, this, [&](bool) {
        QString filename = QFileDialog::getOpenFileName(this, tr("打开文件"), ".", tr("json(*.json)"));
        auto m = ui->widget->ui->openGLWidget->getModelManager();
        auto t = ui->widget->ui->openGLWidget->getTextureManager();
        m->load_json_file(filename, t);

    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

