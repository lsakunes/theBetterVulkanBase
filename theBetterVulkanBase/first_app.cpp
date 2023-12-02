#include "first_app.hpp"
#include "simple_render_system.hpp"
#include <stdexcept>
#include <array>
#include <cassert>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace sve {

	FirstApp::FirstApp() {
		loadGameObjects();
	}

	FirstApp::~FirstApp() {
	}

	void FirstApp::run() {
		SimpleRenderSystem simpleRenderSystem(sveDevice, sveRenderer.getSwapChainRenderPass());

		while (!sveWindow.shouldClose() ) {
			glfwPollEvents();

			if (auto commandBuffer = sveRenderer.beginFrame()) {
				sveRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
				sveRenderer.endSwapChainRenderPass(commandBuffer);
				sveRenderer.endFrame();
			}
		}
		vkDeviceWaitIdle(sveDevice.device());
	}


	void FirstApp::loadGameObjects() {
		std::vector<SveModel::Vertex> vertices{
			{{0.0f, -0.5f}, { 1.0f, 0.0f, 0.0f }},
			{{0.5f, 0.5f}, { 0.0f, 0.0f, 0.0f } },
			{{-0.5f, 0.5f}, { 0.0f, 0.0f, 1.0f } }
		};

		auto sveModel = std::make_shared<SveModel>(sveDevice, vertices);

		auto triangle = SveGameObject::createGameObject();
		triangle.model = sveModel;
		triangle.color = { .1f, .8f, .1f };
		triangle.transform2d.translation.x = .2f;
		triangle.transform2d.scale = { 2.f, .5f };
		triangle.transform2d.rotation = .25f * glm::two_pi<float>();

		gameObjects.push_back(std::move(triangle));
	}
}