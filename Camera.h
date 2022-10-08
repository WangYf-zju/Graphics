#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
public:
    Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f), 
        float yaw = 0.0f, 
        float pitch = 0.0f) 
        : _position(position)
        , _yaw(yaw)
        , _pitch(pitch)
        , _front(-position)
        , _zoom(45.0f)
    {
        _worldup = glm::vec3(0.0f, 1.0f, 0.0f);
        updateCameraVectors();
    }
    glm::vec3 getPosition() { return _position; }
    glm::vec3 getFront() { return _front; }

    glm::mat4 GetViewMatrix() { return _view; }
    float getZoom() { return _zoom; }
    void translate(float dx, float dz)
    {
        _position.x += dx;
        _position.z += dz;
        updateCameraVectors();
    }
    void rotate(float dx, float dy)
    {
        _yaw += dx;
        _pitch += dy;
        if (_pitch > 89.0f)
            _pitch = 89.0f;
        if (_pitch < -89.0f)
            _pitch = -89.0f;
        updateCameraVectors();
    }
    void zoom(float d)
    {
        _position += _front * d;
        updateCameraVectors();
    }

private:
    glm::vec3 _position, _front, _up, _right;
    glm::vec3 _worldup;
    float _yaw, _pitch, _zoom;
    glm::mat4 _view;
    void updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
        front.y = sin(glm::radians(_pitch));
        front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
        _front = glm::normalize(front);
        _right = glm::normalize(glm::cross(_front, _worldup));
        _up = glm::normalize(glm::cross(_right, _front));
        _view = glm::lookAt(_position, _position + _front, _up);
    }
};

#endif