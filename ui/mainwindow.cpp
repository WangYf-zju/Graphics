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
    connect(ui->widget->ui->dockWidget, &ModelDockWidget::closed, this, [&]() {
        ui->labelModels->blur();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

