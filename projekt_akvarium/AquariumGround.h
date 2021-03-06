#pragma once

//Headery z projektu:
#include "Object.h"

//PPGSO kniznice:
#include <ppgso/ppgso.h>

class AquariumGround final : public Object
{
private:
    //!Premenne potrebne na renderovanie objektu
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

public:
    //!Konstruktor
    AquariumGround();

    /*!Updateneme poziciu spodku akvaria
    * @param scene Scena, ktoru updatujeme
    * @param delta_time Delta cas
    * @return false pre vymazanie objektu
    */
    bool update(Scene &scene, float delta_time) override;

    /*!Renderovanie objektu
    * @param scene Scena, v ktorej renderujeme*/
    void render(Scene &scene) override;

    //!Lokalna pozicia v akvariu
    glm::vec3 local_position = {0.0f, 0.0f, 0.0f};
};