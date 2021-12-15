#pragma once

//Headery z projektu:
#include "Object.h"
#include "Camera.h"

//STL kniznice:
#include <iostream>
#include <list>

//!Trieda pre scenu
class Scene {
public:
    //!Velkost obrazovky
    int width, height;

    //!Update objektov
    void update(float time);

    //!Renderovanie objektov
    void render();

    //!Zoznam (vektor) kamier
    std::vector< std::unique_ptr<Camera> > cameras;

    //!Aktualna kamera
    int currentCameraIndex = 0;

    //!Pozicie svetiel
    glm::vec3 light_positions[3] = {{-10.0f, 20.0f, 0.0f}, {0.0f, 20.0f, 0.0f}, {10.0f, 20.0f, 0.0f}};

    //!Farby svetiel
    glm::vec3 light_colors[3] = {{0.3f, 0.3f, 0.3f}, {0.3f, 0.3f, 0.3f}, {0.3f, 0.3f, 0.3f}};

    //!Premenna pre urcenie, ktore svetla su pouzivane:
    int light_mode = 0;

    //!Premenna pre urcenie, ktory post processing efekt je pouzivany:
    int post_processing_mode = 0;

    //!Zoznam objektov
    std::list< std::unique_ptr<Object> > objects;

    //!Presuny medzi kamerami
    void prev_camera();
    void next_camera();

    //!Pridanie castice bubliny
    static bool create_new_bubble();

    //!Pridanie castic jedla
    void create_new_food();

    //!Vytvorenie nahodneho vektoru
    static glm::vec3 randomize_vec3(float min, float max, char type);

    //!Zmena svetiel
    void change_lights();

    //!Animacia pohybu svetla
    void animate_light(float delta_time);
    bool animate_lights = false;
    glm::vec3 light_begin_point = glm::vec3(-10.0f, 5.0f, 0.0f);
    glm::vec3 light_end_point = glm::vec3(10.0f, 5.0f, 0.0f);
    float light_animation_time = 3.0f;
    float light_current_time = 0.0f;

    //!Zmena post processing efektu
    void change_post_processing();

    //!Zistenie pozicie na bezierovej krivke
    glm::vec3 find_curve_coords(float t) const;

    //!Animacia pohybu kamery
    void animate_cameras(float delta_time);
    glm::vec3 camera_begin_point = glm::vec3(-10.0f, 20.0f, -20.0f);
    glm::vec3 camera_mid_point = glm::vec3(0.0f, 20.0f, -40.0f);
    glm::vec3 camera_end_point = glm::vec3(10.0f, 20.0f, -20.0f);
    float camera_animation_time = 10.0f;
    float camera_current_time = 0.0f;
};