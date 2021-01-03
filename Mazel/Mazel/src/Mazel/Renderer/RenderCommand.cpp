#include "mzpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Mazel
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}