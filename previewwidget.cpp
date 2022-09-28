#include "previewwidget.h"
#include <QDebug>

PreviewWidget::PreviewWidget(QWidget *parent, Qt::WindowFlags flag)
    : QOpenGLWidget(parent, flag)
    , cube(0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f)
    , camera(glm::vec3(0.0f, 0.0f, 1.0f), -90.0f)
    , shader("./shaders/shader.vs", "./shaders/shader.fs")
{
    this->mousePressButton = Qt::NoButton;
    this->width = 800;
    this->height = 600;
}

PreviewWidget::~PreviewWidget()
{
    
}

void PreviewWidget::initializeGL()
{
    // initializeOpenGLFunctions();
    this->f = QOpenGLContext::currentContext()->
        versionFunctions<QOpenGLFunctions_3_3_Core>();
    shader.initialize(f)->compile();
    f->glGenVertexArrays(1, &VAO);
    f->glGenBuffers(1, &VBO);
    f->glGenBuffers(1, &EBO);
    
    f->glBindVertexArray(VAO);
    f->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    f->glBufferData(GL_ARRAY_BUFFER, cube.vertexSize(), cube.getVertex(), GL_STATIC_DRAW);
    f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube.indicesSize(), cube.getIndices(), GL_STATIC_DRAW);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    f->glEnableVertexAttribArray(0);
    //cube.rotate(1, 1, 1);
    f->glEnable(GL_DEPTH_TEST);

    
}


void PreviewWidget::paintGL()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    f->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.use();
    glm::mat4 model = cube.getModelMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), (float)width / (float)height, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    shader.setMat4("model", model)
        ->setMat4("view", view)
        ->setMat4("projection", projection);

    f->glBindVertexArray(VAO);
    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    f->glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}


void PreviewWidget::resizeGL(int w, int h)
{
    this->width = w;
    this->height = h;
}


void PreviewWidget::wheelEvent(QWheelEvent * event)
{
    int d = event->delta();
    camera.zoom((float)d * 0.002);
    update();
}


void PreviewWidget::mousePressEvent(QMouseEvent *event)
{
    if (this->mousePressButton == Qt::NoButton) {
        this->mousePressButton = event->button();
        this->point = event->pos();
    }
}


void PreviewWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (this->mousePressButton == event->button())
        this->mousePressButton = Qt::NoButton;
}


void PreviewWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (this->mousePressButton == Qt::NoButton) return;
    int dx = event->pos().x() - this->point.x();
    int dy = event->pos().y() - this->point.y();
    this->point = event->pos();
    if (this->mousePressButton == Qt::LeftButton) 
    {
        camera.translate(-(float)dx*0.002, (float)dy*0.002);
        update();
    }
    else if (this->mousePressButton == Qt::MiddleButton) 
    {
        
    }
    else if (this->mousePressButton == Qt::RightButton) 
    {
        camera.rotate(-(float)dx*0.05, (float)dy*0.05);
        update();
    }
}
