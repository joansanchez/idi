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

out vec3 matambfr;
out vec3 matdifffr;
out vec3 matspecfr;
out float matshinfr;
out mat4 viewmatrix;
out vec4 vertSCO;
out vec3 normalSCO;



void main()
{
    vertSCO = view * TG * vec4(vertex,1.0);
    normalSCO = normalize(inverse(transpose(mat3(view * TG)))*normal);

    gl_Position = proj * view * TG * vec4 (vertex, 1.0);
    matambfr = matamb;
    matdifffr = matdiff;
    matspecfr = matspec;
    matshinfr = matshin;
    viewmatrix = view;
}
