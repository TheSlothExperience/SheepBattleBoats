#version 330 core

in vec2 UV;

out vec4 outputColor;

uniform sampler2D posTexture;
uniform sampler2D colorTexture;
uniform sampler2D normalTexture;
uniform sampler2D depthTexture;
uniform sampler2D depthTexture2;

uniform float selected;
uniform vec4 activeColor;

const int maxLights = 12;
uniform vec3 lightPositions[maxLights];
uniform vec4 lightColors[maxLights];
uniform int numLights;

uniform mat4 lightViews[maxLights];
uniform mat4 lightPerspectives[maxLights];
uniform sampler2D shadowMaps[maxLights];
uniform sampler2D shadowSATs[maxLights];
uniform mat4 lightBias;

uniform mat4 inverseCameraMatrix;

//Stuff for variance shadow maps -------------------------
float linstep(float a, float b, float v) {
	return clamp((v - a) / (b - a), 0, 1);
}

float chebyshevUpperBound(float d, vec2 moments) {

	//Surface is fully lit
	if(d <= moments.x) {
		return 1.0;
	}

	// The fragment is either in shadow or penumbra. We now use
    // chebyshev's upperBound to check how likely this pixel is
	// to be lit (p_max)
	float variance = moments.y - (moments.x * moments.x);
	variance = max(variance, 0.00002);

	float delta = d - moments.x;
	float p_max = variance / (variance + delta*delta);

	//Reduce light bleeding
	return linstep(0.4, 1.0, p_max);
}
//-------------------------------------------------------

float stepmix(float edge0, float edge1, float E, float x){
    float T = clamp(0.5*(x-edge0+E)/E,0.0,1.0);
    return mix(edge0,edge1,T);
}

void main(){

    vec4 color=vec4(texture(colorTexture,UV));
    vec4 V_=vec4(texture(posTexture,UV));
    vec3 V = V_.xyz;
    float a = V_.a;
    vec3 N=vec3(texture(normalTexture,UV));

    vec3 E = normalize(-V); // we are in Eye Coordinates, so EyePos is (0,0,0)
    int i;
	const float A = 0.1;
	const float B = 0.33;
	const float C = 0.66;
	const float D = 1.0;

	outputColor = vec4(0.0);
	float diffuse = 0.0;

	for(i = 0; i < numLights; i++) {

		vec3 L = normalize(lightPositions[i] - V);
		//Blinn: Using H Vektor for cartoonish reflectance look
		vec3 H = normalize(L+E);

		vec4 lightColor = lightColors[i];

		//Calculate shadow visibility ---------------------------------------------------
		vec4 shCoord = (lightBias * lightPerspectives[i] * lightViews[i]) * inverseCameraMatrix * vec4(V, 1.0);
		vec4 shCoordW = shCoord / shCoord.w;
		float d_receiver = shCoordW.z;
		vec2 moments = texture2D(shadowMaps[i], shCoordW.xy).rg; //Distance to light
		float visibility = chebyshevUpperBound(d_receiver, moments);

		float lightWidth = 1.0; //Make light with fixed. Later dynamic
		vec2 sizes = textureSize(shadowMaps[i], 0);

		if(false) {
			//Search blocker
			vec2 x = vec2(1.0 / sizes.x, 0.0);
			vec2 y = vec2(0.0, 1.0 / sizes.y);
			//Average blocker distance
			float d_blocker =
				texture2D(shadowMaps[i], shCoordW.xy).r
				+ texture2D(shadowMaps[i], shCoordW.xy + x).r
				+ texture2D(shadowMaps[i], shCoordW.xy - x).r
				+ texture2D(shadowMaps[i], shCoordW.xy + y).r
				+ texture2D(shadowMaps[i], shCoordW.xy - y).r;
			d_blocker /= 5.0;
			float w_penumbra = (d_receiver - d_blocker) * lightWidth / d_blocker;
			//Now sample the SAT accordingly
			x *= 2 * w_penumbra;
			y *= 2 * w_penumbra;
			//MAX is in the lower right corner:
			//(x_max, y_max) - (x_max, y_min) - (x_min, y_max) + (x_min, y_min)
			vec2 s =
				texture2D(shadowMaps[i], shCoordW.xy + x - y).rg
				- texture2D(shadowMaps[i], shCoordW.xy + x + y).rg
				- texture2D(shadowMaps[i], shCoordW.xy - x - y).rg
				+ texture2D(shadowMaps[i], shCoordW.xy - x + y).rg;
			s /= (2 * 2 * w_penumbra) * (2 * 2 * w_penumbra);

			visibility = chebyshevUpperBound(d_receiver, s);
		}
		// -----------------------------------------------------------------------------



		diffuse += visibility * max(0.0,dot(N,L));

		float specular = max(0.0,dot(N,H));
		specular = visibility * pow(specular,0.3*80.0);

		//Stylized transformation for spekular highlights
		float edge2 = fwidth(specular);
		if(specular > 0.5-edge2 && specular < 0.5+edge2){
			specular = smoothstep(0.5-edge2,0.5+edge2,specular);
		}else{
			specular = step(0.5,specular);
		}
		//HERE: insert materials for diffuse and specular, if finished
		outputColor += specular * vec4(1.0,1.0,1.0,1.0);
	}

	float edge = fwidth(diffuse);

	//First 3 if's: Antialiasing at transitions of colorbands
	if(diffuse > A-edge && diffuse < A+edge) diffuse = stepmix(A,B,edge,diffuse);
	else if(diffuse > B-edge && diffuse < B+edge) diffuse = stepmix(B,C,edge,diffuse);
	else if(diffuse > C-edge && diffuse < C+edge) diffuse = stepmix(C,D,edge,diffuse);
	else if(diffuse < A) diffuse = 0.0;
	else if(diffuse < B) diffuse = B;
	else if(diffuse < C) diffuse = C;
	else diffuse = D;

	gl_FragDepth = vec4(texture2D(depthTexture, UV)).r;
	outputColor += diffuse*color;
}
