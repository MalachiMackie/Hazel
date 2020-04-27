#pragma once

#include "Cheezy.h"

class Sandbox2D : public Cheezy::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	/*
		Runs 50 times per second.
		Will run multiple times per rendered frame if it has to
	*/
	void OnFixedUpdate() override;
	void OnUpdate(Cheezy::Timestep ts) override;
	void OnEvent(Cheezy::Event& e) override;
	void OnImGuiRender() override;

private:
	Cheezy::Scene2D m_Scene;

	Cheezy::Ref<Cheezy::Texture2D> m_CheckerboardTexture;
	Cheezy::Ref<Cheezy::Texture2D> m_ChernoLogoTexture;

	glm::vec4 m_BackgroundTint = glm::vec4(1.0f);
	glm::vec4 m_SquareColor = glm::vec4(0.55f, 0.2f, 0.8f, 1.0f);
	float m_SquareRotation = 45.0f;
	float m_SquareStep = 0.5f;

	Cheezy::Timestep m_LastTimeStep;
};