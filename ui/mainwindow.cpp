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
}

MainWindow::~MainWindow()
{
    delete ui;
}

