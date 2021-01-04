#include "mzpch.h"
#include "OrtographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Mazel
{
	OrtographicCamera::OrtographicCamera(float left, float right, float bottom, float top, float zNear, float zFar)
		:m_ProjectionMatrix(glm::ortho<float>(left, right, bottom, top, zNear, zFar)), m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrtographicCamera::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
		RecalculateViewMatrix();
	}

	void OrtographicCamera::SetRotation(float rotationDegrees)
	{
		m_Rotation = rotationDegrees;
		RecalculateViewMatrix();
	}

	void OrtographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}