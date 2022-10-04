#version 330
precision highp float;
out vec4 FragColor;
in vec4 InterpolatedColor;
void main(){
	FragColor = InterpolatedColor;
}

