#include "BlueFish.h"
#include "Scene.h"
#include "Aquarium.h"
#include "YellowFish.h"

#include <shaders/project_vert_glsl.h>
#include <shaders/project_frag_glsl.h>

//Objekt ryby:
std::unique_ptr<ppgso::Mesh> BlueFish::mesh;
std::unique_ptr<ppgso::Texture> BlueFish::texture;
std::unique_ptr<ppgso::Shader> BlueFish::shader;

//!Konstruktor
BlueFish::BlueFish() {
    //Skalovanie
    scale *= 0.5f;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(project_vert_glsl, project_frag_glsl);
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
    position += speed * dt;
    // Hit detection
    for ( auto& obj : scene.objects ) {
        // Ignore self in scene
        if (obj.get() == this)
            continue;

        // We only need to collide with asteroids, ignore other objects
        auto aquarium = dynamic_cast<Aquarium*>(obj.get());
        if (!aquarium)
        {
            auto other_fish = dynamic_cast<YellowFish*>(obj.get());
            if (!other_fish)
            {
                continue;
            }
            else
            {
                //Kolozie pomocou AABB (Axis Aligned Bounding Box):
                float x = position.x - 3.0f * scale.x;
                float z = position.z - 3.0f * scale.z;
                float width = (position.x + 3.0f * scale.x) - (position.x - 3.0f * scale.x);
                float height = (position.z + 3.0f * scale.z) - (position.z - 3.0f * scale.z);

                float other_x = other_fish->position.x - 3.0f * scale.x;
                float other_z = other_fish->position.z - 3.0f * scale.z;
                float other_width = (other_fish->position.x + 3.0f * scale.x) - (other_fish->position.x - 3.0f * scale.x);
                float other_height = (other_fish->position.z + 3.0f * scale.z) - (other_fish->position.z - 3.0f * scale.z);

                if ((x < other_x + other_width) && (x + width > other_x) && (z < other_z + other_height) && (z + height > other_z))
                {
                    position -= speed * dt;
                    speed *= -1.0f;
                    other_fish->speed *= -1.0f;
                }
            }
        }
        else
        {
            float min_x = aquarium->position.x - 0.975f * aquarium->scale_amount;
            float max_x = aquarium->position.x + 0.975f * aquarium->scale_amount;
            float min_z = aquarium->position.z - 0.475f * aquarium->scale_amount;
            float max_z = aquarium->position.z + 0.475f * aquarium->scale_amount;
            if ((position.x - 4.0f * scale.x < min_x) || (position.x + 4.0f * scale.x > max_x) || (position.z - 4.0f * scale.z < min_z) || (position.z + 4.0f * scale.z > max_z))
            {
                position -= speed * dt;
                speed *= -1.0f;
            }
        }
    }

    int turn_chance = rand() % 100;
    if (turn_chance < 5)
    {
        speed.x += 2.5f* dt;
        if (speed.x > 2.0f)
        {
            speed.x = 2.0f;
        }
    }
    else if (turn_chance < 10)
    {
        speed.x -= 2.5f * dt;
        if (speed.x < -2.0f)
        {
            speed.x = -2.0f;
        }
    }
    else if (turn_chance < 15)
    {
        speed.z += 2.5f * dt;
        if (speed.z > 2.0f)
        {
            speed.z = 2.0f;
        }
    }
    else if (turn_chance < 20)
    {
        speed.z -= 2.5f * dt;
        if (speed.z < -2.0f)
        {
            speed.z = -2.0f;
        }
    }

    glm::vec3 normalized_speed = glm::normalize(speed);
    glm::vec3 z_vector = glm::vec3(0.0f, 0.0f, -1.0f);
    float angle_cos = glm::dot(normalized_speed, z_vector) / glm::length(normalized_speed) * glm::length(z_vector);
    float angle_in_rad = glm::acos(angle_cos);
    if (normalized_speed.x > 0.0f)
    {
        rotation.y = ppgso::PI * 1.0f - angle_in_rad;
    }
    else
    {
        rotation.y = ppgso::PI * 1.0f + angle_in_rad;
    }
    generateModelMatrix();
    return true;
}

/*!Renderovanie objektu
* @param scene Scena, v ktorej renderujeme*/
void BlueFish::render(Scene &scene) {
    shader->use();

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