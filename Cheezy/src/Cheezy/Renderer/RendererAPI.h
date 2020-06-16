#pragma once

#include "Cheezy/Renderer/VertexArray.h"

#include <glm/glm.hpp>

namespace Cheezy
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1
		};
	public:
		virtual void Init() = 0;
		virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;

		static inline API GetAPI() { return s_API; }
		static Scope<RendererAPI> Create();
		
	private:
		static API s_API;
	};
}