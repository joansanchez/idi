#version 330 core

in vec4 gl_FragCoord;
in vec3 fragColor;
out vec4 FragColor;

void main() {
    FragColor = vec4((fragColor*vec3(1.0,1.0,1.0)),1.0);

}
