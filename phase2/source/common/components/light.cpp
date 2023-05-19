#include "./light.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"
namespace our
{
    void LightComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;
        kind = data.value("kind", 0);//default value is directional light
        color = data.value("color", glm::vec3(1.0f));//default color is white
        attenuation = data.value("attenuation",  glm::vec3(1.0f));//default attenuation is 1
        cone_angles = glm::radians(data.value("cone_angles", glm::vec2(0.0f)));//default cone angles is 0
    }
}