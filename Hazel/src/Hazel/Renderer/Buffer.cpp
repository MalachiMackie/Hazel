#include "hzpch.h"
#include "Buffer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Renderer.h"

namespace Hazel
{
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, float* vertices)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLVertexBuffer>(size, vertices);
			default:						HZ_CORE_ASSERT(false, "Unknown RendererAPI") return nullptr;
		}
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t count, uint32_t* indices)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:	HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
			case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLIndexBuffer>(count, indices);
			default:						HZ_CORE_ASSERT(false, "Unknown RendererAPI") return nullptr;
		}
	}
}