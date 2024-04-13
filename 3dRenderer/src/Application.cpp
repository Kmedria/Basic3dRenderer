#include "Application.h";

Application::Application() {}
Application::~Application() {}

int Application::init() {
	graphicsInstance = Graphics();
	return graphicsInstance.init();
}

int Application::run() {
	graphicsInstance.run();
	return 0;
}

int Application::cleanup() {
	graphicsInstance.cleanup();
	return 0;
}
