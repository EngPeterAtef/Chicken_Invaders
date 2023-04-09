#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include <glm/gtx/euler_angles.hpp>

namespace our
{

    // This function computes and returns a matrix that represents this transform
    // Remember that the order of transformations is: Scaling, Rotation then Translation
    // HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
    glm::mat4 Transform::toMat4() const
    {
        // TODO: (Req 3) Write this function
        glm::mat4 mat = glm::translate(glm::mat4(1.0f), position) * glm::yawPitchRoll(rotation.y, rotation.x, rotation.z) * glm::scale(glm::mat4(1.0f), scale);
        // mat = glm::rotate(mat, rotation.x, glm::vec3(1, 0, 0));
        // mat = glm::rotate(mat, rotation.y, glm::vec3(0, 1, 0));
        // mat = glm::rotate(mat, rotation.z, glm::vec3(0, 0, 1));

        return mat;
    }

    // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json &data)
    {
        position = data.value("position", position);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale = data.value("scale", scale);
    }

}