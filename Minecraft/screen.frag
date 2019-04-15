R"zzz(#version 330 core
out vec4 fragment_color;
in vec2 tex_coord;
uniform sampler2D sampler;
void main() {
	fragment_color = vec4(texture(sampler, tex_coord).xyz, 1.0);
}
)zzz"
