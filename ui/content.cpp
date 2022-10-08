#include "content.h"
#include "ui_content.h"
#include "ui_modeladddlg.h"

#include <QDebug>

#define MODEL_CUBE_ICON QIcon(":/icons/icons/cube.ico")
#define MODEL_CYLINDER_ICON QIcon(":/icons/icons/cylinder.ico")
#define MODEL_SPHERE_ICON QIcon(":/icons/icons/sphere.ico")
#define LIGHT_POINT_ICON QIcon(":/icons/icons/sun.ico")
#define LIGHT_SPOT_ICON QIcon(":/icons/icons/torch.ico")
#define ADD_TEXT QString::fromLocal8Bit("添加")
#define MODEL_CUBE_TEXT QString::fromLocal8Bit("长方体")
#define MODEL_CYLINDER_TEXT QString::fromLocal8Bit("圆柱")
#define MODEL_SPHERE_TEXT QString::fromLocal8Bit("球体")
#define LIGHT_DIRECTIONAL_TEXT QString::fromLocal8Bit("全局照明")
#define LIGHT_POINT_TEXT QString::fromLocal8Bit("点光源")
#define LIGHT_SPOT_TEXT QString::fromLocal8Bit("聚光灯")
#define LIST_ROW_HEIGHT 20

Content::Content(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Content)
    , curModelAction(nullptr)
    , modelAddDlg(this)
{
    ui->setupUi(this);
    initControls();
    connect(ui->toolModel, &QToolButton::released, this, [&]() {
        showAddModelDlg(modelMenuAction[curModelAction]);
    });
    connect(ui->toolLight, &QToolButton::released, this, [&]() { addLight(); });
    connect(&modelMenu, &QMenu::triggered, this, [&](QAction * action) {
        if (curModelAction) curModelAction->setChecked(false);
        ui->toolModel->setIcon(action->icon());
        curModelAction = action;
        showAddModelDlg(modelMenuAction[action]);
    });
    connect(&lightMenu, &QMenu::triggered, this, [&](QAction * action) {
        if (curLightAction) curLightAction->setChecked(false);
        ui->toolLight->setIcon(action->icon());
        curLightAction = action;
        addLight();
    });
    connect(modelAddDlg.ui->buttonBox->button(QDialogButtonBox::Ok),
        &QPushButton::clicked, this, [&]() {
            addModel();
            modelAddDlg.close();
    });
    connect(modelAddDlg.ui->buttonBox->button(QDialogButtonBox::Cancel),
        &QPushButton::clicked, this, [&]() { modelAddDlg.close(); });
    connect(ui->modelList->selectionModel(), &QItemSelectionModel::selectionChanged, this,
        [&](const QItemSelection &, const QItemSelection &) {
        auto selections = ui->modelList->selectionModel()->selectedRows();
        auto models = ui->openGLWidget->getModelVector();
        for (int i = 0; i < models->size(); i++)
            models->at(i).select = false;
        for (auto p = selections.begin(); p != selections.end(); p++)
            models->at(p->row()).select = true;
        ui->openGLWidget->update();
        showModelInfo();
    });
    connect(ui->lightList->selectionModel(), &QItemSelectionModel::selectionChanged, this,
        [&](const QItemSelection &, const QItemSelection &) {
        auto selections = ui->lightList->selectionModel()->selectedRows();
        auto manager = ui->openGLWidget->getLightManager();
        for (int i = 0; i < manager->lightCount(); i++)
            manager->getLight(i)->isSelect = false;
        for (auto it = selections.begin(); it != selections.end(); it++)
            manager->getLight(it->row())->isSelect = true;
        ui->openGLWidget->update();
        showLightInfo();
    });
    connect(ui->modelProp, &ModelPropWidget::modelChanged, this, [&](ModelObject & model) {
        ModelObject * oldModel = ui->openGLWidget->getModelById(model.id);
        if (oldModel != nullptr)
        {
            *(oldModel) = model;
            ui->openGLWidget->update();
        }
    });
    connect(ui->modelProp, &ModelPropWidget::textureChanged, this, 
        [&](unsigned int id, QString filepath) {
        auto texture = ui->openGLWidget->addTexture(filepath);
        filepath = texture.first;
        int index = texture.second;
        if (filepath != "")
        {
            ui->openGLWidget->changeModelTexture(id, index, filepath.toStdString());
            ui->openGLWidget->update();
            ui->modelProp->updateTexture(index, filepath);
        }
    });
    connect(ui->lightProp, &LightPropWidget::lightChanged, this, [&](LightObject & light) {
        auto manager = ui->openGLWidget->getLightManager();
        auto selections = ui->lightList->selectionModel()->selectedRows();
        if (selections.size() > 0)
        {
            int row = (selections.end() - 1)->row();
            manager->updateLight(light, row);
            ui->openGLWidget->update();
        }
    });
    connect(ui->modelList, &ColumnList::deleteKeyPressed, this, [&]() {
        auto selections = ui->modelList->selectionModel()->selectedRows();
        auto manager = ui->openGLWidget->getModelManager();
        if (selections.size() > 0 && QMessageBox::information(this,
            QString::fromLocal8Bit("删除模型"),
            QString::fromLocal8Bit("确认删除选中模型"),
            QMessageBox::Yes, QMessageBox::No)
            == QMessageBox::Yes)
        {
            for (auto it = selections.rbegin(); it != selections.rend(); it++)
                manager->removeModel(it->row());
            ui->openGLWidget->update();
            updateModelList();
        }
    });
    connect(ui->lightList, &ColumnList::deleteKeyPressed, this, [&] {
        auto selections = ui->lightList->selectionModel()->selectedRows();
        auto manager = ui->openGLWidget->getLightManager();
        if (selections.size() == 1 && selections.begin()->row() == 0) return;
        if (selections.size() > 0 && QMessageBox::information(this,
            QString::fromLocal8Bit("删除光源"),
            QString::fromLocal8Bit("确认删除选中光源"),
            QMessageBox::Yes, QMessageBox::No)
            == QMessageBox::Yes)
        {
            for (auto it = selections.rbegin(); it != selections.rend() - 1; it++)
                manager->removeLight(it->row());
            ui->openGLWidget->update();
            updateLightList();
        }
    });
}

