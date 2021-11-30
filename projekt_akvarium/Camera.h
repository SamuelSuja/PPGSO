#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <ppgso/ppgso.h>

//!Objekt kamery
class Camera {
public:
    //!Vektory potrebne pre vytvorenie view matice cez glm::lookat:
    glm::vec3 position;
    glm::vec3 center;
    glm::vec3 up;

    //!View a projection matice:
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    //!Default konstruktor
    Camera() = default;

    /*!Nastavime premenne potrebne pre view maticu
    * @param in_position - Pozicia kamery
    * @param in_center - Bod, na ktory sa pozerame
    * @param in_up - Vektor oznacujuci rotaciu hornej casti kamery*/
    void setView(glm::vec3 in_position, glm::vec3 in_center, glm::vec3 in_up);

    /*!Nastavime projekcnu maticu
    * @param fov - Vstupne zorne pole
    * @param aspect_ratio - Pomer stran okna
    * @param near - Near clipping plane
    * @param far - Far clipping plane*/
    void setProjection(float fov, float ratio, float near, float far);

    //!Updateneme kameru (resp. view maticu)
    void update();
};

