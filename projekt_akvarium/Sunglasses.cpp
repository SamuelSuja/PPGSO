//Headery z projektu:
#include "Sunglasses.h"
#include "Scene.h"

//Shadery:
#include <shaders/project_vert_glsl.h>
#include <shaders/project_frag_glsl.h>

//Objekt okuliarov:
std::unique_ptr<ppgso::Mesh> Sunglasses::mesh;
std::unique_ptr<ppgso::Texture> Sunglasses::texture;
std::unique_ptr<ppgso::Shader> Sunglasses::shader;

//!Konstruktor
Sunglasses::Sunglasses()
{
    //Skalovanie
    scale *= 4.0f;

    //Pridame texturu, mesh a shader
    if (!shader) shader = std::make_unique<ppgso::Shader>(project_vert_glsl, project_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Sunglasses.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Sunglasses.obj");
}

/*!Updateneme poziciu okuliarov
* @param scene Scena, ktoru updatujeme
* @param delta_time Delta cas
* @return false pre zmazanie objektu*/
bool Sunglasses::update(Scene &scene, float delta_time)
{
    //Ulozime maticu rodica
    glm::mat4 parent_model_matrix = model_matrix;

    //Vytvorime model maticu
    generate_model_matrix();

    //Aplikujeme transformacie rodica na transformacie tohto objektu
    model_matrix = parent_model_matrix * model_matrix;

    return true;
}

/*!Renderovanie objektu
* @param scene Scena, v ktorej renderujeme*/
void Sunglasses::render(Scene &scene)
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