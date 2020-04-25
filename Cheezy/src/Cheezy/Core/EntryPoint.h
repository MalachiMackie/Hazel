#pragma once
#include "Cheezy/Core/Core.h"
#include "Cheezy/Core/Application.h"
#include "Cheezy/Core/Log.h"
#include "Cheezy/Debug/Instumentor.h"

#ifdef CZ_PLATFORM_WINDOWS

extern Cheezy::Application* Cheezy::CreateApplication();

int main(int argc, char** argv)
{
	Cheezy::Log::Init();

	CZ_PROFILE_BEGIN_SESSION("Startup", "CheezyProfile-Startup.json");
	auto app = Cheezy::CreateApplication();
	CZ_PROFILE_END_SESSION();

	CZ_PROFILE_BEGIN_SESSION("Runtime", "CheezyProfile-Runtime.json");
	app->Run();
	CZ_PROFILE_END_SESSION();

	CZ_PROFILE_BEGIN_SESSION("Shutdown", "CheezyProfile-Shutdown.json");
	delete app;
	CZ_PROFILE_END_SESSION();
}

#endif