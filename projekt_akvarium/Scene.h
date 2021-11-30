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
    //!Update objektov
    void update(float time);

    //!Renderovanie objektov
    void render();

    //!Zoznam (vektor) kamier
    std::vector< std::unique_ptr<Camera> > cameras;

    //!Aktualna kamera
    int currentCameraIndex = 0;

    //!Zoznam objektov
    std::list< std::unique_ptr<Object> > objects;

    //!Smer svetla
    glm::vec3 lightDirection{0.0f, 0.0f, -1.0f};

    //!Presuny medzi kamerami
    void prevCamera();
    void nextCamera();
};