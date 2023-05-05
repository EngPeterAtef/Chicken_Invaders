#include "world.hpp"
#include <iostream>
namespace our
{

    // This will deserialize a json array of entities and add the new entities to the current world
    // If parent pointer is not null, the new entities will be have their parent set to that given pointer
    // If any of the entities has children, this function will be called recursively for these children
    void World::deserialize(const nlohmann::json &data, Entity *parent)
    {
        if (!data.is_array())
            return;
        for (const auto &entityData : data)
        {
            // TODO: (Req 8) Create an entity, make its parent "parent" and call its deserialize with "entityData".
            Entity *entity = add(); // to create an entity and add it to the world
            entity->parent = parent;
            entity->deserialize(entityData);
            // if the entity name is enemy repeat it many times in different positions
            // if (entity->name == "enemy")
            // {
            //     for (int i = 0; i < 100; i++)
            //     {
            //         Entity *newEntity = add();
            //         newEntity->parent = parent;
            //         newEntity->deserialize({{"name", "enemy"},
            //                                 {"rotation", {0, 0, 0}},
            //                                 {"position", {((1 + i / 2) ^ 2) % 10, -0.3, -15 - i * 5}},
            //                                 {"scale", {0.4, 0.4, 0.4}},
            //                                 {"components",
            //                                  {{{"type", "Mesh Renderer"}, {"mesh", "monkey"}, {"material", "monkey"}},
            //                                   {{"type", "Collision"}, {"mesh", "monkey"}},
            //                                   {{"type", "Movement"}, {"angularVelocity", {0, 100, 0}}}}}});
            //     }
            // }
            // data is map of maps
            // entityData is map
            // if there is a key called "children" in the map so it has children
            if (entityData.contains("children"))
            {
<<<<<<< HEAD
                // TODO: (Req 8) Recursively call this world's "deserialize" using the children data
                //  and the current entity as the parent
                deserialize(entityData["children"], entity);
=======
                Entity *newEntity = add();
                newEntity->parent = parent;
                newEntity->deserialize({{"name", "enemy"},
                                        {"rotation", {0, 0, 0}},
                                        {"position", {((1 + i / 2) ^ 2) % 10, -0.3, -15 - i * 5}},
                                        {"scale", {0.4, 0.4, 0.4}},
                                        {"components",
                                         {{{"type", "Mesh Renderer"}, {"mesh", "monkey"}, {"material", "monkey"}},
                                          {{"type", "Collision"}, {"mesh", "monkey"}},
                                          {{"type", "Movement"}, {"angularVelocity", {0, 100, 0}}}}}});
>>>>>>> c2235502f178af8811e710d7af67d8371e6f5f97
            }
            std::cout << "finish world deserialize\n"
                      << std::endl;
        }
    }

} // namespace our