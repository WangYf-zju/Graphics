#include "content.h"
#include "ui_content.h"
#include "ui_modeladddlg.h"

#include <QDebug>

#define MODEL_CUBE_ICON QIcon(":/icons/icons/cube.ico")
#define MODEL_CYLINDER_ICON QIcon(":/icons/icons/cylinder.ico")
#define MODEL_SPHERE_ICON QIcon(":/icons/icons/sphere.ico")
#define MODEL_ADD QString::fromLocal8Bit("添加")
#define MODEL_CUBE_TEXT QString::fromLocal8Bit("长方体")
#define MODEL_CYLINDER_TEXT QString::fromLocal8Bit("圆柱")
#define MODEL_SPHERE_TEXT QString::fromLocal8Bit("球体")

Content::Content(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Content)
    , curModelAction(nullptr)
    , modelAddDlg(this)
{
    ui->setupUi(this);
    initControls();
    ui->properties->hide();
    connect(ui->toolModel, &QToolButton::released, this, [&]() {
        ShowAddModelDlg(modelMenuAction[curModelAction]);
    });
    connect(&modelMenu, &QMenu::triggered, this, [&](QAction * action) {
        if(curModelAction) curModelAction->setChecked(false);
        ui->toolModel->setIcon(action->icon());
        curModelAction = action;
        ShowAddModelDlg(modelMenuAction[action]);
    });
    connect(modelAddDlg.ui->buttonBox->button(QDialogButtonBox::Ok),
        &QPushButton::clicked, this, [&]() {
            AddModel();
            modelAddDlg.close();
    });
    connect(modelAddDlg.ui->buttonBox->button(QDialogButtonBox::Cancel),
        &QPushButton::clicked, this, [&]() { modelAddDlg.close(); });
    connect(ui->list->selectionModel(), &QItemSelectionModel::selectionChanged, this,
        [&](const QItemSelection &, const QItemSelection &) {
        QList<QModelIndex> select = ui->list->selectionModel()->selectedRows();
        auto models = ui->openGLWidget->getModelVector();
        for (int i = 0; i < models->size(); i++)
            models->at(i).select = false;
        for (auto p = select.begin(); p != select.end(); p++)
            models->at(p->row()).select = true;
        ui->openGLWidget->update();
        openModelInfo(false);
    });
    connect(ui->properties, &PropertiesWidget::modelChanged, this, [&](ModelObject & model) {
        ModelObject * oldModel = ui->openGLWidget->getModelById(model.id);
        if (oldModel != nullptr)
        {
            *(oldModel) = model;
            ui->openGLWidget->update();
        }
    });
    connect(ui->properties, &PropertiesWidget::textureChanged, this, 
        [&](unsigned int id, QString filepath) {
        auto texture = ui->openGLWidget->addTexture(filepath);
        filepath = texture.first;
        int index = texture.second;
        if (filepath != "")
        {
            ui->openGLWidget->changeModelTexture(id, index, filepath.toStdString());
            ui->openGLWidget->update();
            ui->properties->updateTexture(index, filepath);
        }
    });
}

Content::~Content()
{
    delete ui;
}

void Content::openModelList()
{
    ui->dockWidget->setWindowTitle(QString::fromLocal8Bit("模型列表"));
    //ui->properties->hide();
    ui->list->setMaximumHeight(16777215);
    ui->dockWidget->show();
}

void Content::openModelInfo(bool hideList)
{
    QList<QModelIndex> select = ui->list->selectionModel()->selectedRows();
    if (select.size() == 0)
    {
        ui->properties->hide();
        ui->properties->setModel(nullptr);
        return;
    }
    // Use ui->list->hide() may cause an unexplained bug
    // which changes current selection
    if (hideList) 
    {
        ui->list->setMaximumHeight(0);
        ui->dockWidget->setWindowTitle(QString::fromLocal8Bit("模型属性"));
    }
    auto models = ui->openGLWidget->getModelVector();
    int row = (select.end() - 1)->row();
    ui->properties->setModel(&(models->at(row)));

    ui->properties->show();
    ui->dockWidget->show();
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
    // Model list
    listModel = new QStandardItemModel(this);
    listModel->setColumnCount(3);
    listModel->setHeaderData(0, Qt::Horizontal, "ID");
    listModel->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("模型名称"));
    listModel->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("模型类型"));
    ui->list->setModel(listModel);
    ui->list->verticalHeader()->hide();
    ui->list->setColumnWidth(0, 50);
    ui->list->setColumnWidth(1, 130);
    ui->list->setColumnWidth(2, 100);
    ui->list->horizontalHeader()->setHighlightSections(false);
    ui->list->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->dockWidget->setFeatures(QDockWidget::DockWidgetClosable);
    updateModelList();
    // 
}

void Content::AddModel()
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
}

void Content::ShowAddModelDlg(gm::MODEL_TYPE modelType)
{
    if (modelType == gm::MODEL_CUBE)
    {
        modelAddDlg.setWindowTitle(MODEL_ADD + MODEL_CUBE_TEXT);
        modelAddDlg.ui->sY->setEnabled(true);
        modelAddDlg.ui->sZ->setEnabled(true);
        modelAddDlg.exec();
    }
    else if (modelType == gm::MODEL_CYLINDER)
    {
        modelAddDlg.setWindowTitle(MODEL_ADD + MODEL_CYLINDER_TEXT);
        modelAddDlg.ui->sY->setEnabled(true);
        modelAddDlg.ui->sZ->setEnabled(false);
        modelAddDlg.exec();
    }
    else if (modelType == gm::MODEL_SPHERE)
    {
        modelAddDlg.setWindowTitle(MODEL_ADD + MODEL_SPHERE_TEXT);
        modelAddDlg.ui->sY->setEnabled(false);
        modelAddDlg.ui->sZ->setEnabled(false);
        modelAddDlg.exec();
    }
}

void Content::updateModelList()
{
    auto list = ui->openGLWidget->getModelVector();
    listModel->removeRows(0, listModel->rowCount());

    for (int i = 0; i < list->size(); i++)
    {
        listModel->setItem(i, 0, new QStandardItem(QString::number(list->at(i).id)));
        listModel->setItem(i, 1, new QStandardItem(QString::fromStdString(list->at(i).name)));
        switch (list->at(i).type)
        {
        case gm::MODEL_CUBE: listModel->setItem(i, 2, new QStandardItem(MODEL_CUBE_TEXT)); break;
        case gm::MODEL_CYLINDER: listModel->setItem(i, 2, new QStandardItem(MODEL_CYLINDER_TEXT)); break;
        case gm::MODEL_SPHERE: listModel->setItem(i, 2, new QStandardItem(MODEL_SPHERE_TEXT)); break;
        default: break;
        }
        ui->list->setRowHeight(i, 20);
    }
}
