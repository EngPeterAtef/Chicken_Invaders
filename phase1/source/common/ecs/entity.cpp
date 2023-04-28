#include "entity.hpp"
#include "../deserialize-utils.hpp"
#include "../components/component-deserializer.hpp"

#include <glm/gtx/euler_angles.hpp>

namespace our
{

    // This function returns the transformation matrix from the entity's local space to the world space
    // Remember that you can get the transformation matrix from this entity to its parent from "localTransform"
    // To get the local to world matrix, you need to combine this entities matrix with its parent's matrix and
    // its parent's parent's matrix and so on till you reach the root.
    glm::mat4 Entity::getLocalToWorldMatrix() const
    {
        // TODO: (Req 8) Write this function
        // the line is creating a 4x4 transformation matrix that represents the transformation 
        //from a local coordinate system to a world coordinate system
        glm::mat4 localToWorld = localTransform.toMat4();
        //  if(parent != nullptr){
        //      localToWorld = parent->getLocalToWorldMatrix() * localToWorld;
        //  }
        //  return localToWorld;
        // currentEntity is initially set to the parent entity of the current entity.
        Entity *currentEntity = parent;
        while (currentEntity != nullptr)
        {
            //transformation from local space to world space by multiplying 
            localToWorld = currentEntity->localTransform.toMat4() * localToWorld;
            currentEntity = currentEntity->parent;
        }
        // std::cout << "localToWorld: " << localToWorld.length << std::endl;
        return localToWorld;
    }

    // Deserializes the entity data and components from a json object
    void Entity::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;
        name = data.value("name", name);
        localTransform.deserialize(data);
        if (data.contains("components"))
        {
            if (const auto &components = data["components"]; components.is_array())
            {
                for (auto &component : components)
                {
                    deserializeComponent(component, this);
                }
            }
        }
    }

}