R"zzz(#version 330 core
uniform vec4 trans[128];
in vec4 vertex_position;
uniform vec4 light_position;
out vec4 vs_light_direction;
void main()
{
	gl_Position =  trans[gl_InstanceID] + vertex_position;
	vs_light_direction = -gl_Position + light_position;
}
)zzz"
