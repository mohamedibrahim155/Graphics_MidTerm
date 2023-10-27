#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{

public:


	VertexArray();
	~VertexArray();

	void CreateBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);


	void Bind() const;
	void UnBind() const;



private:
	unsigned int m_rendID;





};
#endif