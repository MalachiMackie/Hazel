#pragma once

#include "Cheezy.h"

class ExampleLayer : public Cheezy::Layer
{
public:
	ExampleLayer();

	void OnUpdate(Cheezy::Timestep ts) override;
	void OnEvent(Cheezy::Event& e) override;
	void OnImGuiRender() override;

private:
	void SquareMovement(Cheezy::Timestep ts);

private:
	Cheezy::ShaderLibrary m_ShaderLibrary;

	Cheezy::Ref<Cheezy::VertexArray> m_TriVA;
	Cheezy::Ref<Cheezy::VertexArray> m_SquareVA;

	Cheezy::Ref<Cheezy::Texture2D> m_CheckerboardTexture, m_ChernoLogoTexture;

	Cheezy::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquarePos;
	float m_SquareMoveSpeed = 2.0f;
	float m_SquareRotateSpeed = 90.0f;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};