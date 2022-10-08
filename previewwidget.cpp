#include "previewwidget.h"
#include <QDebug>

unsigned int ModelManager::modelNameIndex = 1;

PreviewWidget::PreviewWidget(QWidget *parent, Qt::WindowFlags flag)
    : QOpenGLWidget(parent, flag)
    , camera(glm::vec3(0.0f, 10.0f, 10.0f), -90.0f, -45.0f)
    , modelShader("./shaders/shaderM.vs", "./shaders/shaderM.fs")
    , worldShader("./shaders/shaderW.vs", "./shaders/shaderW.fs")
{
    this->setWindowFlag(Qt::WindowStaysOnBottomHint, true);
    this->mousePressButton = Qt::NoButton;
    //mManager.addModel(new gm::Sphere<float>(0.0f, 0.0f, 0.0f, 1.0f));
    mManager.addModel(new gm::Cube<float>(2.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f));
    mManager.addModel(new gm::Cylinder<float>(0.0f, 0.0f, 0.0f, 1.0f, 1.0f));

}

PreviewWidget::~PreviewWidget()
{
    
}

void PreviewWidget::initializeGL()
{
    this->f = QOpenGLContext::currentContext()->
        versionFunctions<QOpenGLFunctions_3_3_Core>();
    modelShader.initialize(f)->compile();
    worldShader.initialize(f)->compile();
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
    modelShader.use();
    GLint viewPosLoc = f->glGetUniformLocation(modelShader.ID, "viewPos");
    f->glUniform3f(viewPosLoc, camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
    // Set material properties
    f->glUniform1f(f->glGetUniformLocation(modelShader.ID, "material.shininess"), 32.0f);
    
    addLight();
    glm::mat4 projection = glm::perspective(
        glm::radians(camera.getZoom()), 
        (float)this->width() / (float)this->height(), 
        0.1f, 
        100.0f
    );
    glm::mat4 view = camera.GetViewMatrix();
    modelShader.setMat4("view", view)
        ->setMat4("projection", projection)
        ->setMat4("model", glm::mat4(1.0f));
    f->glBindVertexArray(VAO);
    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    
    
    for (int i = 0; i < mManager.size(); i++)
    {
        drawModel(mManager.getModelObj(i));
    }
    worldShader.use();
    worldShader.setMat4("view", view)
        ->setMat4("projection", projection)
        ->setMat4("model", glm::mat4(1.0f));
    drawZ0Plane();
    drawAxis();
    drawBoundingBox();
    f->glBindVertexArray(0);
}


void PreviewWidget::resizeGL(int w, int h)
{
    update();
}


void PreviewWidget::wheelEvent(QWheelEvent * event)
{
    int d = event->delta();
    camera.zoom((float)d * 0.005);
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
    int dx = -event->pos().x() + this->point.x();
    int dy = -event->pos().y() + this->point.y();
    this->point = event->pos();
    if (this->mousePressButton == Qt::LeftButton)
    {
        camera.translate((float)dx*0.005, (float)dy*0.005);
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
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, EACH_VERTEX_SIZE * sizeof(float), (void*)(3*sizeof(float)));
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, EACH_VERTEX_SIZE * sizeof(float), (void*)(6*sizeof(float)));
    f->glEnableVertexAttribArray(2);
    f->glUniform1i(f->glGetUniformLocation(modelShader.ID, "material.diffuse"), 0);
}

