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

#include "controls.h"
#include "modeladddlg.h"
#include "models/GraphicModel"

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
    void openModelInfo(bool hideList = true);

private:
    Ui::Content *ui;
    ModelAddDlg modelAddDlg;
    QMenu modelMenu;
    QMap<QAction *, gm::MODEL_TYPE> modelMenuAction;
    QAction * curModelAction;
    QStandardItemModel * listModel;
    void initControls();
    void AddModel();
    void ShowAddModelDlg(gm::MODEL_TYPE modelType);

    void updateModelList();
};

#endif // CONTENT_H
