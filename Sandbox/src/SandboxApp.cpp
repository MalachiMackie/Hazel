#include "Sandbox2D.h"
#include "ExampleLayer.h"

#include <Cheezy.h>
#include <Cheezy/Core/EntryPoint.h>
#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Sandbox : public Cheezy::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(Cheezy::CreateRef<Sandbox2D>());
	}

	~Sandbox()
	{

	}
};

Cheezy::Application* Cheezy::CreateApplication()
{
	return new Sandbox();
}