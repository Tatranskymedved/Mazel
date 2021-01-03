#pragma once

#include "mzpch.h"

#include "RenderCommand.h"
#include "RendererAPI.h"

namespace Mazel
{
	class Renderer
	{
	public:
		static void BeginScene();
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);
		static void Draw(const RenderCommand& cmd);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
	};
}