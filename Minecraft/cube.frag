R"zzz(#version 430 core
flat in vec4 normal;
in vec4 light_direction;
out vec4 fragment_color;
in vec4 worldCoord;
uniform sampler2D sampler;
in vec2 tex_C;
void main()
{
	vec4 color = vec4(0.4, 0.4, 0.4, 1.0);
	float dot_nl = dot(normalize(light_direction), normalize(normal));
	dot_nl = clamp(dot_nl + 0.5, 0.0, 1.0);
	
	float thing = min(max(texture(sampler, tex_C).r,0.0),1.0);
	thing = (thing + 1.0)/2.5;
	thing = (thing + 1.0)/2.0;
	thing = (thing + 1.0)/2.0;
	if(worldCoord.y > 2.0005){
		color = vec4(0.5,0.5,0.5,1.0);
	}
	if(worldCoord.y > 4.0005){
		color = vec4(0.1, 0.7, 0.2, 1.0);
	}
	if(worldCoord.y > 10.0005){
		color = vec4(1.0, 1.0, 1.0, 1.0);
	}
	
	
	color = clamp(thing * color, 0.0,1.0);
	// Super Simple Shading
	fragment_color = color * clamp(dot_nl * color, 0.0, 1.0);
}
)zzz"