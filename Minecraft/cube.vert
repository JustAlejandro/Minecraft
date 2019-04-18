R"zzz(#version 330 core
uniform vec3 trans[300];
in vec4 vertex_position;
uniform vec4 light_position;
out vec4 vs_light_direction;
void main()
{
	gl_Position =  vec4(trans[gl_InstanceID], 0.0) + vertex_position;
	vs_light_direction = -gl_Position + light_position;
}
)zzz"
