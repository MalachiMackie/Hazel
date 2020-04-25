#include "hzpch.h"
#include "GraphicsContext.h"

#include "Cheezy/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Cheezy
{
	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    CZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
			default:						CZ_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}
}
