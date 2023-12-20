#include "first_app.hpp"
#include "sve_camera.hpp"
#include "sve_buffer.hpp"
#include "simple_render_system.hpp"
#include "keyboard_movement_controller.hpp"
#include <stdexcept>
#include <array>
#include <cassert>
#include <chrono>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

struct GlobalUbo {
	glm::mat4 projectionView{1.f};
	glm::vec3 lightDirection = glm::normalize(glm::vec3(1.f, -3.f, -1.f));
};

namespace sve {

	FirstApp::FirstApp() {
		globalPool = SveDescriptorPool::Builder(sveDevice)
			.setMaxSets(SveSwapChain::MAX_FRAMES_IN_FLIGHT)
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SveSwapChain::MAX_FRAMES_IN_FLIGHT)
			.build();

		loadGameObjects();
	}

	FirstApp::~FirstApp() {
	}

	void FirstApp::run() {
		std::vector<std::unique_ptr<SveBuffer>> uboBuffers(SveSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < uboBuffers.size(); i++) {
			uboBuffers[i] = std::make_unique <SveBuffer>(sveDevice,
				sizeof(GlobalUbo),
				1,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
			uboBuffers[i]->map();
		}

		auto globalSetLayout = SveDescriptorSetLayout::Builder(sveDevice)
			.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
			.build();

		std::vector<VkDescriptorSet> globalDescriptorSets(SveSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < globalDescriptorSets.size(); i++) {
			auto bufferInfo = uboBuffers[i]->descriptorInfo();
			SveDescriptorWriter(*globalSetLayout, *globalPool)
				.writeBuffer(0, &bufferInfo)
				.build(globalDescriptorSets[i]);
		}

		SimpleRenderSystem simpleRenderSystem(sveDevice, sveRenderer.getSwapChainRenderPass());
        SveCamera camera{};

        auto viewerObject = SveGameObject::createGameObject();
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();

		while (!sveWindow.shouldClose() ) {
			glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime; 

            frameTime = glm::min(frameTime, MAX_FRAME_TIME);

            cameraController.moveInPlaneXZ(sveWindow.getGLFWwindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = sveRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);
			if (auto commandBuffer = sveRenderer.beginFrame()) {
				int frameIndex = sveRenderer.getFrameIndex();
				FrameInfo frameInfo{
					frameIndex,
					frameTime,
					commandBuffer,
					camera
				};
				//update
				GlobalUbo ubo{};
				ubo.projectionView = camera.getProjection() * camera.getView();
				uboBuffers[frameIndex]->writeToBuffer(&ubo);
				uboBuffers[frameIndex]->flush();

				//render
				sveRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(frameInfo, gameObjects);
				sveRenderer.endSwapChainRenderPass(commandBuffer);
				sveRenderer.endFrame();
			}
		}
		vkDeviceWaitIdle(sveDevice.device());
	}

	void FirstApp::loadGameObjects() {
		std::shared_ptr<SveModel> sveCubeModel = SveModel::createModelFromFile(sveDevice, "models/colored_cube.obj");
		std::shared_ptr<SveModel> sveVaseModel = SveModel::createModelFromFile(sveDevice, "models/smooth_vase.obj");

        auto gameObj = SveGameObject::createGameObject();
        gameObj.model = sveCubeModel;
        gameObj.transform.translation = { .0f, .5f, 4.f };
		gameObj.transform.scale = glm::vec3(.5f);
        gameObjects.push_back(std::move(gameObj));
		auto gameObj2 = SveGameObject::createGameObject();
		gameObj2.model = sveVaseModel;
		gameObj2.transform.translation = { .1f, 0.f, 4.f };
		gameObj2.transform.scale = glm::vec3(2.f);
		gameObjects.push_back(std::move(gameObj2));
	}
}