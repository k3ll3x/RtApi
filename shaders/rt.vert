#version 330
uniform float time;
uniform float mox;
uniform float moy;
uniform float psz;
out vec4 InterpolatedColor;

mat3 rotationX(float angle){
    float c = cos(angle);
    float s = sin(angle);
    return mat3(
        1.0, 0.0, 0.0,
        0.0, c,   -s,
        0.0, s,   c
    );
}

mat3 rotationY(float angle){
    float c = cos(angle);
    float s = sin(angle);
    return mat3(
        c,   0.0, s,
        0.0, 1.0, 0.0,
        -s,  0.0, c
    );
}

mat3 rotationZ(float angle){
    float c = cos(angle);
    float s = sin(angle);
    return mat3(
        c,   -s,  0.0,
        s,   c,   0.0,
        0.0, 0.0, 1.0
    );
}

int id = gl_VertexID;
float pi = 3.14159;

void main(){
	vec3 origin = vec3(0.0,0.0,0.0);

	float i = mod(id, 9*pi);
	// float i = mod(id, psz*pi);
	// float i = mod(id, time*pi);
	float r = sin(i)*0.9;
	float theta = i;
	float fi = (time/1.233+i)*pi;
	// float fi = (mox+i)*pi;

	float x = r * sin(theta) * cos(fi);
	float y = r * sin(theta) * sin(fi);
	float z = r * cos(theta);
	
	float o = 1.0;
	float c = mod((theta/pi)-(time/3), o);
	vec3 color = vec3(c-mox,c-moy,c);
	vec3 pos = vec3(origin.x + x, origin.y + y, origin.z + z) * rotationX(pi/2.3);

	gl_Position = vec4(pos,1.0);
	InterpolatedColor = vec4(color,1.0);
	// gl_PointSize = psz;
}

