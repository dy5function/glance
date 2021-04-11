#version 400 core

uniform vec4 aColor;

out vec4 aFragmentColor;

void main()
{
    aFragmentColor = aColor;
}
