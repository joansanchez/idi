#version 330 core

in vec3 vertex;

void main()  {
    vec4 TG = vec4(0.5, 0.5, 0.5, 1);
    gl_Position = TG * vec4 (vertex, 1.0);
}
