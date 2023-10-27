#include "Transform.h"



void Transform::translate(const glm::vec3& translation) {
    position += translation;
}

void Transform::Scalemat(const glm::vec3& scaling) {
    scale *= scaling;
}

void Transform::rotate(float angle, const glm::vec3& axis) {
    rotationAngle += angle;
    rotationAxis = axis;
}

glm::mat4 Transform::getModelMatrix() const {
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, scale);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle), rotationAxis);

    return modelMatrix;
}