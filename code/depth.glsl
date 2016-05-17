
 #version 330

uniform mat4 uProjMatrix;
uniform mat4 uTransform;

uniform vec4 lightPosition;

// varying vec4 vColor;

#ifdef VERTEX_SHADER

	out vec3 position;

	void main () {
		gl_Position = uProjMatrix * ((gl_Vertex) * uTransform);
		// vColor = gl_Color;
		position = (gl_Vertex * uTransform).xyz;
	}

#endif

#ifdef FRAGMENT_SHADER

	in vec3 position;
	out float fragColor;

	void main () {
		fragColor = length(position - lightPosition);
	}

#endif