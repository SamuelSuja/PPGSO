#include "BlueFish.h"
#include "Scene.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

//Objekt ryby:
std::unique_ptr<ppgso::Mesh> BlueFish::mesh;
std::unique_ptr<ppgso::Texture> BlueFish::texture;
std::unique_ptr<ppgso::Shader> BlueFish::shader;

//!Konstruktor
BlueFish::BlueFish() {
    //Skalovanie
    scale *= 1.5f;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Fish2.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Fish2.obj");
}

/*!Updateneme poziciu ryby
* @param scene Scena, ktoru updatujeme
* @param delta_time Delta cas
* @return true to delete the object
*/
bool BlueFish::update(Scene &scene, float dt)
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
void BlueFish::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.cameras[scene.currentCameraIndex]->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.cameras[scene.currentCameraIndex]->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();
}