#pragma once

#include "Object.h"

#include <ppgso/ppgso.h>

class AquariumWallLong final : public Object
{
private:
    //!Premenne potrebne na renderovanie objektu
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

public:
    //!Konstruktor
    AquariumWallLong();

    /*!Updateneme poziciu ryby
    * @param scene Scena, ktoru updatujeme
    * @param delta_time Delta cas
    * @return true to delete the object
    */
    bool update(Scene &scene, float delta_time) override;

    /*!Renderovanie objektu
    * @param scene Scena, v ktorej renderujeme*/
    void render(Scene &scene) override;

    glm::vec3 local_position = {0.0f, 0.0f, 0.0f};
};