#include "SceneWindow.h"

//Headery z projektu:
#include "Camera.h"
#include "YellowFish.h"
#include "BlueFish.h"
#include "Aquarium.h"
#include "Snail.h"
#include "Palm.h"
#include "Castle.h"
#include "Coral.h"
#include "Water.h"

//!Vytvorenie sceny
void SceneWindow::initScene() {
    //Vyprazdnime zoznam objektov a kamier:
    main_scene.objects.clear();
    main_scene.cameras.clear();

    //Vytvorime kamery:
    auto camera = std::make_unique<Camera>();
    camera->setProjection(90.0f, (float)XSIZE/(float)YSIZE, 0.1f, 100.0f);
    camera->setView({0, 25, -25}, {0, 0, 0}, {0, 1, 0});
    main_scene.cameras.push_back(move(camera));
    //... dalsie kamery

    auto camera2 = std::make_unique<Camera>();
    camera2->setProjection(90.0f, (float)XSIZE/(float)YSIZE, 0.1f, 100.0f);
    camera2->setView({0, 7, -15}, {0, 7, 0}, {0, 1, 0});
    main_scene.cameras.push_back(move(camera2));
    //... dalsie kamery

    auto camera3 = std::make_unique<Camera>();
    camera3->setProjection(90.0f, (float)XSIZE/(float)YSIZE, 0.1f, 100.0f);
    camera3->setView({0, 20, 0}, {0, 0, 0}, {0, 0, 1});
    main_scene.cameras.push_back(move(camera3));
    //... dalsie kamery
    main_scene.currentCameraIndex = 2;

    auto castle = std::make_unique<Castle>();
    castle->position.x = -6;
    castle->position.y = 1.5;
    castle->position.z = 3;
    castle->rotation.x = ppgso::PI * 1.5f;
    castle->rotation.y = ppgso::PI * 1.0f;
    main_scene.objects.push_back(move(castle));

    auto palm = std::make_unique<Palm>();
    palm->position.x = -8.5f;
    palm->position.y = 1.5;
    palm->position.z = -2.5f;
    palm->rotation.x = ppgso::PI * 1.5f;
    main_scene.objects.push_back(move(palm));

    auto coral = std::make_unique<Coral>();
    coral->position.x = 7;
    coral->position.y = 1.5;
    coral->position.z = 1;
    coral->rotation.x = ppgso::PI * 1.5f;
    main_scene.objects.push_back(move(coral));

    //Pridame ryby:
    auto fish = std::make_unique<YellowFish>();
    fish->position.y = 10;
    fish->position.x = -3;
    fish->position.z = -3;
    fish->rotation.x = ppgso::PI * 1.5f;
    fish->rotation.y = ppgso::PI * 1.5f;
    fish->speed = glm::vec3(1.0f, 0.0f, 0.0f);
    main_scene.objects.push_back(move(fish));

    auto fish2 = std::make_unique<BlueFish>();
    fish2->position.y = 10;
    fish2->position.x = 3;
    fish2->position.z = 2;
    fish2->rotation.x = ppgso::PI * 1.5f;
    fish2->rotation.y = ppgso::PI * 1.0f;
    fish2->speed = glm::vec3(-1.0f, 0.0f, 0.0f);
    main_scene.objects.push_back(move(fish2));

    auto snail = std::make_unique<Snail>();
    snail->position.x = 1;
    snail->position.y = 1.5;
    snail->position.z = 4;
    snail->rotation.x = ppgso::PI * 1.0f;
    snail->rotation.y = ppgso::PI * 1.0f;
    main_scene.objects.push_back(move(snail));

    /*auto water = std::make_unique<Water>();
    water->position.y = 5;
    water->position.z = 0;
    main_scene.objects.push_back(move(water));*/

    auto aquarium = std::make_unique<Aquarium>();
    aquarium->position.y = 0;
    aquarium->position.z = 0;
    main_scene.objects.push_back(move(aquarium));
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

    //Nastavime blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);

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