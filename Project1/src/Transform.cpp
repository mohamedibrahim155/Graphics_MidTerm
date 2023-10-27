#include "Transform.h"

Transform::Transform()
{
   // this->modelMatrix = glm::mat4(1.0f);
    this->position = glm::vec3(0, 0, 0.0f);
    this->rotation = glm::vec3(0.0f, 0.00000001f, 0.000f);
    this->scale = glm::vec3(1.0f);
    this->rotaationAxis =0;
}

void Transform::SetTranslation(const glm::vec3& position)
{
    this->position += position;
  
}

void Transform::SetRotation(float angle ,const glm::vec3& rotation)
{
    this->rotaationAxis += angle;
    this->rotation = rotation;

   
}

void Transform::SetScale(const glm::vec3& scale)
{
    this->scale *= scale;
  
}

glm::mat4 Transform::getModelMatrix()
{
    glm::mat4 modelMat = glm::mat4(1.0f);

    modelMat = glm::translate(modelMat, position);
    modelMat = glm::rotate(modelMat,glm::radians(rotaationAxis),rotation);
    modelMat = glm::scale(modelMat, scale);
    return modelMat;
}

