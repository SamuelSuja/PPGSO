#pragma once

//Headery z projektu:
#include "Object.h"
#include "Sunglasses.h"

//PPGSO kniznice:
#include <ppgso/ppgso.h>

class Hat final : public Object
{
private:
    //!Premenne potrebne na renderovanie objektu
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    //!Okuliare
    std::unique_ptr<Sunglasses> sunglasses;

public:
    //!Konstruktor
    Hat();

    /*!Updateneme poziciu klobuku
    * @param scene Scena, ktoru updatujeme
    * @param delta_time Delta cas
    * @return false pre zmazanie objektu*/
    bool update(Scene &scene, float delta_time) override;

    /*!Renderovanie objektu
    * @param scene Scena, v ktorej renderujeme*/
    void render(Scene &scene) override;
};