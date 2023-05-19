#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our
{
    class LightComponent : public Component
    {
    public:
        int kind;//the kind of the light
        glm::vec3 color;//the light color
        glm::vec3 attenuation;//1/(constant+linear*d+quadratic*d*d)
        glm::vec2 cone_angles;//the inner and outer cone angles
        static std::string getID() { return "LightComponent"; }

        void deserialize(const nlohmann::json &data) override;
    };

}