#include "pch.h"

#include "Core/Application.h"

int main(int argc, char* argv[])
{
	auto app = Application::GetInstance();
	app->Init();
	app->LoadDemo();

	app->Run();

	return EXIT_SUCCESS;
}
