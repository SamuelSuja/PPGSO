#version 330
//Textura
uniform sampler2D Texture;

//Pozicia a farba prveho svetla
uniform vec3 FirstLightPosition;
uniform vec3 FirstLightColor;

//Pozicia a farba druheho svetla
uniform vec3 SecondLightPosition;
uniform vec3 SecondLightColor;

//Pozicia a farba tretieho svetla
uniform vec3 ThirdLightPosition;
uniform vec3 ThirdLightColor;

//Vlastnosti materialu
uniform vec3 MaterialAmbient = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 MaterialDiffuse = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 MaterialSpecular = vec3(1.0f, 1.0f, 1.0f);

//Aktualny post processing filter
uniform float PostProcessingMode = 0;

//Pozicia kamery
uniform vec3 CameraPosition;

//Priehladnost
uniform float Transparency = 1.0f;

//Vstupne koordinaty textur z vertex shaderu
in vec2 vert_tex_coord;

//Vstupne normaly z vertex shaderu
in vec3 vert_normal;

//Vstupne koordinaty z vertex shaderu
in vec3 vert_position;

//Vysledna farba
out vec4 fragment_color;

//Velkost obrazovky
uniform float width = 1200.0f;
uniform float height = 800.0f;

//Funkcia na vypocet svetla
vec4 calculate_light_color(vec3 light_position, vec3 light_color)
{
    //Utlm svetla
    vec3 light_direction = normalize(light_position - vert_position);
    float distance = length(light_direction);
    float a = 0.30f;
    float b = 0.12f;
    float attenuation = 1.0f / (a * pow(distance, 2) + b * distance);

    //Difuzna zlozka
    float diffuse = max(dot(vert_normal, light_direction), 0.0f);

    //Ambientna zlozka
    float ambient = 0.2f;

    //Spekularna zlozka
    vec3 view_direction = normalize(CameraPosition - vert_position);
    vec3 reflection_direction = reflect(-light_direction, vert_normal);
    float specular_amount = pow(max(dot(view_direction, reflection_direction), 0.0f), 6);
    float specular_light = 0.5f;
    float specular = specular_amount * specular_light;

    //Skombinovanie materialnych a svetelnych zloziek
    vec4 combined_ambient = vec4(MaterialAmbient * ambient, 1.0f);
    vec4 combined_diffuse = vec4(MaterialDiffuse * diffuse, 1.0f);
    vec4 combined_specular = vec4(MaterialSpecular * specular, 1.0f);

    //Vypocet farby pomoucou svetelnej zlozky a textury
    //!! Textury invertnute pre komatibilitu s .obj subormi
    vec4 result_color = (texture(Texture, vec2(vert_tex_coord.x, 1.0f - vert_tex_coord.y))
    * (combined_diffuse * attenuation + combined_ambient)
    + texture(Texture, vec2(vert_tex_coord.x, 1.0f - vert_tex_coord.y))
    * combined_specular * attenuation) * vec4(light_color, 1.0f);
    return result_color;
}

//Main funkcia
void main() {

    //Vypocitame farby svetiel
    vec4 first_light_color = calculate_light_color(FirstLightPosition, FirstLightColor);
    vec4 second_light_color = calculate_light_color(SecondLightPosition, SecondLightColor);
    vec4 third_light_color = calculate_light_color(ThirdLightPosition, ThirdLightColor);
    vec4 light_color = first_light_color + second_light_color + third_light_color;

    //Nastavime spravny post processing efekt
    switch(int(PostProcessingMode))
    {
        //Ziadny filter
        case 0:
        {
            fragment_color = light_color;
            fragment_color.a = Transparency;
            break;
        }

        //Grayscale
        case 1:
        {
            float brightness = 0.299f * light_color.r + 0.587f * light_color.g + 0.114f * light_color.b;
            fragment_color = vec4(brightness, brightness, brightness, Transparency);
            break;
        }

        //Sobel edge detection
        case 2:
        {
            //Pole okolitych pixelov
            vec4 n[9];
            float w = 1.0 / width;
            float h = 1.0 / height;

            //Textury z okolitych pixelov
            n[0] = texture(Texture, vert_tex_coord + vec2( -w, -h));
            n[1] = texture(Texture, vert_tex_coord + vec2(0.0, -h));
            n[2] = texture(Texture, vert_tex_coord + vec2(  w, -h));
            n[3] = texture(Texture, vert_tex_coord + vec2( -w, 0.0));
            n[4] = texture(Texture, vert_tex_coord);
            n[5] = texture(Texture, vert_tex_coord + vec2(  w, 0.0));
            n[6] = texture(Texture, vert_tex_coord + vec2( -w, h));
            n[7] = texture(Texture, vert_tex_coord + vec2(0.0, h));
            n[8] = texture(Texture, vert_tex_coord + vec2(  w, h));

            //Vypocet
            vec4 sobel_edge_h = n[2] + (2.0*n[5]) + n[8] - (n[0] + (2.0*n[3]) + n[6]);
            vec4 sobel_edge_v = n[0] + (2.0*n[1]) + n[2] - (n[6] + (2.0*n[7]) + n[8]);
            vec4 sobel = sqrt((sobel_edge_h * sobel_edge_h) + (sobel_edge_v * sobel_edge_v));

            //Vysledna farba
            fragment_color = vec4(sobel.rgb, Transparency);

            break;
        }

        //Bloom
        case 3:
        {
            //Zistime jasnost pixelu, ak je pixel svetly, vykreslime ho, inak bude tmavsi
            float brightness = dot(light_color.rgb, vec3(0.2126, 0.7152, 0.0722));
            if (brightness > 1.0)
                fragment_color = vec4(light_color.rgb, Transparency);
            else
                fragment_color = vec4(light_color.rgb * 0.10f, Transparency);
        }
    }
}
