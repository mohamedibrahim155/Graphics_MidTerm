#pragma once
#include<glm/glm.hpp>
#include "Shader.h"
#include "Model.h"
#include <vector>

enum LightType
{
    DIRECTION_LIGHT =0,
    POINT_LIGHT=1,
    SPOT_LIGHT =2
};

class Light
{
public:
    Light();
    ~Light() { };

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 direction;
    LightType lightType;

    Model* lightModel;
    //for pointLight
    float constant;
    float linear;
    float quadratic;


    //spot Lights
    float cutOffAngle;
    float outerCutOffAngle;

    GLint cuttOffAngle_UL;
    GLint outerCutOffAngle_UL;
    GLint quadratic_UL;
    GLint linear_UL;
    GLint constant_UL;
    GLint lightType_UL;
    GLint direction_UL;
    GLint specular_UL;
    GLint diffuse_UL;
    GLint ambient_UL;
    GLint position_UL;



private:

};







