#version 330 core

in vec3 vertex;
in vec3 color;
out vec3 fragColor;

uniform mat4 matTG;

void main()  {
    fragColor = color;
    gl_Position = matTG * vec4 (vertex, 1.0);
}