Content::~Content()
{
    delete ui;
}

void Content::openModelList()
{
    ui->modelList->setMaximumHeight(16777215);
    ui->modelWidget->show();
    ui->lightWidget->hide();
}

void Content::showModelInfo(bool hideList)
{
    openModelList();
    auto selections = ui->modelList->selectionModel()->selectedRows();
    if (selections.size() == 0)
    {
        ui->modelProp->hide();
        ui->modelProp->setModel(nullptr);
        return;
    }
    // Use ui->modelList->hide() may cause an unexplained bug
    // which changes current selection
    if (hideList) ui->modelList->setMaximumHeight(0);
    auto models = ui->openGLWidget->getModelVector();
    int row = (selections.end() - 1)->row();
    ui->modelProp->setModel(&(models->at(row)));
    ui->modelProp->show();
}

void Content::showLightInfo(bool hideList)
{
    openLightList();
    auto selections = ui->lightList->selectionModel()->selectedRows();
    if (selections.size() == 0)
        ui->lightProp->hide();
    else
    {
        int row = (selections.end() - 1)->row();
        auto light = ui->openGLWidget->getLightManager()->getLight(row);
        if (hideList) ui->lightList->setMaximumHeight(0);
        ui->lightProp->setLight(light);
        ui->lightProp->show();
    }
}

void Content::openLightList()
{
    ui->lightList->setMaximumHeight(16777215);
    ui->modelWidget->hide();
    ui->lightWidget->show();
}

