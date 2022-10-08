#ifndef CONTROLS_H
#define CONTROLS_H

#include <QLabel>
#include <QEvent>
#include <QMouseEvent>
#include <QString>
#include <QChar>

#include "models/GraphicModel"

static QString qss_normal = "margin: 5px 0px 5px;\n"
"border: none;\n"
"border-right: 5px solid #ccc;\n"
"color: black;";
static QString qss_highlight = "margin: 5px 0px 5px;\n"
"border: none;\n"
"border-right: 5px solid #40a9ff;\n"
"color: #40a9ff;";

class NavLabel : public QLabel
{
    Q_OBJECT
public:
    NavLabel(QWidget * parent = nullptr) 
        : QLabel(parent)
        , isFocus(false) {}
signals:
    void clicked();
public:
    void focus() 
    {
        this->isFocus = true;
        this->setStyleSheet(qss_highlight); 
    }
    void blur() 
    { 
        this->isFocus = false;
        this->setStyleSheet(qss_normal); 
    }

protected:
    virtual void enterEvent(QEvent * event) override
    {
        if (!isFocus) this->setStyleSheet(qss_highlight);
        QLabel::enterEvent(event);
    }
    virtual void leaveEvent(QEvent * event) override
    {
        if (!isFocus) this->setStyleSheet(qss_normal);
        QLabel::leaveEvent(event);
    }
    virtual void mouseReleaseEvent(QMouseEvent * event) override
    {
        if (event->button() == Qt::LeftButton) emit clicked();
        QLabel::mouseReleaseEvent(event);
    }
private:
    bool isFocus;
};

#include <QToolButton>
#include <QMenu>
#include <QContextMenuEvent>

class SwitchToolButton : public QToolButton
{
    Q_OBJECT
public:
    SwitchToolButton(QWidget * parent = nullptr) : QToolButton(parent){}
    void setContextMenu(QMenu * menu) { this->menu = menu; }

protected:
    virtual void contextMenuEvent(QContextMenuEvent * event) override
    {
        QPoint pt(this->width(), 0);
        menu->exec(this->mapToGlobal(pt));
    }
private:
    QMenu * menu;
};

#include <QDockWidget>
#include <QCloseEvent>

class NavDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    NavDockWidget(QWidget * parent = nullptr) : QDockWidget(parent) 
    {
        this->setFeatures(QDockWidget::DockWidgetClosable);
    }
signals:
    void closed();
protected:
    virtual void closeEvent(QCloseEvent * event) override
    {
        emit closed();
        QDockWidget::closeEvent(event);
    }
    virtual void hideEvent(QHideEvent *event) override
    {
        emit closed();
        QDockWidget::hideEvent(event);
    }
};

#include <QTableView>
#include <QKeyEvent>

class ColumnList : public QTableView
{
    Q_OBJECT
public:
    ColumnList(QWidget * parent = nullptr) : QTableView(parent) {}
signals:
    void deleteKeyPressed();
protected:
    virtual void keyPressEvent(QKeyEvent * event) override
    {
        if (event->key() == Qt::Key_Delete)
            emit deleteKeyPressed();
        QTableView::keyPressEvent(event);
    }
};


#endif // !CONTROLS_H

