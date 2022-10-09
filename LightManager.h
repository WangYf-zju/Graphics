#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H

#define POINT_LIGHT_MAX 8
#define SPOT_LIGHT_MAX 8

#include <vector>
#include <glm/glm.hpp>

enum LIGHT_TYPE {
    DIRECTIONAL_LIGHT,
    POINT_LIGHT,
    SPOT_LIGHT,
};

struct LightObject {
    LIGHT_TYPE type;
    bool isEnable;
    bool isSelect;
    bool visible;
    glm::vec3 direction;
    glm::vec3 position;
    glm::vec3 ambient;
    float diffuse;
    float specular;
    glm::vec3 attenuation;
    float cutOff;
    float outerCutOff;
    glm::mat4 matrix;
};

class LightManager {
public:
    LightManager() 
    {
        _dirLight = LightObject{
            DIRECTIONAL_LIGHT,
            true,
            false,
            true,
            glm::vec3(-0.2f, -1.0f, -0.3f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.1f, 0.1f, 0.1f),
            0.5f,
            0.5f,
            glm::vec3(0.0f, 0.0f, 0.0f),
            0.0f,
            0.0f,
        };
        _pointLight.reserve(POINT_LIGHT_MAX);
        _spotLight.reserve(SPOT_LIGHT_MAX);
    }
    LightObject * getLight(int index)
    {
        if (index == 0) return &_dirLight;
        if (index > 0 && index <= _pointLight.size()) return &_pointLight[index - 1];
        if (index > _pointLight.size() && index <= _pointLight.size() + _spotLight.size())
            return &_spotLight[index - _pointLight.size() - 1];
        return nullptr;
    }
    bool addLight(LIGHT_TYPE type)
    {
        if (type == POINT_LIGHT && _pointLight.size() < POINT_LIGHT_MAX)
        {
            _pointLight.push_back(LightObject{
                POINT_LIGHT,
                true,
                false,
                true,
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(2.0f, 2.0f, 2.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                1.0f,
                1.0f,
                glm::vec3(0.032f, 0.09f, 1.0f),
                0.0f,
                0.0f,
                });
            return true;
        }
        else if (type == SPOT_LIGHT && _spotLight.size() < SPOT_LIGHT_MAX)
        {
            _spotLight.push_back(LightObject{
                SPOT_LIGHT,
                true,
                false,
                true,
                glm::vec3(3.0f, -3.0f, 0.0f),
                glm::vec3(-3.0f, 3.0f, 0.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                1.0f,
                1.0f,
                glm::vec3(0.032f, 0.09f, 1.0f),
                12.5f,
                15.0f,
                });
            return true;
        }
        return false;
    }
    //void updateLight(LightObject & light, int index)
    //{
    //    if (light.type == DIRECTIONAL_LIGHT) _dirLight = light;
    //    else if (light.type == POINT_LIGHT && index > 0 && index < _pointLight.size())
    //        _pointLight[index] = light;
    //    else if (light.type == SPOT_LIGHT && index > 0 && index < _spotLight.size())
    //        _spotLight[index] = light;
    //}
    void updateLight(LightObject & light, int index)
    {
        LightObject * l = getLight(index);
        if (l != nullptr) *l = light;
    }
    void removeLight(LIGHT_TYPE type, int index)
    {
        if (type == POINT_LIGHT && index > 0 && index < _pointLight.size())
            _pointLight.erase(_pointLight.begin() + index);
        else if (type == SPOT_LIGHT && index > 0 && index < _spotLight.size())
            _spotLight.erase(_spotLight.begin() + index);
    }
    void removeLight(int index)
    {
        LightObject * light = getLight(index);
        if (light != nullptr)
        {
            if (light->type == POINT_LIGHT)
                _pointLight.erase(_pointLight.begin() + index - 1);
            else if (light->type == SPOT_LIGHT)
                _spotLight.erase(_spotLight.begin() + index - _pointLight.size() - 1);
        }
    }
    LightObject * getDirectionalLight() { return &_dirLight; }
    std::vector<LightObject>* getPointLights() { return &_pointLight; }
    std::vector<LightObject>* getSpotLights() { return &_spotLight; }
    int lightCount() { return 1 + _pointLight.size() + _spotLight.size(); }
private:
    LightObject _dirLight;
    std::vector<LightObject> _pointLight;
    std::vector<LightObject> _spotLight;
};

#endif // !LIGHTMANAGER_H

