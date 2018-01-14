#version 330 core

in vec4 gl_FragCoord; //coordenadas del píxel
out vec4 FragColor;

uniform vec2 WindowSize;
void main() {

    if (int(gl_FragCoord.y)%10 < 5)   FragColor = vec4(1.0,0.0,0.0,1.0);
    else discard;
}
