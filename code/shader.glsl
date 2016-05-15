
uniform mat4 uProjMatrix;
uniform mat4 uRotationMatrix;
uniform mat4 uTransform;

uniform vec4 lightPosition;

varying vec4 vColor;
varying float light;
varying vec4 colorModifier;

varying vec4 cameraVector;
varying vec3 vNormal;

#ifdef VERTEX_SHADER

	void main () {
		gl_Position = uProjMatrix * ((gl_Vertex) * uTransform);
		vColor = gl_Color;
		vColor = vec4(0.75f, 0.75f, 0.75f, 1.0f);

		colorModifier = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		light = 1.0f;

		vec4 lightDir = normalize(lightPosition);
		vec4 vertexNormal = vec4(gl_Normal, 0.0f);

		vec4 rotatedNormal = normalize(vertexNormal * uRotationMatrix);
		float lightDot = dot(rotatedNormal, lightDir);
		light = (lightDot * 0.5f) + 0.5f;

		float attenuation = ((gl_Vertex * uTransform) - lightPosition) / 20.0f;
		light *= attenuation;

		// if (lightDot < 0.0f) {
		// 	colorModifier = vec4(1.0f, 0.0f, 0.0f, 1.0f);
		// } else if (lightDot > 1.0f) {
		// 	colorModifier = vec4(0.0f, 0.0f, 1.0f, 1.0f);
		// } else {
		// 	light = clamp(lightDot, 0.0f, 1.0f);
		// }

		cameraVector = normalize(-(gl_Vertex * uTransform));
		vNormal = rotatedNormal;
	}

#endif

#ifdef FRAGMENT_SHADER

	void main () {
		// vec4(0.5f, 1.0f, 0.5f, 1.0f)

		vec3 lightDir = normalize(lightPosition.xyz);

		vec3 halfVector = normalize(lightDir + cameraVector);
		vec4 specularColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		float shininess = 50.0f;
		vec4 specular = specularColor * pow(max(dot(halfVector, vNormal), 0.0f), shininess);
		specular *= 1.0f - (vColor * light);

		vec4 ambient = 0.2f;

		gl_FragColor = max((vColor * light) + specular, ambient);
	}

#endif