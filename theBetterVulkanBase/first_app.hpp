#ifndef FIRST_APP_HPP
#define FIRST_APP_HPP

#include "sve_window.hpp"

namespace sve {
	class FirstApp {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void run();

	private:
		SveWindow sveWindow{ WIDTH, HEIGHT, "Hello Vulkan" };
	};
}

#endif