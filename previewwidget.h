#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <QWheelEvent>
#include <QMouseEvent>
#include <QPoint>
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_CORE>
#include "models/GraphicModel"
#include "Shader.h"
#include "Camera.h"

class PreviewWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    PreviewWidget(QWidget *parent = nullptr, Qt::WindowFlags flag = Qt::WindowFlags());
    ~PreviewWidget();

protected:
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int, int) override;
    virtual void wheelEvent(QWheelEvent * event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    GraphicModel::Cube<float> cube;
    QOpenGLFunctions_3_3_Core * f;
    Shader shader;
    Camera camera;
    float width;
    float height;
    Qt::MouseButton mousePressButton;
    QPoint point;
};

#endif // PREVIEWWIDGET_H
