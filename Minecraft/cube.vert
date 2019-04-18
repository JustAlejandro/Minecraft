R"zzz(#version 330 core
uniform vec3 trans[300];
in vec4 vertex_position;
uniform vec4 light_position;
out vec4 vs_light_direction;
out vec4 world;
in vec2 tex_coord_in;
out vec2 tex_coord;
//Exquisite Garbage
vec2 randoms(vec2 seed){
	float one = fract(tan(dot(vec2(seed.y * 0.2829, seed.x * 0.83247), vec2(14.123456, 45.111))));
	float two = fract(tan(dot(seed, vec2(1.111, 2.222))));
	return vec2(one,two);
}
void main()
{
	gl_Position =  vec4(trans[gl_InstanceID], 0.0) + vertex_position;
	world = gl_Position;
	vs_light_direction = -gl_Position + light_position;
	tex_coord = randoms(vec2(gl_Position.x + gl_Position.z * 0.222, gl_Position.y));
}
)zzz"
