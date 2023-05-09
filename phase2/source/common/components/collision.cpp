#include "collision.hpp"
#include "../asset-loader.hpp"
#include "../deserialize-utils.hpp"
#include "../ecs/entity.hpp"
#include "../mesh/mesh.hpp"
#include "mesh-renderer.hpp"

namespace our
{
// reads the mesh from the json object and populates the min and max vertices
void CollisionComponent::deserialize(const nlohmann::json &data)
{
    if (!data.is_object())
        return;
    mesh = AssetLoader<Mesh>::get(data["mesh"].get<std::string>());
    health = data.value("health", bonus);
    minCollider = mesh->minvertex;
    maxCollider = mesh->maxvertex;
}

} // namespace our