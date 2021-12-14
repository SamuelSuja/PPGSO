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
    glm::vec3 light_positions[3] = {{-10.0f, 10.0f, 0.0f}, {0.0f, 10.0f, 0.0f}, {10.0f, 10.0f, 0.0f}};

    //!Farby svetiel
    glm::vec3 light_colors[3] = {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}};


    int light_mode = 0;
    int post_processing_mode = 0;

    //!Zoznam objektov
    std::list< std::unique_ptr<Object> > objects;

    //!Presuny medzi kamerami
    void prevCamera();
    void nextCamera();

    //
    static bool create_new_bubble();
    void create_new_food();
    static glm::vec3 randomize_vec3(float min, float max, char type);

    void change_lights();
    void animate_light(float delta_time);
    bool animate_lights = false;
    glm::vec3 light_begin_point = glm::vec3(-10.0f, 5.0f, 0.0f);
    glm::vec3 light_end_point = glm::vec3(10.0f, 5.0f, 0.0f);
    float light_animation_time = 3.0f;
    float light_current_time = 0.0f;

    void change_post_processing();

    glm::vec3 find_curve_coords(float t) const;
    void animate_cameras(float delta_time);
    glm::vec3 camera_begin_point = glm::vec3(-10.0f, 20.0f, -20.0f);
    glm::vec3 camera_mid_point = glm::vec3(0.0f, 20.0f, -40.0f);
    glm::vec3 camera_end_point = glm::vec3(10.0f, 20.0f, -20.0f);
    float camera_animation_time = 10.0f;
    float camera_current_time = 0.0f;
};