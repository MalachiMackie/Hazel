#pragma once

#include "Cheezy.h"

class Sandbox2D : public Cheezy::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnStart() override;
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
	Cheezy::Ref<Cheezy::Scene2D> m_Scene;

	Cheezy::Ref<Cheezy::Texture2D> m_CheckerboardTexture;
	Cheezy::Ref<Cheezy::Texture2D> m_ChernoLogoTexture;

	Cheezy::Timestep m_LastTimeStep;
};