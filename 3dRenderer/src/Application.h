#pragma once
#include <iostream>
#include "Graphics/Graphics.h"

class Application {
private:	
	Graphics graphicsInstance;

public:
	
	Application();
	~Application();

	int init();
	int run();
	int cleanup();
};