R"zzz(#version 430 core
flat in vec4 normal;
in vec4 light_direction;
out vec4 fragment_color;
void main()
{
	vec4 color = vec4(0.0, 0.0, 0.0, 1.0);
	float dot_nl = dot(normalize(light_direction), normalize(normal));
	dot_nl = clamp(dot_nl, 0.0, 1.0);

	if (normal.x > .666){
		color = vec4(1.0, 0.0, 0.0, 1.0);
	}
	if (normal.y > 0.666){
		color = vec4(0.0, 1.0, 0.0, 1.0);
	}
	if (normal.z > 0.666){
		color = vec4(0.0, 0.0, 1.0, 1.0);
	}
	if (normal.x < -0.666){
		color = vec4(1.0, 0.0, 0.0, 1.0);
	}
	if (normal.y < -0.666){
		color = vec4(0.0, 1.0, 0.0, 1.0);
	}
	if (normal.z < -0.666){
		color = vec4(0.0, 0.0, 1.0, 1.0);
	}
	
	// Super Simple Shading
	fragment_color = color * clamp(dot_nl * color, 0.0, 1.0);
}
)zzz"