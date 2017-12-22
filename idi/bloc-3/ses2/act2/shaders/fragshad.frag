#version 330 core


out vec4 FragColor;

in vec3 vertex1;
in vec3 normal1;

in vec3 matamb1;
in vec3 matdiff1;
in vec3 matspec1;
in float matshin1;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;
uniform vec3 posFocus;
uniform vec3 colFocus;

// Valors per als components que necessitem dels focus de llum
vec3 llumAmbient = vec3(0.2, 0.2, 0.2);

in vec3 normalSCO;
in vec4 vertexSCO;

vec3 Lambert (vec3 NormSCO, vec3 L)
{
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats

    // Inicialitzem color a component ambient
    vec3 colRes = llumAmbient * matamb1;

    // Afegim component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colRes + colFocus * matdiff1 * dot (L, NormSCO);
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
	vec4 posFocusSCO = vec4(posFocus, 1); //posicio focus SCO
	vec3 L = normalize(posFocusSCO.xyz - vertexSCO.xyz);
	vec3 fcolor =  Phong(normalize(normalSCO), L, vertexSCO);
	FragColor = vec4(fcolor,1);
}
