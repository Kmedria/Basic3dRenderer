#include "Application.h"

int main() {
    Application ThreeDRenderer = Application();
    int applicationStatus = ThreeDRenderer.init();
    if (0 == applicationStatus) {
        applicationStatus = ThreeDRenderer.run();
        if (0 == applicationStatus) {
            applicationStatus = ThreeDRenderer.cleanup();
            if (0 != applicationStatus) {
                std::cout << "An error has occurred during the application cleanup process. Error Code: " << applicationStatus << std::endl;
            }
            return 0;
        }
        else {
            std::cout << "An error has occurred during the application running process. Error Code: " << applicationStatus << std::endl;
        }
    }
    else {
        std::cout << "An error has occurred during the application initialisation process. Error Code: " << applicationStatus << std::endl;
    }
}