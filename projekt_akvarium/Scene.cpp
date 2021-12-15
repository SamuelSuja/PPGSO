#include "Scene.h"
#include "Bubble.h"
#include "Food.h"

//!Update sceny
void Scene::update(float time) {
    cameras[(unsigned int) currentCameraIndex]->update();

    //Urcime zaciatok zoznamu cez iteratory
    auto object_iterator = std::begin(objects);

    //Kym neprejdeme cely zoznam
    while (object_iterator != std::end(objects)) {
        //Pre kazdy objekt
        auto current_object = object_iterator->get();
        //Ak objekt po update vrati false, bude odstraneny
        if (!current_object->update(*this, time))
            //Odstranime objekt zo zoznamu
            object_iterator = objects.erase(object_iterator);
        //Inak sa posunieme na dalsi objekt
        else
            object_iterator++;
    }

    //Ak sa ma vytvorit bublina, vytvormie ju na randomnom mieste vyhradenej oblasti
    if (create_new_bubble()) {
        auto pos = randomize_vec3(-2, 2, 'P');
        auto speed = randomize_vec3(0, 0.1f, 'S');
        auto scale = randomize_vec3(.1f, 0.35f, 'C');
        auto bubble = std::make_unique<Bubble>();
        bubble->position = pos;
        bubble->position.x += 4;
        bubble->position.z += -2;
        bubble->speed = speed;
        bubble->scale = scale;

        //Vlozime bublinu na predposledne miesto (pred akvarium), aby sa renderovala spravne
        auto iterator = objects.end();
        iterator--;
        objects.insert(iterator, move(bubble));
    }

    //Ak sa ma animovat svetlo, bude animovane
    if (animate_lights)
    {
        animate_light(time);
    }

    //Kamery s animaciami su animovane stale
    animate_cameras(time);
}

//!Pridanie castice bubliny
bool Scene::create_new_bubble() {
    bool chance = (rand() % 10) < 1;
    return chance;
}

//!Vytvorenie nahodneho vektoru
glm::vec3 Scene::randomize_vec3(float min, float max, char type) {

    glm::vec3 values;
    float x, y, z;
    float y_min = -1.0f;
    float y_max = 1.0f;
    float z_min = -3.0f;
    float z_max = 4.0f;

    switch (type) {
        case 'P':
            x = min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max - min)));
            z = z_min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(z_max - z_min)));
            y = y_min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(y_max - y_min)));
            values = glm::vec3 (x, y, z);
            break;
        case 'S':
            x = min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max - min)));
            y = min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max - min)));
            z = min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max - min)));
            values = glm::vec3 (x, y, z);
            break;
        case 'C':
            x = min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max - min)));
            y = min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max - min)));
            values = glm::vec3 (x, y, 0.1f);
            break;
        default:
            break;
    }

    return values;
}

//!Renderovanie sceny
void Scene::render() {
    //Prejdeme zoznamom a narenderujeme kazdy objekt
    for ( auto& obj : objects )
        obj->render(*this);
}

//!Pridanie castic jedla
void Scene::create_new_food()
{
    //Nahodny pocet jedla od 10 po 30
    int amount = 10 + rand() % 20;

    //Vsetky kusy jedla pridame do zoznamu objektov na predposledne miesto, ako s bublinami
    for (int i = 0; i < amount; i++) {
        auto pos = randomize_vec3(-10, 10, 'P');
        auto speed = randomize_vec3(0, 0.1f, 'S');
        auto scale = randomize_vec3(.1f, 0.35f, 'C');
        auto food = std::make_unique<Food>();
        pos.y += 20.0f;
        food->position = pos;
        food->speed = speed;
        food->scale = scale;

        //Vlozime jedlo na predposledne miesto (pred akvarium), aby sa renderovalo spravne
        auto iterator = objects.end();
        iterator--;
        objects.insert(iterator, move(food));
    }
}

//!Posunieme na predchadzajucu kameru
void Scene::prev_camera()
{
    //Zvacsime aktualny index
    currentCameraIndex--;
    //Ak je index mensi ako 0, posunieme sa na koniec zoznamu
    if (currentCameraIndex < 0)
        currentCameraIndex = (int)cameras.size() - 1;
}

//!Prepneme na nasledujucu kameru
void Scene::next_camera()
{
    //Zmensime aktualny index
    currentCameraIndex++;
    //Ak index presahuje velkost vektoru, vratime sa na zaciatok zoznamu
    if (currentCameraIndex >= (int)cameras.size())
        currentCameraIndex = 0;
}

