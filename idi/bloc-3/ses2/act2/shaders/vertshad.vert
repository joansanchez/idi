#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;
uniform vec3 posFocus;
uniform vec3 colFocus;   // en SCA


// Valors per als components que necessitem dels focus de llum
vec3 llumAmbient = vec3(0.2, 0.2, 0.2);


out vec3 vertex;
out vec3 normal;

out vec3 matamb;
out vec3 matdiff;
out vec3 matspec;
out float matshin;

out vec3 llumAmbient;

out vec3 L;
out vec3 normalSCO;
out vec4 vertexSCO;

void main()
{
  //vec4 posFocusSCO = view * vec4(posFocus, 1); //posicio focus SCO

    vec4 posFocusSCO = vec4(posFocus, 1); //posicio focus SCO
    vec4 vertexSCO = view * TG * vec4(vertex, 1); //posicio vertex SCO
    vec3 L = normalize(posFocusSCO.xyz - vertexSCO.xyz);

    mat3 NormalMatrix = inverse (transpose (mat3 (view * TG)));
    vec3 normalSCO = normalize(NormalMatrix * normal);

    gl_Position = proj * view * TG * vec4 (vertex, 1.0);

}
