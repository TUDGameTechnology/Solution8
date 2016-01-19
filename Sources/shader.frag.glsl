#ifdef GL_ES
precision mediump float;
#endif

uniform vec4 tint;
uniform sampler2D tex;
varying vec2 texCoord;
varying vec3 normal;

void kore() {
	gl_FragColor = texture2D(tex, texCoord) * tint + normal.x * 0.01; // replace with your BRDF
}
