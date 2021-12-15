#version 330
//Vstupna pozicia, pozicia textury, normala
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 tex_coord;
layout(location = 2) in vec3 normal;

//Transformacne matice
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

//Vystupne koordinaty textury do fragment shaderu
out vec2 vert_tex_coord;

//Vystupne normaly do fragment shaderu
out vec3 vert_normal;

//Vystupne pozicie do fragment shaderu
out vec3 vert_position;

//Funkcia main
void main() {
    //Preposleme koordinaty textury do fragment shaderu
    vert_tex_coord = tex_coord;

    //Preposleme normaly do fragment shaderu
    vert_normal = normalize(normal);

    //Preposleme pozicie do fragment shaderu
    vert_position = vec3(ModelMatrix * vec4(position, 1.0f));

    //Vypocet pozicie na obrazovke
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.0);

}
