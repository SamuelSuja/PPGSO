#pragma once

#include "Object.h"
#include "AquariumGround.h"
#include "AquariumCorner.h"
#include "AquariumWallLong.h"
#include "AquariumWallShort.h"

#include <ppgso/ppgso.h>

#include <list>

class Aquarium final : public Object
{
private:
    std::list<std::unique_ptr<Object>> components;

    //Skalovanie
    float scale_amount;

public:
    //!Konstruktor
    Aquarium();

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