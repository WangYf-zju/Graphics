#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include "models/GraphicModel"
#include <vector>
#include <string>
#include <sstream>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTextStream>

#include "TextureManager.h"

#define INIT_VERTEX_SIZE 4096
#define INIT_ELEMENT_SIZE 4096
#define INIT_MODEL_MAXCOUNT 1000
#define X_AXIS_VERTEX_START 0
#define X_AXIS_VERTEX_COUNT 2
#define Y_AXIS_VERTEX_START 2
#define Y_AXIS_VERTEX_COUNT 2
#define Z_AXIS_VERTEX_START 4
#define Z_AXIS_VERTEX_COUNT 2
#define BOUNDING_BOX_VERTEX_START (6)
#define BOUNDING_BOX_VERTEX_COUNT 24
#define LIGHT_VERTEX_START 30
#define LIGHT_VERTEX_COUNT 16
#define CUBE_VERTEX_START 46
#define CUBE_VERTEX_COUNT 36
#define Z0_PLANE_VERTEX_START 82
#define Z0_PLANE_SIZE 5
#define Z0_PLANE_VERTEX_COUNT (Z0_PLANE_SIZE * 8 + 4) 

enum DRAW_METHOD {
    DIRECT_DRAW,
    INDICES_DRAW,
};

struct ModelObject {
    DRAW_METHOD method;
    glm::mat4 modelMatrix;
    int vertexStartIndex;
    int indicesStartIndex;
    int vertexSize;
    int indicesSize;
    unsigned int id;
    std::string name;
    gm::MODEL_TYPE type;
    bool visible;
    bool select;
    glm::vec4 color;
    std::string texture;
    int textureIndex;
    float modelTrans[9];
    float specular;
};

class ModelManager: public QObject {
public:
    ModelManager() 
    {
        _vertexBuffer.reserve(INIT_VERTEX_SIZE);
        _indicesBuffer.reserve(INIT_ELEMENT_SIZE);
        _models.reserve(INIT_MODEL_MAXCOUNT);
        initModelManager();
    };
    inline size_t size()
    {
        return _models.size();
    }
    inline float * adata()
    {
        return _vertexBuffer.data();
    }
    inline unsigned int * edata()
    {
        return _indicesBuffer.data();
    }
    inline size_t asize()
    {
        return _vertexBuffer.size() * sizeof(float);
    }
    inline size_t esize()
    {
        return _indicesBuffer.size() * sizeof(int);
    }
    inline size_t vcount()
    {
        return _indicesBuffer.size();
    }
    template<typename Derived>
    void addModel(gm::BaseModel<Derived> * model)
    {
        if (model != nullptr)
        {
            int mVertexStartIndex;
            int mIndicesStartIndex;
            int mVertexSize;
            int mIndicesSize;
            if (model->type == gm::MODEL_CUBE)
            {
                mVertexStartIndex = CUBE_VERTEX_START*EACH_VERTEX_SIZE;
                mIndicesStartIndex = 0;
                mVertexSize = model->vertexSize();
                mIndicesSize = model->indicesSize();
            }
            else
            {
                const size_t vertexOldSize = _vertexBuffer.size();
                const size_t vertexSize = vertexOldSize + model->vertexSize();
                const size_t indicesOldSize = _indicesBuffer.size();
                const size_t indicesSize = indicesOldSize + model->indicesSize();
                _vertexBuffer.resize(vertexSize);
                _indicesBuffer.resize(indicesSize);
                float * vertex = _vertexBuffer.data() + vertexOldSize;
                unsigned int * indices = _indicesBuffer.data() + indicesOldSize;

                model->render(vertex, indices, vertexOldSize);
                mVertexStartIndex = vertexOldSize;
                mIndicesStartIndex = indicesOldSize;
                mVertexSize = model->vertexSize();
                mIndicesSize = model->indicesSize();
            }

            std::stringstream name;
            name << "Model" << modelNameIndex;
            DRAW_METHOD method = INDICES_DRAW;
            if (model->indicesSize() <= 0 || model->getIndices() == nullptr)
                method = DIRECT_DRAW;
            ModelObject m = {
                method,
                model->getModelMatrix(),
                mVertexStartIndex,
                mIndicesStartIndex,
                mVertexSize,
                mIndicesSize,
                modelNameIndex,
                name.str(),
                model->type,
                true,
                false,
                glm::vec4(1.0f, 0.5f, 0.2f, 1.0f),
                "",
                -1,
                {
                    model->getXF(), 
                    model->getYF(), 
                    model->getZF(), 
                    model->getThetaF(), 
                    model->getPhiF(), 
                    model->getPsiF(), 
                    model->getSxF(), 
                    model->getSyF(), 
                    model->getSzF() 
                },
                0.4,
            };
            _models.push_back(m);
            modelNameIndex++;
        }
    }
    void removeModel(int index)
    {
        if (index >= 0 && index < this->_models.size())
        {
            const auto it = _models.begin() + index;
            const auto nextIt = it + 1;
            const auto start = _indicesBuffer.begin() + it->indicesStartIndex;
            
            if (nextIt == _models.end())
            {
                _indicesBuffer.erase(start, _indicesBuffer.end());
            }
            else
            {
                const auto nextStart = _indicesBuffer.begin() + nextIt->indicesStartIndex;
                int diff = nextIt->indicesStartIndex - it->indicesStartIndex;
                for (auto p = nextIt; p != _models.end(); p++)
                    p->indicesStartIndex -= diff;
                _indicesBuffer.erase(start, nextStart);
            }
            _models.erase(it);
        }
    }
    std::vector<ModelObject> * getModelVector()
    {
        return &_models;
    }

