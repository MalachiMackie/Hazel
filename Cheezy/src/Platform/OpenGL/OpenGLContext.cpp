#include "hzpch.h"
#include "OpenGLContext.h"
#include "Cheezy/Core/Core.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Cheezy
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		CZ_CORE_ASSERT(windowHandle, "Window handle is null");
	}

	void OpenGLContext::Init()
	{
		CZ_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CZ_CORE_ASSERT(status, "Faild to initialize Glad!");

		CZ_CORE_INFO("OpenGL Info:");
		CZ_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		CZ_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		CZ_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

	#ifdef CZ_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		CZ_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Cheezy requires at least OpenGL version 4.5");
	#endif
	}

	void OpenGLContext::SwapBuffers()
	{
		CZ_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}