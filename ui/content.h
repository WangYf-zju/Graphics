#ifndef CONTENT_H
#define CONTENT_H

#include <QWidget>
#include <QDockWidget>
#include <QToolButton>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QListView>
#include <QMap>
#include <QStandardItemModel>
#include <QMessageBox>

#include "controls.h"
#include "modeladddlg.h"
#include "models/GraphicModel"
#include "LightManager.h"

namespace Ui {
class Content;
}

class Content : public QWidget
{
    friend class MainWindow;
    Q_OBJECT

public:
    explicit Content(QWidget *parent = nullptr);
    ~Content();
    void openModelList();
    void openLightList();

private:
    Ui::Content *ui;
    ModelAddDlg modelAddDlg;
    QMenu modelMenu, lightMenu;
    QMap<QAction *, gm::MODEL_TYPE> modelMenuAction;
    QMap<QAction *, LIGHT_TYPE> lightMenuAction;
    QAction * curModelAction, * curLightAction;
    QStandardItemModel * modelListModel, * lightListModel;
    void initControls();
    void addModel();
    void addLight();
    void showAddModelDlg(gm::MODEL_TYPE modelType);
    void showModelInfo(bool hideList = false);
    void showLightInfo(bool hideList = false);
    void updateModelList();
    void updateLightList();
};

#endif // CONTENT_H
