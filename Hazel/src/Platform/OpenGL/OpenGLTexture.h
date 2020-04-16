#pragma once

#include "Hazel/Renderer/Texture.h"

#include <string>
#include <glad/glad.h>

namespace Hazel
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void SetData(void* data, uint32_t size) override;
		virtual uint32_t GetRendererId() const override { return m_RendererId; }

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool operator==(const Texture& other) const override { return m_RendererId == other.GetRendererId(); }

	private:
		const std::string& m_Path;

		uint32_t m_Height;
		uint32_t m_Width;

		uint32_t m_RendererId;

		GLenum m_InternalFormat, m_DataFormat;
	};
}