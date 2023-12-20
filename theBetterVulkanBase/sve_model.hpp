#ifndef SVE_MODEL
#define SVE_MODEL

#include "sve_device.hpp"


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "sve_buffer.hpp"

#include <memory>
#include <vector>

namespace sve {
	class SveModel {
	public:

		struct Vertex {
			glm::vec3 position{};
			glm::vec3 color{};
			glm::vec3 normal{};
			glm::vec2 uv{};

			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();

			bool operator==(const Vertex& other) const {
				return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
			}
		};

		struct Builder {
			std::vector<Vertex> vertices{};
			std::vector<uint32_t> indices{};

			void loadModel(const std::string& filepath);
		};

		SveModel(SveDevice & device, const SveModel::Builder &builder);
		~SveModel();

		SveModel(const SveModel&) = delete;
		SveModel& operator=(const SveModel&) = delete;

		static std::unique_ptr<SveModel> createModelFromFile(SveDevice& device, const std::string& filepath);

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);

	private:
		void createIndexBuffer(const std::vector<uint32_t>& indices);
		void createVertexBuffers(const std::vector<Vertex>& vertices);

		SveDevice& sveDevice;

		std::unique_ptr<SveBuffer> vertexBuffer;
		uint32_t vertexCount;

		bool hasIndexBuffer = false;
		std::unique_ptr<SveBuffer> indexBuffer;
		uint32_t indexCount;
	};
}

#endif