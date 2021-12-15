#pragma once

//Headery z projektu:
#include "Object.h"
#include "Hat.h"

//PPGSO kniznice:
#include <ppgso/ppgso.h>

class Snail final : public Object
{
private:
    //!Premenne potrebne na renderovanie objektu
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    //!Premenne potrebne pre animaciu a pohyb cez bezierovu krivku
    glm::vec3 begin_point = glm::vec3(1.0f, 1.5f, 4.0f);
    glm::vec3 mid_point = glm::vec3(1.0f, 1.5f, -4.0f);
    glm::vec3 end_point = glm::vec3(-5.0f, 1.5f, -4.0f);

    //!Maximalny cas animacie a aktualny cas animacie
    float animation_time = 25.0f;
    float current_time = 0.0f;

    //!Zistenie pozicie na bezierovej krivke
    glm::vec3 find_curve_coords(float t) const;

    //!Klobuk
    std::unique_ptr<Hat> hat;

public:
    //!Konstruktor
    Snail();

    /*!Updateneme poziciu slimaka
    * @param scene Scena, ktoru updatujeme
    * @param delta_time Delta cas
    * @return false pre zmazanie objektu*/
    bool update(Scene &scene, float delta_time) override;

    /*!Renderovanie objektu
    * @param scene Scena, v ktorej renderujeme*/
    void render(Scene &scene) override;
};