#pragma once

//Headery z projektu:
#include "Object.h"
#include "AquariumGround.h"
#include "AquariumCorner.h"
#include "AquariumWallLong.h"
#include "AquariumWallShort.h"


#include <ppgso/ppgso.h>

//Headery z projektu:
#include <list>

class Aquarium final : public Object
{
private:
    //!Komponenty
    std::list<std::unique_ptr<Object>> components;

public:
    //!Skalovanie
    float scale_amount;

    //!Konstruktor
    Aquarium();

    /*!Updateneme poziciu akvaria
    * @param scene Scena, ktoru updatujeme
    * @param delta_time Delta cas
    * @return false pre zmazanie objektu
    */
    bool update(Scene &scene, float delta_time) override;

    /*!Renderovanie objektu
    * @param scene Scena, v ktorej renderujeme*/
    void render(Scene &scene) override;
};