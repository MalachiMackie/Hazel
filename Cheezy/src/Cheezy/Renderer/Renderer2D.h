#pragma once

#include "Cheezy/Core/Scene2D.h"
#include "Cheezy/Renderer/OrthographicCamera.h"
#include "Cheezy/Renderer/Texture.h"

#include <glm/glm.hpp>

namespace Cheezy
{
	class Renderer2D
	{
	public:
		Renderer2D() = delete;

		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void BeginScene(const Ref<Scene2D>& scene);
		static void EndScene();
		static void Flush();

		// Primitives
		
		static void DrawQuad(const glm::vec2& position, const glm::vec2& dimensions, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& dimensions, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& dimensions, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& dimensions, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& color = glm::vec4(1.0f));

		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& dimensions, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& dimensions, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& dimensions, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& dimensions, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		static Ref<Texture2D> WhiteTexture;

		//Stats
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};

		static void ResetStats();
		static Statistics GetStats();

	private:
		static void FlushAndReset();
	};
}