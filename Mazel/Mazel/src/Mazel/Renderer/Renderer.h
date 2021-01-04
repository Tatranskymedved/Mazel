#pragma once

#include "mzpch.h"

#include "RenderCommand.h"
#include "RendererAPI.h"
#include "Shader.h"
#include "OrtographicCamera.h"

namespace Mazel
{
	class Renderer
	{
	public:
		static void BeginScene(OrtographicCamera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);
		static void Draw(const RenderCommand& cmd);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};
}