#ifndef SVE_RENDERER_HPP
#define SVE_RENDERER_HPP

#include "sve_device.hpp"
#include "sve_window.hpp"
#include "sve_swap_chain.hpp"

#include <memory>
#include <vector>
#include <cassert>

namespace sve {
	class SveRenderer {
	public:
		
		SveRenderer(SveWindow& window, SveDevice & device);
		~SveRenderer();

		SveRenderer(const SveRenderer&) = delete;
		SveRenderer& operator=(const SveRenderer&) = delete;

		VkRenderPass getSwapChainRenderPass() const { return sveSwapChain->getRenderPass(); }
		float getAspectRatio() const {
			return sveSwapChain->extentAspectRatio();
		}
		bool isFrameInProgress() const { return isFrameStarted; }
		
		VkCommandBuffer getCurrentCommandBuffer() const { 
			assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
			return commandBuffers[currentFrameIndex];
		}

		int getFrameIndex() const {
			assert(isFrameStarted && " Cannot get frame index when frame not in progress");
			return currentFrameIndex;
		}

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

	private:
		void createCommandBuffers();
		void recreateSwapChain();
		void freeCommandBuffers();

		SveWindow& sveWindow;
		SveDevice& sveDevice;
		std::unique_ptr<SveSwapChain> sveSwapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentImageIndex;
		int currentFrameIndex;
		bool isFrameStarted;
	};
}

#endif