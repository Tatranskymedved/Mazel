#include "mzpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Mazel
{
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::OpenGL:
				return new OpenGLVertexBuffer(vertices, size);
			case RendererAPI::None:
				MZ_CORE_ASSERT(false, "VertexBuffer::Create: Selected option of RendererAPI is currently not supported.")
					return nullptr;
		}

		MZ_CORE_ASSERT(false, "VertexBuffer::Create: Unknown RendererAPI.")
			return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indicies, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::OpenGL:
				return new OpenGLIndexBuffer(indicies, size);
			case RendererAPI::None:
				MZ_CORE_ASSERT(false, "IndexBuffer::Create: Selected option of RendererAPI is currently not supported.")
					return nullptr;
		}

		MZ_CORE_ASSERT(false, "IndexBuffer::Create: Unknown RendererAPI.")
			return nullptr;
	}

	void BufferLayout::CalculateOffsetsAndStride()
	{
		uint32_t offset = 0;
		m_Stride = 0;
		for (auto& element : m_Elements)
		{
			element.Offset = offset;
			offset += element.Size;
			m_Stride += element.Size;
		}
	}
}