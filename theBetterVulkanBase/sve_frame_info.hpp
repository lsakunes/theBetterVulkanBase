#ifndef SVE_FRAME_INFO_HPP
#define SVE_FRAME_INFO_HPP

#include "sve_camera.hpp"

#include <vulkan/vulkan.h>

namespace sve {
	struct FrameInfo {
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		SveCamera& camera;
	};
}

#endif