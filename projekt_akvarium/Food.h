#pragma once

//Headery z projektu:
#include "Object.h"

//PPGSO kniznice:
#include <ppgso/ppgso.h>

class Food final : public Object
{
private:
    //!Premenne potrebne na renderovanie objektu
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

public:
    //!Kmitanie:
    glm::vec3 oscillate_direction = glm::vec3(0.0f, 0.0f, 0.0f);

    //!Vektor rychlosti
    glm::vec3 speed;

    //!Konstruktor
    Food();

    /*!Updateneme poziciu jedla
    * @param scene Scena, ktoru updatujeme
    * @param delta_time Delta cas
    * @return false pre zmazanie objektu*/
    bool update(Scene &scene, float delta_time) override;

    /*!Renderovanie objektu
    * @param scene Scena, v ktorej renderujeme*/
    void render(Scene &scene) override;
};