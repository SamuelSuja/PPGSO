#pragma once

//GLM kniznice:
#include <glm/glm.hpp>

//Deklarujeme si triedu sceny, aby sme ju mohli pouzivat:
class Scene;

//!Trieda pre vsetky objekty
class Object {
public:
    //!Defaultne konstruktory a destruktor
    Object() = default;
    Object(const Object&) = default;
    Object(Object&&) = default;
    virtual ~Object() {};

    /*!Update objektu v danej scene
    * @param scene - Scena, v ktorej sa tento objekt nachadza
    * @param delta_time - Delta cas (rozdiel casu)
    * @return Vrati false ak ma objekt byt odstraneny, inak true*/
    virtual bool update(Scene &scene, float delta_time) = 0;

    /*!Renderovanie objektu v danej scene
    * @param scene - Scena, v ktorej sa tento objekt nachadza*/
    virtual void render(Scene &scene) = 0;

    //!Vlastnosti objektu
    glm::vec3 position = {0, 0, 0};
    glm::vec3 rotation = {0, 0, 0};
    glm::vec3 scale = {1, 1, 1};
    glm::mat4 model_matrix = glm::mat4(1.0f);

protected:
    //!Generacia modelovej matice
    void generate_model_matrix();
};

