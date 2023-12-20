#ifndef FIRST_APP_HPP
#define FIRST_APP_HPP

#include "sve_window.hpp"
#include "sve_game_object.hpp"
#include "sve_renderer.hpp"
#include "sve_device.hpp"
#include "sve_descriptors.hpp"

#include <memory>
#include <vector>

namespace sve {
	class FirstApp {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp& operator=(const FirstApp&) = delete;

		void run();

	private:
		void loadGameObjects();

		SveWindow sveWindow{ WIDTH, HEIGHT, "Hello Vulkan" };
		SveDevice sveDevice{ sveWindow };
		SveRenderer sveRenderer{ sveWindow, sveDevice };

		std::unique_ptr<SveDescriptorPool> globalPool{};
		std::vector<SveGameObject> gameObjects;
		float MAX_FRAME_TIME{ 0.04f };
	};
}

#endif