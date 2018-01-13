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
uniform int escena;


// Valors per als components que necessitem dels focus de llum
vec3 llumAmbient = vec3(0.2, 0.2, 0.2);


out vec3 fcolor;

vec3 Lambert (vec3 NormSCO, vec3 L)
{
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats

    // Inicialitzem color a component ambient
    vec3 colRes = llumAmbient * matamb;

    // Afegim component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colRes + colFocus * matdiff * dot (L, NormSCO);
    return (colRes);
}

vec3 Phong (vec3 NormSCO, vec3 L, vec4 vertSCO)
{
    // Els vectors estan normalitzats

    // Inicialitzem color a Lambert
    vec3 colRes = Lambert (NormSCO, L);

    // Calculem R i V
    if (dot(NormSCO,L) < 0)
      return colRes;  // no hi ha component especular

    vec3 R = reflect(-L, NormSCO); // equival a: normalize (2.0*dot(NormSCO,L)*NormSCO - L);
    vec3 V = normalize(-vertSCO.xyz);

    if ((dot(R, V) < 0) || (matshin == 0))
      return colRes;  // no hi ha component especular

    // Afegim la component especular
    float shine = pow(max(0.0, dot(R, V)), matshin);
    return (colRes + matspec * colFocus * shine);
}

void main()
{
    vec4 posFocusSCO = vec4(0);
    if(escena == 1) posFocusSCO = view * vec4(posFocus, 1); //posicio focus SCO
    else posFocusSCO = vec4(posFocus, 1); //posicio focus SCO
    vec4 vertexSCO = view * TG * vec4(vertex, 1); //posicio vertex SCO
    vec3 L = normalize(posFocusSCO.xyz - vertexSCO.xyz);

    mat3 NormalMatrix = inverse (transpose (mat3 (view * TG)));
    vec3 normalSCO = normalize(NormalMatrix * normal);

    fcolor =  Phong(normalSCO, L, vertexSCO);
    gl_Position = proj * view * TG * vec4 (vertex, 1.0);

}
