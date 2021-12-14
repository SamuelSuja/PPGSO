#include "Snail.h"
#include "Scene.h"

#include <shaders/project_vert_glsl.h>
#include <shaders/project_frag_glsl.h>

//Objekt ryby:
std::unique_ptr<ppgso::Mesh> Snail::mesh;
std::unique_ptr<ppgso::Texture> Snail::texture;
std::unique_ptr<ppgso::Shader> Snail::shader;

//!Konstruktor
Snail::Snail() {
    //Skalovanie
    scale *= 0.5f;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(project_vert_glsl, project_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Snail.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Snail.obj");
}

glm::vec3 Snail::find_curve_coords(float t) const
{
    glm::vec3 begin_mid_point = begin_point + t * (mid_point - begin_point);
    glm::vec3 mid_end_point = mid_point + t * (end_point - mid_point);

    glm::vec3 final_position = begin_mid_point + t * (mid_end_point - begin_mid_point);
    return final_position;
}

/*!Updateneme poziciu ryby
* @param scene Scena, ktoru updatujeme
* @param delta_time Delta cas
* @return true to delete the object
*/
bool Snail::update(Scene &scene, float delta_time)
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
    if (current_time >= animation_time)
    {
        glm::vec3 temp = begin_point;
        begin_point = end_point;
        end_point = temp;
        current_time = 0.0f;
    }

    current_time += delta_time;
    glm::vec3 prev_position = position;
    position = find_curve_coords(current_time/animation_time);

    glm::vec3 speed = position - prev_position;
    glm::vec3 normalized_speed = glm::normalize(speed);
    glm::vec3 z_vector = glm::vec3(0.0f, 0.0f, -1.0f);
    float angle_cos = glm::dot(normalized_speed, z_vector) / glm::length(normalized_speed) * glm::length(z_vector);
    float angle_in_rad = glm::acos(angle_cos);
    if (normalized_speed.x > 0.0f)
    {
        rotation.z = -angle_in_rad;
    }
    else
    {
        rotation.z = angle_in_rad;
    }

    generateModelMatrix();
    return true;
}

/*!Renderovanie objektu
* @param scene Scena, v ktorej renderujeme*/
void Snail::render(Scene &scene) {
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