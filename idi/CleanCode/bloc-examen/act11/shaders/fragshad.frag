#version 330 core

in vec3 fcolor;
out vec4 FragColor;

uniform int frangesLoc;

void main()
{
	if (frangesLoc == 0)FragColor = vec4(fcolor,1);
	else {
		if ((int(gl_FragCoord.y /10)%3) == 0) FragColor = vec4(vec3(0,0,0),1);
		else if ((int(gl_FragCoord.y /10)%3) == 1) FragColor = vec4(fcolor,1);
		else FragColor = vec4(vec3(1,1,1),1);
	}
}