//!Zmena svetiel
void Scene::change_lights()
{
    //Vyber, ktore svetla budu pouzite
    switch (light_mode)
    {
        //3 biele svetla
        case 0:
        {
            light_mode++;

            //Pozicie svetiel
            light_positions[0] = glm::vec3(-10.0f, 10.0f, 0.0f);
            light_positions[1] = glm::vec3(0.0f, 10.0f, 0.0f);
            light_positions[2] = glm::vec3(10.0f, 10.0f, 0.0f);

            //Farby svetiel
            light_colors[0] = glm::vec3(0.4f, 0.1f, 0.1f);
            light_colors[1] = glm::vec3(0.1f, 0.4f, 0.1f);
            light_colors[2] = glm::vec3(0.1f, 0.1f, 0.4f);
            break;
        }

        //1 cervene, 1 zelene, 1 modre svetlo
        case 1:
        {
            light_mode++;

            //Pozicie svetiel
            light_positions[0] = glm::vec3(-10.0f, 10.0f, 0.0f);
            light_positions[1] = glm::vec3(0.0f, 5.0f, 0.0f);
            light_positions[2] = glm::vec3(10.0f, 10.0f, 0.0f);

            //Farby svetiel
            light_colors[0] = glm::vec3(0.0f, 0.0f, 0.0f);
            light_colors[1] = glm::vec3(1.0f, 1.0f, 1.0f);
            light_colors[2] = glm::vec3(0.0f, 0.0f, 0.0f);
            break;
        }

        //1 biele svetlo v strede
        case 2:
        {
            light_mode++;

            //Pozicie svetiel
            light_positions[0] = glm::vec3(-10.0f, 10.0f, 0.0f);
            light_positions[1] = glm::vec3(0.0f, 5.0f, 0.0f);
            light_positions[2] = glm::vec3(10.0f, 10.0f, 0.0f);

            //Farby svetiel
            light_colors[0] = glm::vec3(0.0f, 0.0f, 0.0f);
            light_colors[1] = glm::vec3(1.0f, 1.0f, 1.0f);
            light_colors[2] = glm::vec3(0.0f, 0.0f, 0.0f);

            animate_lights = true;
            break;
        }

        //1 biele svetlo, ktore sa pohybuje
        case 3:
        {
            light_mode=0;

            //Pozicie svetiel
            light_positions[0] = glm::vec3(-10.0f, 20.0f, 0.0f);
            light_positions[1] = glm::vec3(0.0f, 20.0f, 0.0f);
            light_positions[2] = glm::vec3(10.0f, 20.0f, 0.0f);

            //Farby svetiel
            light_colors[0] = glm::vec3(0.3f, 0.3f, 0.3f);
            light_colors[1] = glm::vec3(0.3f, 0.3f, 0.3f);
            light_colors[2] = glm::vec3(0.3f, 0.3f, 0.3f);

            animate_lights = false;
            break;
        }
    }
}

//!Animacia pohybu svetla
void Scene::animate_light(float delta_time)
{
    //Ak uz svetlo preslo aktualny keyframe
    if (light_current_time >= light_animation_time)
    {
        //Vymeni sa koncovy a zaciatocny keyframe a resetne sa counter
        glm::vec3 temp = light_begin_point;
        light_begin_point = light_end_point;
        light_end_point = temp;
        light_current_time = 0.0f;
    }

    //Cas sa zvysi o delta_time
    light_current_time += delta_time;

    //Svetlo sa posunie blizsie k druhemu keyframe po usecke
    light_positions[1] = light_begin_point + (light_current_time/light_animation_time) * (light_end_point - light_begin_point);
}

//!Zmena post processing efektu
void Scene::change_post_processing()
{
    if (post_processing_mode == 3)
    {
        post_processing_mode = 0;
    }
    else
    {
        post_processing_mode++;
    }
}

//!Zistenie pozicie na bezierovej krivke
glm::vec3 Scene::find_curve_coords(float t) const
{
    //Interpolacia medzi 4 bodmi cez de Casteljau algoritmus
    glm::vec3 begin_mid_point = camera_begin_point + t * (camera_mid_point - camera_begin_point);
    glm::vec3 mid_end_point = camera_mid_point + t * (camera_end_point - camera_mid_point);

    //Interpolacia medzi finalnymi 2 bodmi
    glm::vec3 final_position = begin_mid_point + t * (mid_end_point - begin_mid_point);

    //Vrati sa pozicia finalneho bodu
    return final_position;
}

//!Animacia pohybu kamery
void Scene::animate_cameras(float delta_time)
{
    //Ak uz kamera presla aktualny keyframe
    if (camera_current_time >= camera_animation_time)
    {
        //Vymeni sa koncovy a zaciatocny keyframe a resetne sa counter
        glm::vec3 temp = camera_begin_point;
        camera_begin_point = camera_end_point;
        camera_end_point = temp;
        camera_current_time = 0.0f;
    }

    //Cas sa zvysi o delta_time
    camera_current_time += delta_time;

    //Kamera sa posunie blizsie k druhemu keyframe po bezierovej krivke
    cameras[0]->position = find_curve_coords(camera_current_time/camera_animation_time);
}