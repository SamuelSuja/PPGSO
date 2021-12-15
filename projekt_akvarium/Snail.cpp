//Headery z projektu:
#include "Snail.h"
#include "Scene.h"

//Shadery:
#include <shaders/project_vert_glsl.h>
#include <shaders/project_frag_glsl.h>

//Objekt slimaka:
std::unique_ptr<ppgso::Mesh> Snail::mesh;
std::unique_ptr<ppgso::Texture> Snail::texture;
std::unique_ptr<ppgso::Shader> Snail::shader;

//!Konstruktor
Snail::Snail()
{
    //Skalovanie
    scale *= 0.5f;

    //Pridame texturu, mesh a shader
    if (!shader) shader = std::make_unique<ppgso::Shader>(project_vert_glsl, project_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Snail.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Snail.obj");

    //Klobuk
    hat = std::make_unique<Hat>();
    hat->rotation.x = ppgso::PI * 1.5f;
    hat->position = glm::vec3(-0.2f,2.8f,3.3f);
}

//!Zistenie pozicie na bezierovej krivke
glm::vec3 Snail::find_curve_coords(float t) const
{
    //Interpolacia medzi 4 bodmi cez de Casteljau algoritmus
    glm::vec3 begin_mid_point = begin_point + t * (mid_point - begin_point);
    glm::vec3 mid_end_point = mid_point + t * (end_point - mid_point);

    //Interpolacia medzi finalnymi 2 bodmi
    glm::vec3 final_position = begin_mid_point + t * (mid_end_point - begin_mid_point);

    //Vrati sa pozicia finalneho bodu
    return final_position;
}

/*!Updateneme poziciu slimaka
* @param scene Scena, ktoru updatujeme
* @param delta_time Delta cas
* @return false pre zmazanie objektu*/
bool Snail::update(Scene &scene, float delta_time)
{
    //Ak sa kamera dostala do dalsieho keyframe (presiahla nastaveny cas trvania), keyframes sa vymenia a cas sa resetnes
    if (current_time >= animation_time)
    {
        glm::vec3 temp = begin_point;
        begin_point = end_point;
        end_point = temp;
        current_time = 0.0f;
    }

    //Zvysenie aktualneho casu
    current_time += delta_time;

    //Zmena pozicie na bezierovej krivke
    glm::vec3 prev_position = position;
    position = find_curve_coords(current_time/animation_time);

    //Zistenie rychlosti
    glm::vec3 speed = position - prev_position;
    glm::vec3 normalized_speed = glm::normalize(speed);

    //Zistenie uhla rotacie
    glm::vec3 z_vector = glm::vec3(0.0f, 0.0f, -1.0f);
    float angle_cos = glm::dot(normalized_speed, z_vector) / glm::length(normalized_speed) * glm::length(z_vector);
    float angle_in_rad = glm::acos(angle_cos);

    //Rotacia podla smeru pohybu
    if (normalized_speed.x > 0.0f)
    {
        rotation.z = -angle_in_rad;
    }
    else
    {
        rotation.z = angle_in_rad;
    }

    //Vytvorime model maticu
    generate_model_matrix();

    //Posleme model maticu dietatu a updatneme ho
    hat->model_matrix = model_matrix;
    hat->update(scene, delta_time);

    return true;
}

/*!Renderovanie objektu
* @param scene Scena, v ktorej renderujeme*/
void Snail::render(Scene &scene)
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

    //Renderovanie dietata - klobuku
    hat->render(scene);
}