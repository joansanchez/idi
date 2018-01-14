#version 330 core

in vec3 vertex;
in vec3 color;

out vec3 colorToPint;

void main()  {
    colorToPint = color;
    gl_Position = vec4 (vertex, 1.0);
}
