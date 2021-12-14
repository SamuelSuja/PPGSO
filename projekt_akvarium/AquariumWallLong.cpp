#include "AquariumWallLong.h"
#include "Scene.h"

#include <shaders/project_vert_glsl.h>
#include <shaders/project_frag_glsl.h>

//Objekt ryby:
std::unique_ptr<ppgso::Mesh> AquariumWallLong::mesh;
std::unique_ptr<ppgso::Texture> AquariumWallLong::texture;
std::unique_ptr<ppgso::Shader> AquariumWallLong::shader;

//!Konstruktor
AquariumWallLong::AquariumWallLong() {
    //Skalovanie

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(project_vert_glsl, project_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("AquariumWall.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("AquariumWallLong.obj");
}

/*!Updateneme poziciu ryby
* @param scene Scena, ktoru updatujeme
* @param delta_time Delta cas
* @return true to delete the object
*/
bool AquariumWallLong::update(Scene &scene, float dt)
{
    /*
    // Hit detection
    for ( auto& obj : scene.objects ) {
        // Ignore self in scene
        if (obj.get() == this)
            continue;

        // We only need to collide with asteroids, ignore other objects
        auto asteroid = dynamic_cast<Asteroid*>(obj.get());
        if (!asteroid) continue;

        if (distance(position, asteroid->position) < asteroid->scale.y) {
            // Explode
            auto explosion = std::make_unique<Explosion>();
            explosion->position = position;
            explosion->scale = scale * 3.0f;
            scene.objects.push_back(move(explosion));

            // Die
            return false;
        }
    }
    */

    generateModelMatrix();
    return true;
}

/*!Renderovanie objektu
* @param scene Scena, v ktorej renderujeme*/
void AquariumWallLong::render(Scene &scene) {
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
    shader->setUniform("ModelMatrix", modelMatrix * glm::translate(glm::mat4(1.0f),local_position/scale));
    shader->setUniform("Texture", *texture);

    shader->setUniform("Transparency", 0.05f);

    mesh->render();
}