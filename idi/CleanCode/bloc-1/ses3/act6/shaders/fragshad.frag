#version 330 core

out vec4 FragColor;
in vec3 colorToPint;

void main() {
    FragColor = vec4(colorToPint, 1);
}
