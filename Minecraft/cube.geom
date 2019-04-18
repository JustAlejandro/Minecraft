R"zzz(#version 430 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
uniform mat4 projection;
uniform mat4 view;
in vec4 vs_light_direction[];
flat out vec4 normal;
out vec4 light_direction;
out mat4 proj;
out mat4 v;
void main()
{
	int n = 0;
	vec4 c0 = gl_in[0].gl_Position;
	vec4 c1 = gl_in[1].gl_Position;
	vec4 c2 = gl_in[2].gl_Position;
	// Generate the normal
	normal = vec4(normalize(cross(c1.xyz - c2.xyz, c0.xyz - c2.xyz)),0.0);

	for (n = 0; n < gl_in.length(); n++) {
		gl_Position = projection * view * gl_in[n].gl_Position;
		light_direction = vs_light_direction[n];
		EmitVertex();
	}
	EndPrimitive();
}
)zzz"