#include "SceneWindow.h"

//Headery z projektu:
#include "Camera.h"
#include "YellowFish.h"
#include "BlueFish.h"
#include "Aquarium.h"
#include "Snail.h"
#include "Grass.h"
#include "Castle.h"
#include "Coral.h"

//!Vytvorenie sceny
void SceneWindow::initScene() {
    //Vyprazdnime zoznam objektov a kamier:
    main_scene.objects.clear();
    main_scene.cameras.clear();

    //Vytvorime kamery:
    auto camera = std::make_unique<Camera>();
    camera->setProjection(90.0f, (float)XSIZE/(float)YSIZE, 0.1f, 100.0f);
    camera->setView({0, 0, -20}, {0, 0, 0}, {0, 1, 0});
    main_scene.cameras.push_back(move(camera));
    //... dalsie kamery
    main_scene.currentCameraIndex = 0;

    //Pridame ryby:
    /*auto fish = std::make_unique<YellowFish>();
    fish->position.y = 5;
    fish->rotation.y = ppgso::PI * 0.5f;
    fish->rotation.x = ppgso::PI * 1.5f;
    fish->rotation.z = ppgso::PI * 1.5f;
    main_scene.objects.push_back(move(fish));

    auto fish2 = std::make_unique<BlueFish>();
    fish2->position.y = -10;
    fish2->rotation.y = ppgso::PI * 0.5f;
    fish2->rotation.x = ppgso::PI * 1.5f;
    fish2->rotation.z = ppgso::PI;
    main_scene.objects.push_back(move(fish2));*/

    /*auto aquarium = std::make_unique<Aquarium>();
    aquarium->rotation.x = ppgso::PI * 1.5f;
    aquarium->position.z = 20;
    main_scene.objects.push_back(move(aquarium));*/

    /*auto snail = std::make_unique<Snail>();
    snail->position.y = -10;
    snail->rotation.y = ppgso::PI * 0.5f;
    snail->rotation.x = ppgso::PI * 1.5f;
    snail->rotation.z = ppgso::PI;
    main_scene.objects.push_back(move(snail));

    auto grass = std::make_unique<Grass>();
    grass->position.y = -10;
    main_scene.objects.push_back(move(grass));*/

    /*auto castle = std::make_unique<Castle>();
    castle->position.y = -10;
    castle->rotation.x = ppgso::PI * 1.5f;
    castle->rotation.y = ppgso::PI;
    main_scene.objects.push_back(move(castle));*/

    /*auto coral = std::make_unique<Coral>();
    coral->position.y = -10;
    main_scene.objects.push_back(move(coral));*/

}

//!Konstruktor
SceneWindow::SceneWindow() : ppgso::Window{"Projekt Akvarium", XSIZE, YSIZE} {
    //Nastavime input mode pre GLFW
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

    //Nastavime Z-buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    //Nastavime polygon culling
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    //Inicializujeme scenu
    initScene();
}

/*!Stlacenie klavesy
* @param key Key code stlacenej klavesy
* @param scanCode Scan code stlacenej klavesy
* @param action Stav klavesy
* @param mods Modifiery naviac*/
void SceneWindow::onKey(int key, int scanCode, int action, int mods)
{
    //Predchadzajuca kamera
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        main_scene.prevCamera();
    }

    //Nasledujuca kamera
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        main_scene.nextCamera();
    }
}

//!Idle update
void SceneWindow::onIdle()
{
    //Zistime cas a vypocitame delta cas:
    static auto time = (float) glfwGetTime();
    float delta_time = animate ? (float) glfwGetTime() - time : 0;
    time = (float) glfwGetTime();

    //Nastavime pozadie cez OpenGL
    glClearColor(.5f, .5f, .5f, 0);
    //Clearneme color a depth buffery
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Update a render sceny
    main_scene.update(delta_time);
    main_scene.render();
}