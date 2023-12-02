#ifndef SVE_GAME_OBJECT_HPP
#define SVE_GAME_OBJECT_HPP

#include "sve_model.hpp"

#include <memory>

namespace sve{

	struct Transform2dComponent {
		glm::vec2 translation{};
		glm::vec2 scale{1.f, 1.f};
		float rotation{};
		
		glm::mat2 mat2() {
			const float s = glm::sin(rotation);
			const float c = glm::cos(rotation);
			glm::mat2 rotMat{{c, s}, { -s, c }};
			glm::mat2 scaleMat{{scale.x, 0.f}, { .0f, scale.y }}; 
			return rotMat * scaleMat;
		}
	};

	class SveGameObject {
	public:
		using id_t = unsigned int;

		static SveGameObject createGameObject() {
			static id_t currentId = 0;
			return SveGameObject{ currentId++ };
		}

		SveGameObject(const SveGameObject&) = delete;
		SveGameObject& operator=(const SveGameObject&) = delete;
		SveGameObject(SveGameObject&&) = default;
		SveGameObject& operator=(SveGameObject&&) = default;

		const id_t getId() { return id; }

		std::shared_ptr<SveModel> model{};
		glm::vec3 color{};
		Transform2dComponent transform2d;

	private:

		SveGameObject(id_t objId) : id(objId) {}

		id_t id;
	};
}

#endif