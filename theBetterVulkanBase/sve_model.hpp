#ifndef SVE_MODEL
#define SVE_MODEL

#include "sve_device.hpp"


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace sve {
	class SveModel {
	public:

		struct Vertex {
			glm::vec2 position;
			glm::vec3 color;

			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
		};

		SveModel(SveDevice & device, const std::vector<Vertex> &vertices);
		~SveModel();

		SveModel(const SveModel&) = delete;
		SveModel& operator=(const SveModel&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);

	private:void createVertexBuffers(const std::vector<Vertex>& vertices);


		SveDevice& sveDevice;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;
	};
}

#endif