void Content::initControls()
{
    // Model menu
    QAction * modelCube = new QAction(MODEL_CUBE_TEXT);
    modelCube->setCheckable(true);
    modelCube->setIcon(MODEL_CUBE_ICON);
    modelCube->setChecked(true);
    curModelAction = modelCube;
    QAction * modelCylinder = new QAction(MODEL_CYLINDER_TEXT);
    modelCylinder->setCheckable(true);
    modelCylinder->setIcon(MODEL_CYLINDER_ICON);
    QAction * modelSphere = new QAction(MODEL_SPHERE_TEXT);
    modelSphere->setCheckable(true);
    modelSphere->setIcon(MODEL_SPHERE_ICON);
    modelMenuAction.insert(modelCube, gm::MODEL_CUBE);
    modelMenuAction.insert(modelCylinder, gm::MODEL_CYLINDER);
    modelMenuAction.insert(modelSphere, gm::MODEL_SPHERE);
    modelMenu.addAction(modelCube);
    modelMenu.addAction(modelCylinder);
    modelMenu.addAction(modelSphere);
    ui->toolModel->setContextMenu(&modelMenu);
    // Light menu
    QAction * lightPoint = new QAction(LIGHT_POINT_TEXT);
    lightPoint->setCheckable(true);
    lightPoint->setIcon(LIGHT_POINT_ICON);
    lightPoint->setChecked(true);
    curLightAction = lightPoint;
    QAction * lightSpot = new QAction(LIGHT_SPOT_TEXT);
    lightSpot->setCheckable(true);
    lightSpot->setIcon(LIGHT_SPOT_ICON);
    lightMenuAction.insert(lightPoint, POINT_LIGHT);
    lightMenuAction.insert(lightSpot, SPOT_LIGHT);
    lightMenu.addAction(lightPoint);
    lightMenu.addAction(lightSpot);
    ui->toolLight->setContextMenu(&lightMenu);
    // Model list
    modelListModel = new QStandardItemModel(this);
    modelListModel->setColumnCount(3);
    modelListModel->setHeaderData(0, Qt::Horizontal, "ID");
    modelListModel->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("模型名称"));
    modelListModel->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("模型类型"));
    ui->modelList->setModel(modelListModel);
    ui->modelList->verticalHeader()->hide();
    ui->modelList->setColumnWidth(0, 50);
    ui->modelList->setColumnWidth(1, 130);
    ui->modelList->setColumnWidth(2, 100);
    ui->modelList->horizontalHeader()->setHighlightSections(false);
    ui->modelList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    updateModelList();
    ui->modelProp->hide();
    // Light list
    lightListModel = new QStandardItemModel(this);
    lightListModel->setColumnCount(2);
    lightListModel->setHeaderData(0, Qt::Horizontal, "ID");
    lightListModel->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("模型类型"));
    ui->lightList->setModel(lightListModel);
    ui->lightList->verticalHeader()->hide();
    ui->lightList->setColumnWidth(0, 60);
    ui->lightList->setColumnWidth(1, 220);
    ui->lightList->horizontalHeader()->setHighlightSections(false);
    ui->lightList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->lightProp->hide();
    lightListModel->setItem(0, 0, new QStandardItem(QString::number(1)));
    lightListModel->setItem(0, 1, new QStandardItem(LIGHT_DIRECTIONAL_TEXT));
    ui->lightList->setRowHeight(0, LIST_ROW_HEIGHT);
}

void Content::addModel()
{
    float tX = (float)modelAddDlg.ui->tX->value();
    float tY = (float)modelAddDlg.ui->tY->value();
    float tZ = (float)modelAddDlg.ui->tZ->value();
    float rX = (float)modelAddDlg.ui->rX->value();
    float rY = (float)modelAddDlg.ui->rY->value();
    float rZ = (float)modelAddDlg.ui->rZ->value();
    float sX = (float)modelAddDlg.ui->sX->value();
    float sY = (float)modelAddDlg.ui->sY->value();
    float sZ = (float)modelAddDlg.ui->sZ->value();
    gm::MODEL_TYPE type = modelMenuAction[curModelAction];
    if (type == gm::MODEL_CUBE)
    {
        gm::Cube<float> cube(tX, tY, tZ, sX, sY, sZ);
        ui->openGLWidget->addModel(cube);
    }
    else if (type == gm::MODEL_CYLINDER)
    {
        
    }
    else if (type == gm::MODEL_SPHERE)
    {

    }
    updateModelList();
}

