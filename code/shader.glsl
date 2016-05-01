
uniform mat4 uRotationMatrix;
uniform mat4 uTransform;

varying vec4 vColor;
varying float light;
varying vec4 colorModifier;

#ifdef VERTEX_SHADER

	void main () {
		gl_Position = gl_ModelViewProjectionMatrix * ((gl_Vertex) * uTransform);
		vColor = gl_Color;

		colorModifier = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		light = 1.0f;

		vec4 lightDir = normalize(vec4(-1.0f, 1.0, 1.0f, 0.0f));
		vec4 vertexNormal = vec4(gl_Normal, 0.0f);

		vec4 rotatedNormal = normalize(vertexNormal * uRotationMatrix);
		float lightDot = dot(rotatedNormal, lightDir);
		light = (lightDot * 0.5f) + 0.5f;

		// if (lightDot < 0.0f) {
		// 	colorModifier = vec4(1.0f, 0.0f, 0.0f, 1.0f);
		// } else if (lightDot > 1.0f) {
		// 	colorModifier = vec4(0.0f, 0.0f, 1.0f, 1.0f);
		// } else {
		// 	light = clamp(lightDot, 0.0f, 1.0f);
		// }
	}

#endif

#ifdef FRAGMENT_SHADER

	void main () {
		// vec4(0.5f, 1.0f, 0.5f, 1.0f)
		gl_FragColor = vColor * colorModifier * light;
	}

#endif