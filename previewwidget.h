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
        bindBuffer();
        update();
    }
    std::vector<ModelObject> * getModelVector()
    {
        return mManager.getModelVector();
    }
    ModelObject * getModelById(unsigned int id)
    {
        return mManager.getModelObjById(id);
    }
    void changeModelTexture(unsigned int id, unsigned int textureIndex, std::string filepath)
    {
        mManager.changeModelTexture(id, textureIndex, filepath);
    }
    QPair<QString, int> addTexture(QString filepath)
    {
        filepath = tManager.addTexture(filepath);
        unsigned int index = -1;
        if (filepath != "") index = tManager.getTextureIndex(filepath);
        return QPair<QString, int>(filepath, index);
    }
    ModelManager * getModelManager() { return &mManager; }
    TextureManager * getTextureManager() { return &tManager; }
    LightManager * getLightManager() { return &lManager; }

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
    void bindBuffer();
    void drawAxis();
    void drawZ0Plane();
    void drawModel(ModelObject & model);
    void drawBoundingBox();
    void addLight();
    void drawLight();
};

#endif // PREVIEWWIDGET_H
