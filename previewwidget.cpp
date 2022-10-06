#include "previewwidget.h"
#include <QDebug>

unsigned int ModelManager::modelNameIndex = 1;

PreviewWidget::PreviewWidget(QWidget *parent, Qt::WindowFlags flag)
    : QOpenGLWidget(parent, flag)
    , camera(glm::vec3(0.0f, 10.0f, 10.0f), -90.0f, -45.0f)
    , shader("./shaders/shader.vs", "./shaders/shader.fs")
{
    this->setWindowFlag(Qt::WindowStaysOnBottomHint, true);
    this->mousePressButton = Qt::NoButton;
    mManager.addModel(new gm::Sphere<float>(0.0f, 0.0f, 0.0f, 1.0f));
    mManager.addModel(new gm::Cube<float>(2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f));

}

PreviewWidget::~PreviewWidget()
{
    
}

void PreviewWidget::initializeGL()
{
    this->f = QOpenGLContext::currentContext()->
        versionFunctions<QOpenGLFunctions_3_3_Core>();
    shader.initialize(f)->compile();
    f->glGenVertexArrays(1, &VAO);
    f->glGenBuffers(1, &VBO);
    f->glGenBuffers(1, &EBO);
    f->glEnable(GL_DEPTH_TEST);
    bindBuffer();
}


void PreviewWidget::paintGL()
{
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    f->glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.use();
    glm::mat4 projection = glm::perspective(
        glm::radians(camera.getZoom()), 
        (float)this->width() / (float)this->height(), 
        0.1f, 
        100.0f
    );
    glm::mat4 view = camera.GetViewMatrix();
    shader.setMat4("view", view)
        ->setMat4("projection", projection)
        ->setMat4("model", glm::mat4(1.0f));
    f->glBindVertexArray(VAO);
    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    
    drawZ0Plane();
    for (int i = 0; i < mManager.size(); i++)
    {
        drawModel(mManager.getModelObj(i));
    }
    drawAxis();
    f->glBindVertexArray(0);
}


void PreviewWidget::resizeGL(int w, int h)
{
    update();
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
        camera.translate(-(float)dx*0.005, (float)dy*0.005);
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

void PreviewWidget::bindBuffer()
{
    f->glBindVertexArray(VAO);
    f->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    f->glBufferData(GL_ARRAY_BUFFER, mManager.asize(), mManager.adata(), GL_STATIC_DRAW);
    f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, mManager.esize(), mManager.edata(), GL_STATIC_DRAW);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, EACH_VERTEX_SIZE * sizeof(float), (void*)0);
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, EACH_VERTEX_SIZE * sizeof(float), (void*)(6*sizeof(float)));
    f->glEnableVertexAttribArray(1);
    f->glUniform1i(f->glGetUniformLocation(shader.ID, "t"), 0);
}

void PreviewWidget::drawAxis()
{
    shader.setFloat("mode", 0.0f);
    shader.setMat4("model", glm::mat4(1.0f));
    shader.setVec4("color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    f->glDrawArrays(GL_LINES, X_AXIS_VERTEX_START, X_AXIS_VERTEX_COUNT);
    shader.setVec4("color", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    f->glDrawArrays(GL_LINES, Y_AXIS_VERTEX_START, Y_AXIS_VERTEX_COUNT);
    shader.setVec4("color", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    f->glDrawArrays(GL_LINES, Z_AXIS_VERTEX_START, Z_AXIS_VERTEX_COUNT);
}


void PreviewWidget::drawZ0Plane()
{
    shader.setFloat("mode", 0.0f);
    shader.setMat4("model", glm::mat4(1.0f));
    shader.setVec4("color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    f->glDrawArrays(GL_LINES, Z0_PLANE_VERTEX_START, Z0_PLANE_VERTEX_COUNT);
}


void PreviewWidget::drawModel(ModelObject & model)
{
    if (model.visible)
    {
        shader.setMat4("model", model.modelMatrix);
        shader.setVec4("color", model.color);
        if (model.texture == "")
        {
            shader.setFloat("mode", 0.0f);
        }
        else
        {
            shader.setFloat("mode", 1.0f);
            tManager.bind(model.textureIndex);
        }
        if (model.method == INDICES_DRAW)
        {
            int offset = model.indicesStartIndex * sizeof(unsigned int);
            f->glDrawElements(
                GL_TRIANGLES,
                model.indicesSize,
                GL_UNSIGNED_INT,
                (void*)offset
            );
        }
        else if (model.method == DIRECT_DRAW)
        {
            f->glDrawArrays(
                GL_TRIANGLES,
                model.vertexStartIndex / EACH_VERTEX_SIZE,
                model.vertexSize / (3 * EACH_VERTEX_SIZE)
            );
        }
        if (model.select)
        {
            drawBoundingBox();
        }
    }
}


void PreviewWidget::drawBoundingBox()
{
    shader.setFloat("mode", 0.0f);
    shader.setVec4("color", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
    f->glLineWidth(2.0f);
    f->glDrawArrays(GL_LINES, BOUNDING_BOX_VERTEX_START, BOUNDING_BOX_VERTEX_COUNT);
    f->glLineWidth(1.0f);
}
