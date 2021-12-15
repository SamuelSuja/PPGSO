#include "Aquarium.h"
#include "Scene.h"

//!Konstruktor
Aquarium::Aquarium()
{
    //Skalovanie
    scale_amount = 15.0f;
    scale *= scale_amount;

    //Spodok akvaria
    auto ground = std::make_unique<AquariumGround>();
    ground->rotation = rotation;
    components.push_back(move(ground));

    //4 rohy akvaria
    auto corner_lf = std::make_unique<AquariumCorner>();
    corner_lf->local_position.x -= 0.975f * scale_amount;
    corner_lf->local_position.z -= 0.475f * scale_amount;
    corner_lf->rotation = rotation;
    components.push_back(move(corner_lf));

    auto corner_rf = std::make_unique<AquariumCorner>();
    corner_rf->local_position.x += 0.975f * scale_amount;
    corner_rf->local_position.z -= 0.475f * scale_amount;
    corner_rf->rotation = rotation;
    components.push_back(move(corner_rf));

    auto corner_lb = std::make_unique<AquariumCorner>();
    corner_lb->local_position.x -= 0.975f * scale_amount;
    corner_lb->local_position.z += 0.475f * scale_amount;
    corner_lb->rotation = rotation;
    components.push_back(move(corner_lb));

    auto corner_rb = std::make_unique<AquariumCorner>();
    corner_rb->local_position.x += 0.975f * scale_amount;
    corner_rb->local_position.z += 0.475f * scale_amount;
    corner_rb->rotation = rotation;
    components.push_back(move(corner_rb));

    //4 steny akvaria (2 dlhe, 2 kratke)
    auto wall_back = std::make_unique<AquariumWallLong>();
    wall_back->local_position.z += 0.475f * scale_amount;
    wall_back->rotation = rotation;
    components.push_back(move(wall_back));

    auto wall_left = std::make_unique<AquariumWallShort>();
    wall_left->local_position.x -= 0.975f * scale_amount;
    wall_left->rotation = rotation;
    components.push_back(move(wall_left));

    auto wall_right = std::make_unique<AquariumWallShort>();
    wall_right->local_position.x += 0.975f * scale_amount;
    wall_right->rotation = rotation;
    components.push_back(move(wall_right));

    auto wall_front = std::make_unique<AquariumWallLong>();
    wall_front->local_position.z -= 0.475f * scale_amount;
    wall_front->rotation = rotation;
    components.push_back(move(wall_front));

    //Uprava scale vsetkych komponentov
    for (auto& component : components)
    {
        component->scale *= scale_amount;
    }
}

/*!Updateneme poziciu akvaria
* @param scene Scena, ktoru updatujeme
* @param delta_time Delta cas
* @return false pre zmazanie objektu*/
bool Aquarium::update(Scene &scene, float delta_time)
{
    //Vytvorime model maticu
    generate_model_matrix();

    //Uodate vsetkych komponentov
    for (auto& component : components)
    {
        component->rotation = rotation;
        component->position = position;
        component->scale = scale;
        component->update(scene, delta_time);
    }

    return true;
}

/*!Renderovanie objektu
* @param scene Scena, v ktorej renderujeme*/
void Aquarium::render(Scene &scene)
{
    //Render pre vsetky komponenty
    for (auto& component : components)
    {
        component->render(scene);
    }

}