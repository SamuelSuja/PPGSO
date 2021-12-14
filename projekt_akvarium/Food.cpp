#include "Food.h"
#include "Scene.h"
#include "BlueFish.h"
#include "YellowFish.h"

#include <shaders/project_vert_glsl.h>
#include <shaders/project_frag_glsl.h>

//Objekt ryby:
std::unique_ptr<ppgso::Mesh> Food::mesh;
std::unique_ptr<ppgso::Texture> Food::texture;
std::unique_ptr<ppgso::Shader> Food::shader;

const float G = 9.81f;

//!Konstruktor
Food::Food() {
    //Skalovanie
    scale *= 2.0f;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(project_vert_glsl, project_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Food.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");
}

/*!Updateneme poziciu ryby
* @param scene Scena, ktoru updatujeme
* @param delta_time Delta cas
* @return true to delete the object
*/
bool Food::update(Scene &scene, float delta_time)
{
    if (position.y < 2.0f) { return false; }

    position += speed * delta_time * 0.05f;
    speed += glm::vec3{0, -5,0} * delta_time;

    generateModelMatrix();
    return true;
}

/*!Renderovanie objektu
* @param scene Scena, v ktorej renderujeme*/
void Food::render(Scene &scene) {
    shader->use();

    // Set up post processing
    shader->setUniform("PostProcessingMode", scene.post_processing_mode);

    // Set up light
    shader->setUniform("FirstLightPosition", scene.light_positions[0]);
    shader->setUniform("FirstLightColor", scene.light_colors[0]);
    shader->setUniform("SecondLightPosition", scene.light_positions[1]);
    shader->setUniform("SecondLightColor", scene.light_colors[1]);
    shader->setUniform("ThirdLightPosition", scene.light_positions[2]);
    shader->setUniform("ThirdLightColor", scene.light_colors[2]);


    // use camera
    shader->setUniform("ProjectionMatrix", scene.cameras[scene.currentCameraIndex]->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.cameras[scene.currentCameraIndex]->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}