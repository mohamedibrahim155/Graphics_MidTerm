#pragma once
#include "Shader.h"





class Material
{

private:

	
	float shininess;
	

public:

	


	Material(float shininess);
	~Material();


	void SetMaterialProperties(Shader& shader) ;
	



};