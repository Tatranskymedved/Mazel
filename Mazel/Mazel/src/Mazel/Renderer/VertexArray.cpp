#include "mzpch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Mazel
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::OpenGL:
				return new OpenGLVertexArray();
			case RendererAPI::None:
				MZ_CORE_ASSERT(false, "VertexArray::Create: Selected option of RendererAPI is currently not supported.");
				return nullptr;
		}

		MZ_CORE_ASSERT(false, "VertexArray::Create: Unknown RendererAPI.");
		return nullptr;
	}
}