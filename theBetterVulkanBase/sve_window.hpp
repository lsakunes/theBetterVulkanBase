#ifndef SVE_WINDOW_HPP
#define SVE_WINDOW_HPP
 // !SVE_WINDOW

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
namespace sve {
	class SveWindow {

	public:
		SveWindow(int w, int h, std::string name);
		~SveWindow();

		bool shouldClose() { return glfwWindowShouldClose(window); }

		SveWindow(const SveWindow&) = delete;
		SveWindow &operator=(const SveWindow&) = delete;

	private:

		void initWindow();

		const int width;
		const int height;

		std::string windowName;
		GLFWwindow* window;
	};
}

#endif