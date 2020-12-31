#include "mzpch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>

namespace Mazel
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		MZ_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		MZ_CORE_ASSERT(status, "Failed to initialized Glad!");

		MZ_CORE_INFO("OpenGL Info:");
		MZ_CORE_INFO(" - Vendor:   {0}", glGetString(GL_VENDOR));
		MZ_CORE_INFO(" - Renderer: {0}", glGetString(GL_RENDERER));
		MZ_CORE_INFO(" - Version:  {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}