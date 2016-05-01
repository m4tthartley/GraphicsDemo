
uniform mat4 uTransform;

varying vec4 vColor;

#ifdef VERTEX_SHADER

	void main () {
		gl_Position = gl_ModelViewProjectionMatrix * ((gl_Vertex) * uTransform);
		vColor = gl_Color;
	}

#endif

#ifdef FRAGMENT_SHADER

	void main () {
		gl_FragColor = vColor;
	}

#endif