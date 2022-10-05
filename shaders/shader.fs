#version 330 core
out vec4 FragColor;
in vec2 TexCoor;

uniform vec4 color;
uniform sampler2D t;
uniform float mode;

void main()
{
    FragColor = mode * texture(t, TexCoor) + (1 - mode) * color;
} 