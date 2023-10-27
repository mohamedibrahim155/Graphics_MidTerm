#pragma once

#include <iostream>
#include <glad/glad.h>
#include "VertexArray.h"
#include "Shader.h"


#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClrError();\
x;\
ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClrError();
bool GLLogCall(const char* function, const char* file, int line);


class Renderer
{
  public:


	  void Clear();

	  void Draw(const VertexArray& va, const Shader& shader) const;




};


