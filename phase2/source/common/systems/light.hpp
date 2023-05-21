#pragma once

#include "../ecs/world.hpp"
#include "../components/light.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{
    class LightSystem
    {
    public:
        void update(World *world)
        {
            world->light_count = 0;
            for (auto entity : world->getEntities())
            {
                LightComponent *light = entity->getComponent<LightComponent>();
                if (light)
                {
                    world->lights[world->light_count].kind = light->kind;//directional light, point light, spot light
                    world->lights[world->light_count].position = glm::vec3(entity->getLocalToWorldMatrix() * glm::vec4(entity->localTransform.position, 1));//the position of the light in the world space
                    //the direction of the light in the world space (the roation in case of light represents the direction of the light)
                    world->lights[world->light_count].direction = glm::normalize(glm::vec3(entity->getLocalToWorldMatrix() * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)));
                    // world->lights[world->light_count].direction = glm::normalize(glm::vec3(glm::transpose(glm::inverse(entity->getLocalToWorldMatrix())) * glm::vec4(entity->localTransform.rotation, 0)));
                    //we multiply by the transpose of the inverse of the local to world matrix to get the direction in the world space
                    //because the direction of the light is a vector normal to its surface
                    world->lights[world->light_count].color = light->color; //the color of the material in the light
                    world->lights[world->light_count].attenuation = light->attenuation;//the attenuation of the light
                    world->lights[world->light_count].cone_angles = light->cone_angles;//the cone angles of the spot light
                    world->light_count++;//number of lights in the world
                }
            }
        }
    };

}
