#ifndef BASEMODEL_H
#define BASEMODEL_H

namespace GraphicModel {

template<typename Derived>
class BaseModel 
{
public:
    typedef typename traits<Derived>::Scalar Scalar;
    BaseModel() { _model = glm::mat4((Scalar)1.0); }
    virtual Scalar * getVertex() = 0;
    virtual int * getIndices() = 0;
    virtual int vertexSize() = 0;
    virtual int indicesSize() = 0;
    
    virtual Derived * translate(Scalar x, Scalar y, Scalar z)
    {
        _model = glm::translate(_model, glm::vec3(x, y, z));
        _x += x;
        _y += y;
        _z += z;
        return static_cast<Derived *>(this);
    }
    virtual Derived * translateTo(Scalar x, Scalar y, Scalar z)
    {
        return translate(x - _x, y - _y, z - _z);
    }
    virtual Derived * rotate(Scalar theta, Scalar psi, Scalar phi)
    {
        if (theta != 0)
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
        }
        return static_cast<Derived *>(this);
    }
    virtual Derived * rotateTo(Scalar theta, Scalar psi, Scalar phi) 
    {
        return rotate(theta - _theta, psi - _psi, phi - _phi);
    }
    virtual glm::mat4 getModelMatrix() { return _model; }

protected:
    Scalar _x;
    Scalar _y;
    Scalar _z;
    Scalar _theta;
    Scalar _psi;
    Scalar _phi;
    glm::mat4 _model;
};    
} // namespace Graphic

#endif