#version 330
// The inputs will be fed by the vertex buffer objects
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 tex_coord;
layout(location = 2) in vec3 normal;

// Matrices as program attributes
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

uniform float target_width = 1200.0f;

// This will be passed to the fragment shader
out vec2 vert_tex_coord;

// Normal to pass to the fragment shader
out vec4 vert_normal;

out vec3 vert_position;

void main() {
    // Copy the input to the fragment shader
    vert_tex_coord = tex_coord;

    // Normal in world coordinates
    vert_normal = normalize(ModelMatrix * vec4(normal, 0.0f));

    vert_position = position;

    // Calculate the final position on screen
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1.0);

}
