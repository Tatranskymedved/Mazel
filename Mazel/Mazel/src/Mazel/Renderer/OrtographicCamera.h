#pragma once

#include <glm/glm.hpp>

namespace Mazel
{
	class OrtographicCamera
	{
	public:
		OrtographicCamera(float left, float right, float bottom, float top, float zNear = -1.0f, float zFar = 1.0f);

		const glm::vec3& GetPosition() const { return m_Position; }
		float GetRotation() const { return m_Rotation; }

		void SetPosition(const glm::vec3& position);
		void SetRotation(float rotation);

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	private:
		void RecalculateViewMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
	};
}

