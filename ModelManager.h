#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include "models/GraphicModel"
#include <vector>
#include <string>

#define INIT_VERTEX_SIZE 4096
#define INIT_ELEMENT_SIZE 4096
#define INIT_MODEL_MAXCOUNT 64
#define X_AXIS_VERTEX_START 0
#define X_AXIS_VERTEX_COUNT 2
#define Y_AXIS_VERTEX_START 2
#define Y_AXIS_VERTEX_COUNT 2
#define Z_AXIS_VERTEX_START 4
#define Z_AXIS_VERTEX_COUNT 2
#define BOUNDING_BOX_VERTEX_START (6 * sizeof(float))
#define BOUNDING_BOX_VERTEX_COUNT 24
#define Z0_PLANE_VERTEX_START (30 * sizeof(float))
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
};

class ModelManager {
public:
    ModelManager() 
    {
        _vertexBuffer.reserve(INIT_VERTEX_SIZE);
        _indicesBuffer.reserve(INIT_ELEMENT_SIZE);
        _models.reserve(INIT_MODEL_MAXCOUNT);
        addAxisVertex();
        addBoundingBoxVertex();
        addZ0PlaneVertex();
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
            const size_t vertexOldSize = _vertexBuffer.size();
            const size_t vertexSize = vertexOldSize + model->vertexSize();
            const size_t indicesOldSize = _indicesBuffer.size();
            const size_t indicesSize = indicesOldSize + model->indicesSize();
            float * vertex = _vertexBuffer.data() + _vertexBuffer.size();
            unsigned int * indices = _indicesBuffer.data() + _indicesBuffer.size();
            _vertexBuffer.resize(vertexSize);
            _indicesBuffer.resize(indicesSize);
            model->render(vertex, indices, vertexOldSize);

            std::stringstream name;
            name << "Model" << modelNameIndex;
            DRAW_METHOD method = INDICES_DRAW;
            if (model->indicesSize() <= 0 || model->getIndices() == nullptr)
                method = DIRECT_DRAW;
            ModelObject m = {
                method,
                model->getModelMatrix(),
                vertexOldSize,
                indicesOldSize,
                model->vertexSize(),
                model->indicesSize(),
                modelNameIndex,
                name.str(),
                model->type,
                true,
                false,
                glm::vec4(1.0f, 0.5f, 0.2f, 1.0f),
                "",
                -1,
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
        _vertexBuffer.insert(_vertexBuffer.end(), axisVertex, axisVertex + sizeof(axisVertex));
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
        _vertexBuffer.insert(_vertexBuffer.end(), boxVertex, boxVertex + sizeof(boxVertex));
    }
};

#endif

