#version 330 core

in vec3 fcolor;

out vec4 FragColor;

uniform bool highlight;

void main() {
    if (highlight)  {
        FragColor = vec4(1,1,1,1);
    }
    else {
        FragColor = vec4(fcolor, 1.0);
    }
}
