#pragma once
#include <vector>
#include "Renderer.h"

struct BufferElements
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized; 

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:          return 4;
		case GL_UNSIGNED_INT:   return 4;
		case GL_UNSIGNED_BYTE:  return 1;
		

		}
		//ASSERT(false);
		return 0;

	}


};



class VertexBufferLayout
{
public:
	VertexBufferLayout() :m_Stride(0) {}


	//~VertexBufferLayout();


	template<typename T>
	void Push(unsigned int count)
	{
		//GLCall(static_assert(false)); //should fix this
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT,count,GL_FALSE });
		m_Stride += count * BufferElements::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int >(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT,count,GL_FALSE });
		m_Stride += count * BufferElements::GetSizeOfType(GL_UNSIGNED_INT);

	}

	template<>
	void Push<unsigned char >(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE,count,GL_TRUE });
		m_Stride += count * BufferElements::GetSizeOfType(GL_UNSIGNED_BYTE);

	}



	inline const std::vector<BufferElements> GetElements() const& { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }





private:
	std::vector<BufferElements> m_Elements;
	unsigned int m_Stride;

};

