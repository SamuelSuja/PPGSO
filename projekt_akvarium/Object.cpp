#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "Object.h"

//!Generacia modelovej matice
void Object::generateModelMatrix() {
    modelMatrix =
            glm::translate(glm::mat4(1.0f), position)
            * glm::orientate4(rotation)
            * glm::scale(glm::mat4(1.0f), scale);
    /*//Matica identity
    modelMatrix = glm::mat4(1.0f);
    //Skalovanie
    modelMatrix = glm::scale(glm::mat4(1.0f), scale);
    //Rotacia okolo x osi
    modelMatrix = glm::rotate(modelMatrix, rotation.x, {1.0f, 0.0f, 0.0f});
    //Rotacia okolo y osi
    modelMatrix = glm::rotate(modelMatrix, rotation.y, {0.0f, 1.0f, 0.0f});
    //Rotacia okolo z osi
    modelMatrix = glm::rotate(modelMatrix, rotation.z, {0.0f, 0.0f, 1.0f});
    //Translacia
    modelMatrix = glm::translate(modelMatrix, position);*/
}
