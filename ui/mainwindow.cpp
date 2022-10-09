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
        QString filename = QFileDialog::getSaveFileName(this, 
            QString::fromLocal8Bit("保存文件"), ".", tr("json(*.json)"));
        auto m = ui->widget->ui->openGLWidget->getModelManager();
        auto t = ui->widget->ui->openGLWidget->getTextureManager();
        m->save_json_file(filename, t);
    });
    connect(ui->open, &QAction::triggered, this, [&](bool) {
        QString filename = QFileDialog::getOpenFileName(this, 
            QString::fromLocal8Bit("打开文件"), ".", tr("json(*.json)"));
        auto m = ui->widget->ui->openGLWidget->getModelManager();
        auto t = ui->widget->ui->openGLWidget->getTextureManager();
        t->setCurrentDir(QFileInfo(filename).absoluteDir());
        m->load_json_file(filename, t);
        ui->widget->updateModelList();
        ui->widget->updateLightList();
        ui->widget->ui->openGLWidget->rebindBuffer();
        ui->widget->ui->openGLWidget->update();
    });
    connect(ui->frontView, &QAction::triggered, this, [&](bool) {
        ui->widget->ui->openGLWidget->getCamera()->frontView();
        ui->widget->ui->openGLWidget->update();
    });
    connect(ui->backView, &QAction::triggered, this, [&](bool) {
        ui->widget->ui->openGLWidget->getCamera()->backView();
        ui->widget->ui->openGLWidget->update();
    });
    connect(ui->leftView, &QAction::triggered, this, [&](bool) {
        ui->widget->ui->openGLWidget->getCamera()->leftView();
        ui->widget->ui->openGLWidget->update();
    });
    connect(ui->rightView, &QAction::triggered, this, [&](bool) {
        ui->widget->ui->openGLWidget->getCamera()->rightView();
        ui->widget->ui->openGLWidget->update();
    });
    connect(ui->topView, &QAction::triggered, this, [&](bool) {
        ui->widget->ui->openGLWidget->getCamera()->topView();
        ui->widget->ui->openGLWidget->update();
    });
    connect(ui->bottomView, &QAction::triggered, this, [&](bool) {
        ui->widget->ui->openGLWidget->getCamera()->bottomView();
        ui->widget->ui->openGLWidget->update();
    });
    connect(ui->axis, &QAction::triggered, this, [&](bool checked) {
        ui->widget->ui->openGLWidget->showAxis(checked);
        ui->widget->ui->openGLWidget->update();
    });
    connect(ui->plane, &QAction::triggered, this, [&](bool checked) {
        ui->widget->ui->openGLWidget->showPlane(checked);
        ui->widget->ui->openGLWidget->update();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

