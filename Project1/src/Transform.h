#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transform {
public:

    Transform() {
        position = glm::vec3(0.0f, -5.0f, 0.0f);
        scale = glm::vec3(0.5f, 0.5f, 0.5f);
        rotationAngle = 0.0f;
        rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    glm::vec3 position;
    glm::vec3 scale;
    float rotationAngle;
    glm::vec3 rotationAxis;

    void translate(const glm::vec3& translation);
    void Scalemat(const glm::vec3& scaling);
    void rotate(float angle, const glm::vec3& axis);

    glm::mat4 getModelMatrix() const;

private:
    
};