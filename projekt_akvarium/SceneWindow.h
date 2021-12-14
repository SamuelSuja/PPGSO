#pragma once

//Headery z projektu:
#include "Scene.h"
#include "SceneWindow.h"

//STL kniznice:
#include <iostream>

//PPGSO kniznice:
#include <ppgso/ppgso.h>

//!Sirka okna
const unsigned int XSIZE = 1200;
//!Vyska okna
const unsigned int YSIZE = 800;

//!Trieda pre okno
class SceneWindow : public ppgso::Window
{
private:
    //!Hlavna scena
    Scene main_scene;

    //!Premenna ktora urci, ci chceme aby animacia bola spustena alebo nie
    bool animate = true;

    /*//
    void enableZBuffer();

    //
    void enablePolygonCulling();*/

    //!Vytvorenie sceny
    void initScene(int width, int height);

public:
    //!Konstruktor
    SceneWindow();

    /*!Stlacenie klavesy
    * @param key Key code stlacenej klavesy
    * @param scanCode Scan code stlacenej klavesy
    * @param action Stav klavesy
    * @param mods Modifiery naviac*/
    void onKey(int key, int scanCode, int action, int mods) override;

    //!Idle update
    void onIdle() override;
};