void Content::addLight()
{
    auto manager = ui->openGLWidget->getLightManager();
    auto type = lightMenuAction[curLightAction];
    if (type == POINT_LIGHT)
    {
        if (!manager->addLight(type)) QMessageBox::information(
            this,
            ADD_TEXT + LIGHT_POINT_TEXT,
            LIGHT_POINT_TEXT + QString::fromLocal8Bit("数量已达上限"));
    }
    else if (type == SPOT_LIGHT)
    {
        if (!manager->addLight(type)) QMessageBox::information(
            this,
            ADD_TEXT + LIGHT_SPOT_TEXT,
            LIGHT_SPOT_TEXT + QString::fromLocal8Bit("数量已达上限"));
    }
    updateLightList();
    ui->openGLWidget->update();
}

void Content::showAddModelDlg(gm::MODEL_TYPE modelType)
{
    if (modelType == gm::MODEL_CUBE)
    {
        modelAddDlg.setWindowTitle(ADD_TEXT + MODEL_CUBE_TEXT);
        modelAddDlg.ui->sY->setEnabled(true);
        modelAddDlg.ui->sZ->setEnabled(true);
        modelAddDlg.exec();
    }
    else if (modelType == gm::MODEL_CYLINDER)
    {
        modelAddDlg.setWindowTitle(ADD_TEXT + MODEL_CYLINDER_TEXT);
        modelAddDlg.ui->sY->setEnabled(true);
        modelAddDlg.ui->sZ->setEnabled(false);
        modelAddDlg.exec();
    }
    else if (modelType == gm::MODEL_SPHERE)
    {
        modelAddDlg.setWindowTitle(ADD_TEXT + MODEL_SPHERE_TEXT);
        modelAddDlg.ui->sY->setEnabled(false);
        modelAddDlg.ui->sZ->setEnabled(false);
        modelAddDlg.exec();
    }
}

void Content::updateModelList()
{
    auto list = ui->openGLWidget->getModelVector();
    modelListModel->removeRows(0, modelListModel->rowCount());

    for (int i = 0; i < list->size(); i++)
    {
        modelListModel->setItem(i, 0, new QStandardItem(QString::number(i + 1)));
        modelListModel->setItem(i, 1, new QStandardItem(QString::fromStdString(list->at(i).name)));
        switch (list->at(i).type)
        {
        case gm::MODEL_CUBE: modelListModel->setItem(i, 2, new QStandardItem(MODEL_CUBE_TEXT)); break;
        case gm::MODEL_CYLINDER: modelListModel->setItem(i, 2, new QStandardItem(MODEL_CYLINDER_TEXT)); break;
        case gm::MODEL_SPHERE: modelListModel->setItem(i, 2, new QStandardItem(MODEL_SPHERE_TEXT)); break;
        default: break;
        }
        ui->modelList->setRowHeight(i, 20);
    }
}

void Content::updateLightList()
{
    LightManager * manager = ui->openGLWidget->getLightManager();
    auto pointLights = manager->getPointLights();
    auto spotLights = manager->getSpotLights();
    lightListModel->removeRows(1, lightListModel->rowCount());
    int i = 1;
    for (auto it = pointLights->begin(); it != pointLights->end(); it++, i++)
    {
        lightListModel->setItem(i, 0, new QStandardItem(QString::number(i + 1)));
        lightListModel->setItem(i, 1, new QStandardItem(QString::fromLocal8Bit("点光源")));
        ui->lightList->setRowHeight(i, LIST_ROW_HEIGHT);
    }
    for (auto it = spotLights->begin(); it != spotLights->end(); it++, i++)
    {
        lightListModel->setItem(i, 0, new QStandardItem(QString::number(i + 1)));
        lightListModel->setItem(i, 1, new QStandardItem(QString::fromLocal8Bit("聚光灯")));
        ui->lightList->setRowHeight(i, LIST_ROW_HEIGHT);
    }
}
