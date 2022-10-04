#version 330
uniform float time;
uniform float mox;
uniform float moy;
out vec4 InterpolatedColor;
mat3 mvp1 = mat3(vec3(1.0, 0.0, 0.0), vec3(0.0, cos(time), sin(time)), vec3(0.0, -sin(time), cos(time)));
mat3 mvp2 = mat3(vec3(0.0, 1.0, 0.0), vec3(cos(time), 0.0, -sin(time)), vec3(sin(time), 0.0, cos(time)));
mat3 mvp3 = mat3(vec3(cos(time), sin(time), 0.0), vec3(-sin(time), cos(time), 0.0), vec3(0.0, 0.0, 1.0));
mat3 mvp = mvp1 * mvp2 * mvp3;
void main(){
	vec3 origin = vec3(0.0,0.0,0.0);
	float pi = 3.15159;
	float div = 32.0;//16.0;
	float r = 1.0;
	float theta = (mod(gl_VertexID,div))*((pi)/div);;
	float fi = floor(gl_VertexID/(div))*(2*pi)/div;
	float x = r * sin(theta+mox) * cos(fi+moy);
	float y = r * sin(theta+moy) * sin(fi+mox);
	float z = r * cos(theta);
	vec3 pos = vec3(origin.x + x, origin.y + y, origin.z + z) * mvp;
	gl_Position = vec4(pos,1.0);
	InterpolatedColor = vec4(sin(gl_VertexID+time),cos(gl_VertexID+time),cos(gl_VertexID),1.0);
	// InterpolatedColor = vec4(sin(gl_VertexID),cos(gl_VertexID),cos(gl_VertexID),1.0);
	gl_PointSize = 10.0;
}

