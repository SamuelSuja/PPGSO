#pragma once

#include "Object.h"

#include <ppgso/ppgso.h>

class Snail final : public Object
{
private:
    //!Premenne potrebne na renderovanie objektu
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    glm::vec3 begin_point = glm::vec3(1.0f, 1.5f, 4.0f);
    glm::vec3 mid_point = glm::vec3(1.0f, 1.5f, -4.0f);
    glm::vec3 end_point = glm::vec3(-5.0f, 1.5f, -4.0f);
    float animation_time = 25.0f;
    float current_time = 0.0f;

    glm::vec3 find_curve_coords(float t) const;

public:
    //!Konstruktor
    Snail();

    /*!Updateneme poziciu ryby
    * @param scene Scena, ktoru updatujeme
    * @param delta_time Delta cas
    * @return true to delete the object
    */
    bool update(Scene &scene, float delta_time) override;

    /*!Renderovanie objektu
    * @param scene Scena, v ktorej renderujeme*/
    void render(Scene &scene) override;
};