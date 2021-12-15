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

//!Vytvorenie sceny
void SceneWindow::initScene(int width, int height)
{
    //Ulozenie informacii o velkosti sceny
    main_scene.width = width;
    main_scene.height = height;

    //Vyprazdnime zoznam objektov a kamier
    main_scene.objects.clear();
    main_scene.cameras.clear();

    //Vytvorime kamery:
    //Kamera 1
    auto camera = std::make_unique<Camera>();
    camera->setProjection(90.0f, (float)XSIZE/(float)YSIZE, 0.1f, 100.0f);
    camera->setView({0, 20, -20}, {0, 0, 0}, {0, 1, 0});
    main_scene.cameras.push_back(move(camera));

    //Kamera 2
    auto camera2 = std::make_unique<Camera>();
    camera2->setProjection(90.0f, (float)XSIZE/(float)YSIZE, 0.1f, 100.0f);
    camera2->setView({0, 7, -15}, {0, 7, 0}, {0, 1, 0});
    main_scene.cameras.push_back(move(camera2));

    //Kamera 3
    auto camera3 = std::make_unique<Camera>();
    camera3->setProjection(90.0f, (float)XSIZE/(float)YSIZE, 0.1f, 100.0f);
    camera3->setView({0, 20, 0}, {0, 0, 0}, {0, 0, 1});
    main_scene.cameras.push_back(move(camera3));

    //Kamera 4
    auto camera4 = std::make_unique<Camera>();
    camera4->setProjection(90.0f, (float)XSIZE/(float)YSIZE, 0.1f, 100.0f);
    camera4->setView({0, 7, -6}, {0, 0, 0}, {0, 1, 1});
    main_scene.cameras.push_back(move(camera4));

    //Kamera 5
    auto camera5 = std::make_unique<Camera>();
    camera5->setProjection(90.0f, (float)XSIZE/(float)YSIZE, 0.1f, 100.0f);
    camera5->setView({0, 10, -8}, {0, 7, 0}, {0, 1, 0});
    main_scene.cameras.push_back(move(camera5));

    //Kamera 6
    auto camera6 = std::make_unique<Camera>();
    camera6->setProjection(90.0f, (float)XSIZE/(float)YSIZE, 0.1f, 100.0f);
    camera6->setView({0, 10, -18}, {0, 9, 0}, {0, 1, 0});
    main_scene.cameras.push_back(move(camera6));

    //Kamera 7
    auto camera7 = std::make_unique<Camera>();
    camera7->setProjection(90.0f, (float)XSIZE/(float)YSIZE, 0.1f, 100.0f);
    camera7->setView({-10, 14, -15}, {0, 5, 0}, {0, 1, 0});
    main_scene.cameras.push_back(move(camera7));

    //Kamera 8
    auto camera8 = std::make_unique<Camera>();
    camera8->setProjection(90.0f, (float)XSIZE/(float)YSIZE, 0.1f, 100.0f);
    camera8->setView({-10, 5, -5}, {0, 5, 0}, {0, 1, 0});
    main_scene.cameras.push_back(move(camera8));

    //Kamera 9
    auto camera9 = std::make_unique<Camera>();
    camera9->setProjection(90.0f, (float)XSIZE/(float)YSIZE, 0.1f, 100.0f);
    camera9->setView({5, 10, -5}, {0, 8, 0}, {0, 1, 0});
    main_scene.cameras.push_back(move(camera9));

    //Kamera 10
    auto camera10 = std::make_unique<Camera>();
    camera10->setProjection(90.0f, (float)XSIZE/(float)YSIZE, 0.1f, 100.0f);
    camera10->setView({10, 20, 0}, {0, 0, 0}, {0, 1, 0});
    main_scene.cameras.push_back(move(camera10));

    //Index aktualnej kamery
    main_scene.currentCameraIndex = 0;

    //Objekt hradu
    auto castle = std::make_unique<Castle>();
    castle->position.x = -6;
    castle->position.y = 1.5;
    castle->position.z = 3;
    castle->rotation.x = ppgso::PI * 1.5f;
    castle->rotation.y = ppgso::PI * 1.0f;
    main_scene.objects.push_back(move(castle));

    //Objekt palmy
    auto palm = std::make_unique<Palm>();
    palm->position.x = -8.5f;
    palm->position.y = 1.5;
    palm->position.z = -2.5f;
    palm->rotation.x = ppgso::PI * 1.5f;
    main_scene.objects.push_back(move(palm));

    //Objekt koralu
    auto coral = std::make_unique<Coral>();
    coral->position.x = 7;
    coral->position.y = 1.5;
    coral->position.z = 1;
    coral->rotation.x = ppgso::PI * 1.5f;
    main_scene.objects.push_back(move(coral));

    //Objekty ryb:
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

    //Objekt slimaka
    auto snail = std::make_unique<Snail>();
    snail->position.x = 1;
    snail->position.y = 1.5 + 5;
    snail->position.z = 4;
    snail->rotation.x = ppgso::PI * 1.0f;
    snail->rotation.y = ppgso::PI * 1.0f;
    main_scene.objects.push_back(move(snail));

    //Objekt akvaria
    auto aquarium = std::make_unique<Aquarium>();
    aquarium->position.y = 0;
    aquarium->position.z = 0;
    main_scene.objects.push_back(move(aquarium));
}

//!Konstruktor
SceneWindow::SceneWindow() : ppgso::Window{"Projekt Akvarium", XSIZE, YSIZE}
{
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
    initScene(XSIZE, YSIZE);
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
        main_scene.prev_camera();
    }

    //Nasledujuca kamera
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        main_scene.next_camera();
    }

    //F = Pridanie jedla
    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        main_scene.create_new_food();
    }

    //L = Zmena svetla
    if (key == GLFW_KEY_L && action == GLFW_PRESS) {
        main_scene.change_lights();
    }

    //P = Zmena post processing filteru
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        main_scene.change_post_processing();
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