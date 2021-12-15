//GLM kniznice:
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

//Headery z projektu:
#include "Object.h"

//!Generacia modelovej matice
void Object::generate_model_matrix() {
    model_matrix =
            glm::translate(glm::mat4(1.0f), position)
            * glm::orientate4(rotation)
            * glm::scale(glm::mat4(1.0f), scale);
}
