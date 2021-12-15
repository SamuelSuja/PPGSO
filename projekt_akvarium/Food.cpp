//Headery z projektu:
#include "Food.h"
#include "Scene.h"

//Shadery:
#include <shaders/project_vert_glsl.h>
#include <shaders/project_frag_glsl.h>

//Objekt jedla:
std::unique_ptr<ppgso::Mesh> Food::mesh;
std::unique_ptr<ppgso::Texture> Food::texture;
std::unique_ptr<ppgso::Shader> Food::shader;

//!Gravitacne zrychlenie
const float G = 9.81f;

//!Konstruktor
Food::Food()
{
    //Skalovanie
    scale *= 1.5f;

    //Pridame texturu, mesh a shader
    if (!shader) shader = std::make_unique<ppgso::Shader>(project_vert_glsl, project_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Food.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");
}

/*!Updateneme poziciu jedla
* @param scene Scena, ktoru updatujeme
* @param delta_time Delta cas
* @return false pre zmazanie objektu*/
bool Food::update(Scene &scene, float delta_time)
{
    //Ak je bublina na spodku akvaria, vymaze sa
    if (position.y < 4.0f) { return false; }

    //Update pozicie
    position += speed * delta_time * 0.05f;
    speed += glm::vec3{0, -2,0} * delta_time;

    //Update kmitania
    int oscillate_chance = rand() % 100;
    if (position.y < 15.0f) {
        switch (oscillate_chance) {
            case 0:
            {
                oscillate_direction.x = 1.0f;
                break;
            }
            case 1:
            {
                oscillate_direction.x = 0.0f;
                break;
            }
            case 2:
            {
                oscillate_direction.x = -1.0f;
                break;
            }
            case 3:
            {
                oscillate_direction.z = 1.0f;
                break;
            }
            case 4:
            {
                oscillate_direction.z = 0.0f;
                break;
            }
            case 5:
            {
                oscillate_direction.z = -1.0f;
                break;
            }
            default:
            {
                break;
            }
        }
    }

    //Uprava pozicie o smer kmitania
    position += oscillate_direction * delta_time;

    //Vytvorime model maticu
    generate_model_matrix();

    return true;
}

/*!Renderovanie objektu
* @param scene Scena, v ktorej renderujeme*/
void Food::render(Scene &scene) {
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