void PreviewWidget::drawAxis()
{
    worldShader.setMat4("model", glm::mat4(1.0f));
    worldShader.setVec4("purity", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    f->glDrawArrays(GL_LINES, X_AXIS_VERTEX_START, X_AXIS_VERTEX_COUNT);
    worldShader.setVec4("purity", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    f->glDrawArrays(GL_LINES, Y_AXIS_VERTEX_START, Y_AXIS_VERTEX_COUNT);
    worldShader.setVec4("purity", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    f->glDrawArrays(GL_LINES, Z_AXIS_VERTEX_START, Z_AXIS_VERTEX_COUNT);
}


void PreviewWidget::drawZ0Plane()
{
    worldShader.setMat4("model", glm::mat4(1.0f));

    worldShader.setVec4("purity", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    f->glDrawArrays(GL_LINES, Z0_PLANE_VERTEX_START, Z0_PLANE_VERTEX_COUNT);
}


void PreviewWidget::drawModel(ModelObject & model)
{
    if (model.visible)
    {
        modelShader.setMat4("model", model.modelMatrix);
        modelShader.setVec4("material.purity", model.color);
        modelShader.setVec3("material.specular", model.specular, model.specular, model.specular);
        if (model.texture == "")
        {
            modelShader.setFloat("material.texMode", 0.0f);
        }
        else
        {
            modelShader.setFloat("material.texMode", 1.0f);
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
            // drawBoundingBox();
            boundingBoxMatrix.push_back(model.modelMatrix);
        }
    }
}


void PreviewWidget::drawBoundingBox()
{
    f->glLineWidth(2.0f);
    for (auto m : boundingBoxMatrix)
    {
        worldShader.setVec4("purity", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
        worldShader.setMat4("model", m);
        f->glDrawArrays(GL_LINES, BOUNDING_BOX_VERTEX_START, BOUNDING_BOX_VERTEX_COUNT);
    }
    f->glLineWidth(1.0f);
    boundingBoxMatrix.clear();
}

void PreviewWidget::addLight()
{
    LightObject * dirLight = lManager.getDirectionalLight();
    auto pointLight = lManager.getPointLights();
    auto spotLight = lManager.getSpotLights();
    modelShader.setInt("plCount", pointLight->size());
    modelShader.setInt("slCount", spotLight->size());
    // Directional light
    if (dirLight != nullptr)
    {
        if (dirLight->isEnable)
        {
            float diffuse = dirLight->diffuse, specular = dirLight->specular;
            modelShader.setVec3("dirLight.direction", dirLight->direction)
                ->setVec3("dirLight.ambient", dirLight->ambient)
                ->setVec3("dirLight.diffuse", diffuse, diffuse, diffuse)
                ->setVec3("dirLight.specular", specular, specular, specular);
        }
        else
        {
            modelShader.setVec3("dirLight.ambient", 0.0f, 0.0f, 0.0f)
                ->setVec3("dirLight.diffuse", 0.0f, 0.0f, 0.0f)
                ->setVec3("dirLight.specular", 0.0f, 0.0f, 0.0f);
        }
    }
    // Point light
    int i = 0;
    for (auto it = pointLight->begin(); it != pointLight->end(); it++, i++)
    {
        std::string name = "pointLights[" + std::to_string(i) + "].";
        if (it->isEnable)
        {
            float diffuse = it->diffuse, specular = it->specular;
            modelShader.setVec3(name + "position", it->position)
                ->setVec3(name + "ambient", it->ambient)
                ->setVec3(name + "diffuse", diffuse, diffuse, diffuse)
                ->setVec3(name + "specular", specular, specular, specular)
                ->setFloat(name + "constant", it->attenuation.z)
                ->setFloat(name + "linear", it->attenuation.y)
                ->setFloat(name + "quadratic", it->attenuation.x);
        }
        else
        {
            modelShader.setVec3(name + "ambient", 0.0f, 0.0f, 0.0f)
                ->setVec3(name + "diffuse", 0.0f, 0.0f, 0.0f)
                ->setVec3(name + "specular", 0.0f, 0.0f, 0.0f);
        }
    }
    // Spot light
    i = 0;
    for (auto it = spotLight->begin(); it != spotLight->end(); it++, i++)
    {
        std::string name = "spotLights[" + std::to_string(i) + "].";
        if (it->isEnable)
        {
            float diffuse = it->diffuse, specular = it->specular;
            modelShader.setVec3(name + "position", it->position)
                ->setVec3(name + "direction", it->direction)
                ->setVec3(name + "ambient", it->ambient)
                ->setVec3(name + "diffuse", diffuse, diffuse, diffuse)
                ->setVec3(name + "specular", specular, specular, specular)
                ->setFloat(name + "constant", it->attenuation.z)
                ->setFloat(name + "linear", it->attenuation.y)
                ->setFloat(name + "quadratic", it->attenuation.x)
                ->setFloat(name + "cutOff", glm::cos(glm::radians(it->cutOff)))
                ->setFloat(name + "outerCutOff", glm::cos((glm::radians(it->outerCutOff))));
        }
        else
        {
            modelShader.setVec3(name + "ambient", 0.0f, 0.0f, 0.0f)
                ->setVec3(name + "diffuse", 0.0f, 0.0f, 0.0f)
                ->setVec3(name + "specular", 0.0f, 0.0f, 0.0f);
        }
    }
}

void PreviewWidget::drawLight()
{
}
