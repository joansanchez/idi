#version 330 core

in vec3 vertex;

uniform mat4 matTG;

void main()  {

    gl_Position = matTG * vec4 (vertex, 1.0);
}
