#ifndef BASEMODEL_H
#define BASEMODEL_H

namespace gm {

class AbstractModel {
public:
    virtual void render(float * vertex, unsigned int * indices, int start) = 0;
    virtual glm::mat4 getModelMatrix() = 0;
    MODEL_TYPE type;
};

template<typename _Derived>
class BaseModel : public AbstractModel
{
public:
    typedef _Derived Derived;
    typedef typename traits<_Derived>::Scalar Scalar;
    BaseModel()
        : _x(0)
        , _y(0)
        , _z(0)
        , _theta(0)
        , _psi(0)
        , _phi(0)
    { _model = glm::mat4((Scalar)1.0); }
    virtual Scalar * getVertex() = 0;
    virtual unsigned int * getIndices() = 0;
    virtual size_t vertexSize() = 0;
    virtual size_t indicesSize() = 0;

    //void transform(Scalar x, Scalar y, Scalar z, ...) = 0;

    inline virtual void render(float * vertex, unsigned int * indices, int start) override
    {
        if (this->indicesSize() > 0 && this->getIndices() != nullptr) {
            if (start == 0)
            {
                const size_t n = indicesSize() * sizeof(int);
                memcpy(indices, getIndices(), n);
            }
            else
            {
                unsigned int * p = getIndices();
                unsigned int * end = p + indicesSize();
                for (; p != end; p++, indices++) *indices = *p + start / EACH_VERTEX_SIZE;
            }
        }
        _render<Scalar>(vertex);
    }
    
    virtual _Derived * translate(Scalar x, Scalar y, Scalar z)
    {
        _x += x;
        _y += y;
        _z += z;
        updateModelMatrix();
        return static_cast<_Derived *>(this);
    }
    inline virtual _Derived * translateTo(Scalar x, Scalar y, Scalar z)
    {
        return translate(x - _x, y - _y, z - _z);
    }
    virtual _Derived * rotate(Scalar theta, Scalar psi, Scalar phi)
    {
        _theta += glm::radians(theta);
        _psi += glm::radians(psi);
        _phi += glm::radians(phi);
        
        /*if (theta != 0)
        {
            theta = glm::radians(theta);
            _theta += theta;
            _model = glm::rotate(_model, theta, glm::vec3((Scalar)1.0, (Scalar)0.0, (Scalar)0.0));
        }
        if (psi != 0)
        {
            psi = glm::radians(psi);
            _psi += psi;
            _model = glm::rotate(_model, psi, glm::vec3((Scalar)0.0, (Scalar)1.0, (Scalar)0.0));
        }
        if (phi != 0)
        {
            phi = glm::radians(phi);
            _phi += phi;
            _model = glm::rotate(_model, phi, glm::vec3((Scalar)0.0, (Scalar)0.0, (Scalar)1.0));
        }*/
        updateModelMatrix();
        return static_cast<_Derived *>(this);
    }
    virtual _Derived * rotateTo(Scalar theta, Scalar psi, Scalar phi) 
    {
        _theta = glm::radians(theta);
        _psi = glm::radians(psi);
        _phi = glm::radians(phi);
        updateModelMatrix();
        return static_cast<_Derived *>(this);
    }
    virtual _Derived * scaleTo(Scalar sx, Scalar sy, Scalar sz)
    {
        _sx = sx;
        _sy = sy;
        _sz = sz;
        updateModelMatrix();
        return static_cast<_Derived *>(this);
    }
    inline virtual glm::mat4 getModelMatrix()
    {
        return glm::mat4(_model); 
    }

protected:
    Scalar _x;
    Scalar _y;
    Scalar _z;
    Scalar _theta;
    Scalar _psi;
    Scalar _phi;
    Scalar _sx;
    Scalar _sy;
    Scalar _sz;
    glm::mat<4, 4, Scalar> _model;
    void updateModelMatrix()
    {
        glm::mat<4, 4, Scalar> model = glm::mat<4, 4, Scalar>((Scalar)1);
        glm::vec<3, Scalar> scale = glm::vec<3, Scalar>(_sx, _sy, _sz);
        glm::vec<3, Scalar> axisX = glm::vec<3, Scalar>((Scalar)1, (Scalar)0, (Scalar)0);
        glm::vec<3, Scalar> axisY = glm::vec<3, Scalar>((Scalar)0, (Scalar)1, (Scalar)0);
        glm::vec<3, Scalar> axisZ = glm::vec<3, Scalar>((Scalar)0, (Scalar)0, (Scalar)1);
        glm::vec<3, Scalar> trans = glm::vec<3, Scalar>(_x, _y, _z);
        model = glm::translate(model, trans);
        model = glm::rotate   (model, _theta, axisX);
        model = glm::rotate   (model, _psi  , axisY);
        model = glm::rotate   (model, _phi  , axisZ);
        model = glm::scale(model, scale);
        this->_model = model;
    }

private:
    template<typename T>
    void _render(float * buffer)
    {
        int count = vertexSize();
        float * end = buffer + count;
        T * vertex = getVertex();
        for (; buffer != end; buffer++, vertex++)
            *buffer = static_cast<float>(*vertex);
    }
    template<>
    void _render<float>(float * buffer)
    {
        const size_t n = vertexSize() * sizeof(float);
        memcpy(buffer, getVertex(), n);
    }
};    
} // namespace Graphic

#endif