
varying vec4 vColor;
varying float light;

#ifdef VERTEX_SHADER

void main () {
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	vColor = gl_Color;

	vec4 lightDir = vec4(-1.0f, 1.0, -1.0f, 0.0f);
	vec4 vertexNormal = vec4(gl_Normal, 0.0f);
	// gl_ModelViewMatrix[2][3] = 0.0f;
	vec4 rotatedNormal = normalize(gl_ModelViewMatrix * vertexNormal);
	light = saturate(dot(rotatedNormal, normalize(lightDir)));
}

#endif

#ifdef FRAGMENT_SHADER

void main () {
	// vec4(0.5f, 1.0f, 0.5f, 1.0f)
	gl_FragColor = vColor * light;
}

#endif