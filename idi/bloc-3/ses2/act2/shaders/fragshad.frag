#version 330 core

in vec3 matambfr;
in vec3 matdifffr;
in vec3 matspecfr;
in float matshinfr;
in mat4 viewmatrix;
in vec4 vertSCO;
in vec3 normalSCO;

out vec4 FragColor;

uniform vec3 posFocus;
uniform vec3 colFocus;   // en SCA

// Valors per als components que necessitem dels focus de llum
vec3 llumAmbient = vec3(0.2, 0.2, 0.2);


vec3 Lambert (vec3 NormSCO, vec3 L)
{
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats

    // Inicialitzem color a component ambient
    vec3 colRes = llumAmbient * matambfr;

    // Afegim component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colRes + colFocus * matdifffr * dot (L, NormSCO);
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

    if ((dot(R, V) < 0) || (matshinfr == 0))
      return colRes;  // no hi ha component especular

    // Afegim la component especular
    float shine = pow(max(0.0, dot(R, V)), matshinfr);
    return (colRes + matspecfr * colFocus * shine);
}

void main()
{

  vec4 posFocusSCO = vec4(posFocus, 1.0); //posicio focus SCO
	vec3 L = normalize(posFocusSCO.xyz - vertSCO.xyz);

	vec3 normalSCO_aux = normalize(normalSCO);

	vec3 fcolor =  Phong(normalSCO_aux, L, vertSCO);
	FragColor = vec4(fcolor,1);

}
