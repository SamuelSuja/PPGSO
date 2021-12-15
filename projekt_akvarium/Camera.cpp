//Headery z projektu:
#include "Camera.h"

/*!Nastavime premenne potrebne pre view maticu
* @param in_position - Pozicia kamery
* @param in_center - Bod, na ktory sa pozerame
* @param in_up - Vektor oznacujuci rotaciu hornej casti kamery*/
void Camera::setView(glm::vec3 in_position, glm::vec3 in_center, glm::vec3 in_up) {
    position = in_position;
    center = in_center;
    up = in_up;

    viewMatrix = glm::lookAt(position, center, up);
}

/*!Nastavime projekcnu maticu
* @param fov - Vstupne zorne pole
* @param aspect_ratio - Pomer stran okna
* @param near - Near clipping plane
* @param far - Far clipping plane*/
void Camera::setProjection(float fov, float ratio, float near, float far) {
    projectionMatrix = glm::perspective((ppgso::PI)/180.0f * fov, ratio, near, far);
}

//!Updateneme kameru (resp. view maticu)
void Camera::update() {
    viewMatrix = lookAt(position, center, up);
}
