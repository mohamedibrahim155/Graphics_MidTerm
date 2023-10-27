#pragma once
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Transform
{
public:
	Transform();
	~Transform() {}

	glm::vec3 position;
	glm::vec3 rotation;
	float rotaationAxis;
	glm::vec3 scale;
	void SetTranslation(const glm::vec3& position);
	void SetRotation(float angle,const glm::vec3& rotation);
	void SetScale(const glm::vec3& scale);
	void SetTransform(glm::mat4& model, const  glm::vec3& position = glm::vec3(1), const  glm::vec3& rotation = glm::vec3(0.f, 0.f, 0.1f), float angle = 0, const glm::vec3& scale = glm::vec3(1));

	glm::mat4 getModelMatrix();
	glm::mat4 modelMatrix;
	void SetModelMatrix(const glm::mat4 modleMatrix);

private:

};

