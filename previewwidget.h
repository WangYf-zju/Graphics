#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <QWheelEvent>
#include <QMouseEvent>
#include <QPoint>
#include <QVector>
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_CORE>

#include "models/GraphicModel"
#include "Shader.h"
#include "Camera.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "LightManager.h"

class PreviewWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    PreviewWidget(QWidget *parent = nullptr, Qt::WindowFlags flag = Qt::WindowFlags());
    ~PreviewWidget();
    template<typename Derived>
    void addModel(gm::BaseModel<Derived> & model)
    {
        mManager.addModel(&model);
        rebindBuffer();
        update();
    }
    ModelManager * getModelManager() { return &mManager; }
    TextureManager * getTextureManager() { return &tManager; }
    LightManager * getLightManager() { return &lManager; }
    Camera * getCamera() { return &camera; }
    void rebindBuffer();
    void showAxis(bool b) { this->_showAxis = b; }
    void showPlane(bool b) { this->_showPlane = b; }

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
    bool _showAxis, _showPlane;
    QOpenGLFunctions_3_3_Core * f;
    Shader modelShader;
    Shader worldShader;
    Camera camera;
    QVector<glm::mat4> boundingBoxMatrix;
    
    Qt::MouseButton mousePressButton;
    QPoint point;
    ModelManager mManager;
    TextureManager tManager;
    LightManager lManager;
    void drawAxis();
    void drawZ0Plane();
    void drawModel(ModelObject & model);
    void drawBoundingBox();
    void addLight();
    void drawLight();
};

#endif // PREVIEWWIDGET_H
