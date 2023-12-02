#ifndef SIMPLE_RENDER_SYSTEM_HPP
#define SIMPLE_RENDER_SYSTEM_HPP

#include "sve_game_object.hpp"
#include "sve_pipeline.hpp"
#include "sve_device.hpp"

#include <memory>
#include <vector>

namespace sve {
	class SimpleRenderSystem {
	public:

		SimpleRenderSystem(SveDevice& device, VkRenderPass renderPass);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;


		void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<SveGameObject> &gameObjects);


	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);

		SveDevice& sveDevice;
		std::unique_ptr<SvePipeline> svePipeline;
		VkPipelineLayout pipelineLayout;
	};
}

#endif