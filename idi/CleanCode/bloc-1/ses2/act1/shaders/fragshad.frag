#version 330 core

in vec4 gl_FragCoord; //coordenadas del píxel
out vec4 FragColor;

uniform vec2 WindowSize;
void main() {

    if(gl_FragCoord .x < 355. && gl_FragCoord .y > 355.){ //se supone que 355 es el píxel central
        FragColor = vec4(1.0,0.0,0.0,1.0);
    }
    else if(gl_FragCoord .x >= 355. && gl_FragCoord .y > 355.){ //se supone que 355 es el píxel central
        FragColor = vec4(0.0,0.0,1.0,1.0);
    }
    else if(gl_FragCoord .x < 355. && gl_FragCoord .y < 355.){ //se supone que 355 es el píxel central
        FragColor = vec4(0.0,1.0,0.0,1.0);
    }
    else {
      FragColor = vec4(1.0,1.0,0.0,1.0);
    }
}
