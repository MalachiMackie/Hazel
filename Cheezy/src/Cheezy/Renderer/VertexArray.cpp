#include "hzpch.h"
#include "VertexArray.h"

#include "Cheezy/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Cheezy
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	CZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexArray>();
			default:						CZ_CORE_ASSERT(false, "Unknown RendererAPI") return nullptr;
		}
	}
}