    ModelObject & getModelObj(int index)
    {
        return _models[index];
    }

    ModelObject * getModelObjById(unsigned int id)
    {
        auto p = _models.begin();
        while (p != _models.end())
            if (p->id == id) return &(*p);
            else p++;
        return nullptr;
    }
    void changeModelTexture(unsigned int id, unsigned int textureIndex, std::string filepath)
    {
        ModelObject * model = getModelObjById(id);
        if (model != nullptr)
        {
            model->textureIndex = textureIndex;
            model->texture = filepath;
        }
    }

    template<typename Derived>
    void updateModel(int index, gm::BaseModel<Derived> * model, bool sizeChange = false)
    {
        if (index >= 0 && index < this->_models.size())
        {
            if (sizeChange)
            {
                removeModel(index);
                addModel(model);
            }
            else
            {
                const auto it = _models.begin() + index;
                const vertex = _vertexBuffer.data() + it->vertexStartIndex;
                const indices = _indicesBuffer.data() + it->indicesStartIndex;
                model->render(vertex, indices, it->vertexStartIndex);
            }
        }
    }

    void updateModelObject(ModelObject * model)
    {
        if (model != nullptr)
        {
            ModelObject * old = getModelObjById(model->id);
            if (old != nullptr) *old = *model;
        }
    }

    void save_json_file(QString filename, TextureManager *t)
    {
        QJsonObject  modelObj;
        QJsonArray name;
        for (int i = 0; i < _models.size(); i++)
        {
            QJsonObject model;
            QJsonArray a;
            ModelObject m = _models[i];

            for (int j = 0; j < 9; j++)
            {
                a.append(m.modelTrans[j]);
            }
            QString originPath = t->getOriginPath(m.textureIndex);
            model.insert("inf", a);
            model.insert("visible", m.visible);
            model.insert("type", m.type);
            model.insert("texture", originPath);
            model.insert("specular", m.specular);
            modelObj.insert(QString::fromStdString(m.name), model);
            name.append(QString::fromStdString(m.name));
        }
        modelObj.insert("name", name);
        QJsonDocument doc;
        doc.setObject(modelObj);

        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
            return;

        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        stream << doc.toJson();
        file.close();

    }

    void load_json_file(QString filename, TextureManager *t, QWidget * parent = nullptr)
    {
        initModelManager();
        QFile file(filename);
        if (!file.open(QFile::ReadOnly | QFile::Text))
            return;
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        QString str = stream.readAll();

        file.close();
        QJsonParseError jsonError;
        QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8(), &jsonError);
        if (jsonError.error != QJsonParseError::NoError && !doc.isNull())
            return;
        QJsonObject modelObj = doc.object();
        QJsonValue nameValue = modelObj.value("name");
        if (nameValue.type() == QJsonValue::Array)
        {
            QJsonArray nameArray = nameValue.toArray();
            for (int i = 0; i < nameArray.size(); i++)
            {
                QJsonValue name = nameArray.at(i);
                QJsonValue modelValue = modelObj.value(name.toString());
                QJsonObject model = modelValue.toObject();
                QJsonValue arrayvalue = model.value("inf");
                QJsonValue type = model.value("type");
                QJsonValue texture = model.value("texture");
                QJsonValue specular = model.value("specular");
                QJsonValue visible = model.value("visible");

                QString texturePath = t->addTexture(texture.toString());
                int textureIndex = t->getTextureIndex(texturePath);
                QJsonArray a = arrayvalue.toArray();
                float inf[9];
                for (int j = 0; j < a.size(); j++)
                {
                    QJsonValue vf = a.at(j);
                    inf[j] = vf.toDouble();
                }
                if (type.toInt() == 0)
                {
                    this->addModel(new gm::Cube<float>(inf[0], inf[1], inf[2], inf[6], inf[7], inf[8]));
                    auto m = _models.rbegin();
                    m->specular = specular.toDouble();
                    m->texture = texturePath.toStdString();
                    m->textureIndex = textureIndex;
                    m->name = name.toString().toStdString();
                    m->visible = visible.toBool();
                }
                else if (type.toInt() == 2)
                {
                    this->addModel(new gm::Sphere<float>(inf[0], inf[1], inf[2], inf[8]));
                    auto m = _models.rbegin();
                    m->specular = specular.toDouble();
                    m->texture = texturePath.toStdString();
                    m->textureIndex = textureIndex;
                    m->name = name.toString().toStdString();
                    m->visible = visible.toBool();
                }
                else if (type.toInt() == 1)
                {
                    this->addModel(new gm::Cylinder<float>(inf[0], inf[1], inf[2], inf[6], inf[7]));
                    auto m = _models.rbegin();
                    m->specular = specular.toDouble();
                    m->texture = texturePath.toStdString();
                    m->textureIndex = textureIndex;
                    m->name = name.toString().toStdString();
                    m->visible = visible.toBool();
                }
            }
        }
    }

    void initModelManager()
    {
        _models.clear();
        _vertexBuffer.clear();
        _indicesBuffer.clear();
        addAxisVertex();
        addBoundingBoxVertex();
        addLightVertex();
        addCubeVertex();
        addZ0PlaneVertex();
    }

