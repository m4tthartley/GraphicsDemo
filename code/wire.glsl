
uniform mat4 uProjMatrix;
uniform mat4 uTransform;

varying vec4 vColor;

#ifdef VERTEX_SHADER

	void main () {
		gl_Position = uProjMatrix * ((gl_Vertex) * uTransform);
		vColor = gl_Color;
	}

#endif

#ifdef FRAGMENT_SHADER

	void main () {
		gl_FragColor = vColor;
	}

#endif