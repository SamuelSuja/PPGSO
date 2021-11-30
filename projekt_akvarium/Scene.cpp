#include "Scene.h"

//!Update sceny
void Scene::update(float time) {
    cameras[currentCameraIndex]->update();

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
}

//!Renderovanie sceny
void Scene::render() {
    //Prejdeme zoznamom a narenderujeme kazdy objekt
    for ( auto& obj : objects )
        obj->render(*this);
}

//!Posunieme na predchadzajucu kameru
void Scene::prevCamera()
{
    //Zvacsime aktualny index
    currentCameraIndex++;
    //Ak je index mensi ako 0, posunieme sa na koniec zoznamu
    if (currentCameraIndex < 0)
        currentCameraIndex = cameras.size() - 1;
}

//!Prepneme na nasledujucu kameru
void Scene::nextCamera()
{
    //Zmensime aktualny index
    currentCameraIndex--;
    //Ak index presahuje velkost vektoru, vratime sa na zaciatok zoznamu
    if (currentCameraIndex >= cameras.size())
        currentCameraIndex = 0;
}
