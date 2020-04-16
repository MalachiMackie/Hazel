#pragma once

#include "Hazel.h"

class Sandbox2D : public Hazel::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Hazel::Timestep ts) override;
	void OnEvent(Hazel::Event& e) override;
	void OnImGuiRender() override;

private:
	Hazel::OrthographicCameraController m_CameraController;

	Hazel::Ref<Hazel::Texture2D> m_CheckerboardTexture;
	Hazel::Ref<Hazel::Texture2D> m_ChernoLogoTexture;

	glm::vec4 m_BackgroundTint = glm::vec4(1.0f);
	glm::vec4 m_SquareColor = glm::vec4(0.55f, 0.2f, 0.8f, 1.0f);
	float m_SquareRotation = 45.0f;
};