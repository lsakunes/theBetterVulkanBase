#include "first_app.hpp"

namespace sve {

	void FirstApp::run() {
		while (!sveWindow.shouldClose()) {
			glfwPollEvents();
		}
	}
}