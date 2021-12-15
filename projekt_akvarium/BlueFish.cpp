//Headery z projektu:
#include "BlueFish.h"
#include "Scene.h"
#include "Aquarium.h"
#include "YellowFish.h"

//Shadery:
#include <shaders/project_vert_glsl.h>
#include <shaders/project_frag_glsl.h>

//Objekt ryby:
std::unique_ptr<ppgso::Mesh> BlueFish::mesh;
std::unique_ptr<ppgso::Texture> BlueFish::texture;
std::unique_ptr<ppgso::Shader> BlueFish::shader;

//!Konstruktor
BlueFish::BlueFish()
{
    //Skalovanie
    scale *= 0.5f;

    //Pridame texturu, mesh a shader
    if (!shader) shader = std::make_unique<ppgso::Shader>(project_vert_glsl, project_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Fish2.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Fish2.obj");
}

/*!Updateneme poziciu ryby
* @param scene Scena, ktoru updatujeme
* @param delta_time Delta cas
* @return false pre zmazanie objektu*/
bool BlueFish::update(Scene &scene, float delta_time)
{
    //Upravime poziciu
    position += speed * delta_time;

    //Kolizie s objektami
    for ( auto& obj : scene.objects ) {

        //Kolizie ignoruju seba
        if (obj.get() == this)
            continue;

        //Zisti sa, ci je objekt akvarium
        auto aquarium = dynamic_cast<Aquarium*>(obj.get());
        if (!aquarium)
        {

            //Zisti sa, ci je objekt druha ryba
            auto other_fish = dynamic_cast<YellowFish*>(obj.get());
            if (!other_fish)
            {
                continue;
            }

            //Ak je objekt druha ryba
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

                //Ak nastala kolizia, obidve ryby sa otocia od seba a tato ryba sa vrati
                if ((x < other_x + other_width) && (x + width > other_x) && (z < other_z + other_height) && (z + height > other_z))
                {
                    position -= speed * delta_time;
                    speed *= -1.0f;
                    other_fish->speed = speed * -1.0f;
                }
            }
        }

        //Ak je objekt stena akvaria
        else
        {
            //Porovna sa pozicia ryby oproti stenam akvaria
            float min_x = aquarium->position.x - 0.975f * aquarium->scale_amount;
            float max_x = aquarium->position.x + 0.975f * aquarium->scale_amount;
            float min_z = aquarium->position.z - 0.475f * aquarium->scale_amount;
            float max_z = aquarium->position.z + 0.475f * aquarium->scale_amount;

            //Ak je ryba mimo akvaria, otoci sa a vrati spat
            if ((position.x - 4.0f * scale.x < min_x) || (position.x + 4.0f * scale.x > max_x) || (position.z - 4.0f * scale.z < min_z) || (position.z + 4.0f * scale.z > max_z))
            {
                position -= speed * delta_time;
                speed *= -1.0f;
            }
        }
    }

    //20% sanca na otocenie ryby, 5% sanca na smery -x, +x, -z, +z
    int turn_chance = rand() % 100;
    if (turn_chance < 5)
    {
        speed.x += 4.0f* delta_time;
        if (speed.x > 2.0f)
        {
            speed.x = 2.0f;
        }
    }
    else if (turn_chance < 10)
    {
        speed.x -= 4.0f * delta_time;
        if (speed.x < -2.0f)
        {
            speed.x = -2.0f;
        }
    }
    else if (turn_chance < 15)
    {
        speed.z += 4.0f * delta_time;
        if (speed.z > 2.0f)
        {
            speed.z = 2.0f;
        }
    }
    else if (turn_chance < 20)
    {
        speed.z -= 4.0f * delta_time;
        if (speed.z < -2.0f)
        {
            speed.z = -2.0f;
        }
    }

    //Zistenie rychlost
    glm::vec3 normalized_speed = glm::normalize(speed);
    glm::vec3 z_vector = glm::vec3(0.0f, 0.0f, -1.0f);

    //Zistenie uhla rotacie ryby
    float angle_cos = glm::dot(normalized_speed, z_vector) / glm::length(normalized_speed) * glm::length(z_vector);
    float angle_in_rad = glm::acos(angle_cos);

    //Otocenie ryby v smere pohybu
    if (normalized_speed.x > 0.0f)
    {
        rotation.y = ppgso::PI * 1.0f - angle_in_rad;
    }
    else
    {
        rotation.y = ppgso::PI * 1.0f + angle_in_rad;
    }

    //Vytvorime model maticu
    generate_model_matrix();

    return true;
}

/*!Renderovanie objektu
* @param scene Scena, v ktorej renderujeme*/
void BlueFish::render(Scene &scene)
{
    //Pouzitie shaderu
    shader->use();

    //Post processing
    shader->setUniform("PostProcessingMode", (float) scene.post_processing_mode);

    //Svetlo
    shader->setUniform("FirstLightPosition", scene.light_positions[0]);
    shader->setUniform("FirstLightColor", scene.light_colors[0]);
    shader->setUniform("SecondLightPosition", scene.light_positions[1]);
    shader->setUniform("SecondLightColor", scene.light_colors[1]);
    shader->setUniform("ThirdLightPosition", scene.light_positions[2]);
    shader->setUniform("ThirdLightColor", scene.light_colors[2]);

    //View a projection matice
    shader->setUniform("ProjectionMatrix", scene.cameras[(unsigned int) scene.currentCameraIndex]->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.cameras[(unsigned int) scene.currentCameraIndex]->viewMatrix);

    //Model matica a textura
    shader->setUniform("ModelMatrix", model_matrix);
    shader->setUniform("Texture", *texture);

    //Renderovanie meshu
    mesh->render();
}