#version 450

uniform vec4 tint;

uniform sampler2D tex;

in vec2 texCoord;
in vec3 normal;

out vec4 FragColor;

void main() {
	FragColor = texture(tex, texCoord) * tint + normal.x * 0.01; // replace with your BRDF
}
