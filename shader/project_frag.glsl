#version 330
// A texture is expected as program attribute
uniform sampler2D Texture;

// Direction of first light
uniform vec3 FirstLightPosition;
uniform vec3 FirstLightColor;

// Direction of second light
uniform vec3 SecondLightPosition;
uniform vec3 SecondLightColor;

// Direction of third light
uniform vec3 ThirdLightPosition;
uniform vec3 ThirdLightColor;

uniform int Mode = 0;

// Pozicia kamery
uniform vec3 CameraPosition;

// (optional) Transparency
uniform float Transparency = 1.0f;

// (optional) Texture offset
uniform vec2 TextureOffset;

// The vertex shader will feed this input
in vec2 vert_tex_coord;

// Wordspace normal passed from vertex shader
in vec4 vert_normal;

//
in vec3 vert_position;

// The final color
out vec4 fragment_color;

in vec2 blur_texture_coords[9];

uniform float width = 1200.0f;
uniform float height = 800.0f;

vec4 calculate_light_color(vec3 light_position, vec3 light_color)
{
    //Utlm svetla
    vec3 light_vector = light_position - vert_position;
    float distance = length(light_vector);
    float a = 0.30f;
    float b = 0.12f;
    float attenuation = 1.0f / (a * pow(distance, 2) + b * distance);

    //Difuzna zlozka
    vec3 light_direction = normalize(light_vector);
    float diffuse = max(dot(vert_normal, vec4(normalize(light_direction), 1.0f)), 0.0f);

    //Ambientna zlozka
    float ambient = 0.2f;

    //Spekularna zlozka
    vec4 view_direction = vec4(normalize(CameraPosition - vert_position), 1.0f);
    vec4 reflection_direction = reflect(vec4(-light_direction, 1.0f), vert_normal);
    float specular_amount = pow(max(dot(view_direction, reflection_direction), 0.0f), 6);
    float specular_light = 0.5f;
    float specular = specular_amount * specular_light;

    //Vypocet farby pomoucou svetelnej zlozky a textury
    //!! Textury invertnute pre komatibilitu s .obj subormi
    vec4 result_color = (texture(Texture, vec2(vert_tex_coord.x, 1.0f - vert_tex_coord.y) + TextureOffset)
    * (diffuse * attenuation + ambient) + texture(Texture, vec2(vert_tex_coord.x, 1.0f - vert_tex_coord.y)
    + TextureOffset) * specular * attenuation) * vec4(light_color, 1.0f);
    return result_color;
}

void main() {
    vec4 first_light_color = calculate_light_color(FirstLightPosition, FirstLightColor);
    vec4 second_light_color = calculate_light_color(SecondLightPosition, SecondLightColor);
    vec4 third_light_color = calculate_light_color(ThirdLightPosition, ThirdLightColor);
    vec4 light_color = first_light_color + second_light_color + third_light_color;
    switch(Mode)
    {
        case 0:
        {
            fragment_color = light_color;
            fragment_color.a = Transparency;
            break;
        }
        case 1:
        {
            float brightness = 0.299f * light_color.r + 0.587f * light_color.g + 0.114f * light_color.b;
            fragment_color = vec4(brightness, brightness, brightness, Transparency);
            break;
        }
        //Sobel edge detection
        case 2:
        {
            vec4 n[9];
            float w = 1.0 / width;
            float h = 1.0 / height;
            n[0] = texture(Texture, vert_tex_coord + vec2( -w, -h));
            n[1] = texture(Texture, vert_tex_coord + vec2(0.0, -h));
            n[2] = texture(Texture, vert_tex_coord + vec2(  w, -h));
            n[3] = texture(Texture, vert_tex_coord + vec2( -w, 0.0));
            n[4] = texture(Texture, vert_tex_coord);
            n[5] = texture(Texture, vert_tex_coord + vec2(  w, 0.0));
            n[6] = texture(Texture, vert_tex_coord + vec2( -w, h));
            n[7] = texture(Texture, vert_tex_coord + vec2(0.0, h));
            n[8] = texture(Texture, vert_tex_coord + vec2(  w, h));

            vec4 sobel_edge_h = n[2] + (2.0*n[5]) + n[8] - (n[0] + (2.0*n[3]) + n[6]);
            vec4 sobel_edge_v = n[0] + (2.0*n[1]) + n[2] - (n[6] + (2.0*n[7]) + n[8]);
            vec4 sobel = sqrt((sobel_edge_h * sobel_edge_h) + (sobel_edge_v * sobel_edge_v));

            fragment_color = vec4(sobel.rgb, Transparency);

            break;
        }
        //Bloom
        case 3:
        {
            // check whether fragment output is higher than threshold, if so output as brightness color
            float brightness = dot(light_color.rgb, vec3(0.2126, 0.7152, 0.0722));
            if (brightness > 1.0)
            fragment_color = vec4(light_color.rgb, Transparency);
            else
            fragment_color = vec4(light_color.rgb * 0.10f, Transparency);
        }
    }
}