private:
    std::vector<ModelObject> _models;
    std::vector<float> _vertexBuffer;
    std::vector<unsigned int> _indicesBuffer;
    static unsigned int modelNameIndex;

    void addAxisVertex()
    {
        float axisVertex[] = {
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            1000.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1000.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1000.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        };
        _vertexBuffer.insert(_vertexBuffer.end(), axisVertex, axisVertex + sizeof(axisVertex) / sizeof(float));
    }

    void addZ0PlaneVertex()
    {
        for (int x = -Z0_PLANE_SIZE; x <= Z0_PLANE_SIZE; x++)
        {
            _vertexBuffer.push_back((float)x);
            _vertexBuffer.push_back(0.0f);
            _vertexBuffer.push_back((float)-Z0_PLANE_SIZE);
            for (int i = 3; i < EACH_VERTEX_SIZE; i++)
                _vertexBuffer.push_back(0.0f);
            _vertexBuffer.push_back((float)x);
            _vertexBuffer.push_back(0.0f);
            _vertexBuffer.push_back((float)Z0_PLANE_SIZE);
            for (int i = 3; i < EACH_VERTEX_SIZE; i++)
                _vertexBuffer.push_back(0.0f);
        }
        for (int z = -5; z <= 5; z++)
        {
            _vertexBuffer.push_back((float)-Z0_PLANE_SIZE);
            _vertexBuffer.push_back(0.0f);
            _vertexBuffer.push_back((float)z);
            for (int i = 3; i < EACH_VERTEX_SIZE; i++)
                _vertexBuffer.push_back(0.0f);
            _vertexBuffer.push_back((float)Z0_PLANE_SIZE);
            _vertexBuffer.push_back(0.0f);
            _vertexBuffer.push_back((float)z);
            for (int i = 3; i < EACH_VERTEX_SIZE; i++)
                _vertexBuffer.push_back(0.0f);
        }
    }

    void addBoundingBoxVertex()
    {
        float boxVertex[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        };
        _vertexBuffer.insert(_vertexBuffer.end(), boxVertex, boxVertex + sizeof(boxVertex) / sizeof(float));
    }

    void addLightVertex()
    {
        float lightVertex[] = {
            -0.1f,  0.0f,  0.1f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
             0.1f,  0.0f,  0.1f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
             0.1f,  0.0f,  0.1f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
             0.1f,  0.0f, -0.1f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
             0.1f,  0.0f, -0.1f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.1f,  0.0f, -0.1f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.1f,  0.0f, -0.1f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.1f,  0.0f,  0.1f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.1f,  0.0f,  0.1f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
             0.0f, -0.1f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
             0.1f,  0.0f,  0.1f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
             0.0f, -0.1f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
             0.1f,  0.0f, -0.1f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
             0.0f, -0.1f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.1f,  0.0f, -0.1f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
             0.0f, -0.1f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        };
        _vertexBuffer.insert(_vertexBuffer.end(), lightVertex, lightVertex + sizeof(lightVertex) / sizeof(float));
    }

    void addCubeVertex()
    {
        float cubeVertex[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
        };
        _vertexBuffer.insert(_vertexBuffer.end(), cubeVertex, cubeVertex + sizeof(cubeVertex)/sizeof(float));
    }
};

#endif

