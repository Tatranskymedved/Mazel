#include "mzpch.h"
#include "WindowsWindow.h"

#include "Mazel/Events/ApplicationEvent.h"
#include "Mazel/Events/MouseEvent.h"
#include "Mazel/Events/KeyEvent.h"

#include <glad/glad.h>

namespace Mazel
{
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error_code, const char* description)
	{
		MZ_CORE_ERROR("GLFW Error ({0}): {1}", error_code, description);
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		MZ_CORE_INFO("Creating window {0} ({1}, {2})", m_Data.Title, m_Data.Width, m_Data.Height);

		if (!s_GLFWInitialized)
		{
			//TODO: glfwTerminate on system shut
			int isInit = glfwInit();
			MZ_CORE_ASSERT(isInit, "Could not initialize GLFW!");

			glfwSetErrorCallback(GLFWErrorCallback);

			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		MZ_CORE_ASSERT(status, "Failed to initialized Glad!");
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

#pragma region [GLFW callbacks]
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent ev(width, height);
			data.EventCallback(ev);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent ev;
			data.EventCallback(ev);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent ev(key, 0);
					data.EventCallback(ev);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent ev(key);
					data.EventCallback(ev);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent ev(key, 1);
					data.EventCallback(ev);
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int codepoint)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent ev(codepoint);
			data.EventCallback(ev);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent ev(button);
					data.EventCallback(ev);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent ev(button);
					data.EventCallback(ev);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent ev((float)xOffset, (float)yOffset);
			data.EventCallback(ev);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent ev((float)xPos, (float)yPos);
			data.EventCallback(ev);
		});
#pragma endregion [GLFW callbacks]

	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
}