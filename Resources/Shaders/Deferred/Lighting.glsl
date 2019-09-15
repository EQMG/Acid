const float PI = 3.1415926535897932384626433832795;

float sqr(float x) {
	return x * x;
}

float attenuation(float Dl, float radius) {
	if (radius <= 0.0f) {
		return 1.0f;
	}

	float x = min(Dl, radius);
	return sqr(1.0f - sqr(sqr(x / radius))) / (sqr(x) + 1.0f);
}

// Normal Distribution function --------------------------------------
float D_GGX(float dotNH, float roughness) {
	float alpha = roughness * roughness;
	float alpha2 = alpha * alpha;
	float denom = dotNH * dotNH * (alpha2 - 1.0f) + 1.0f;
	return alpha2 / (PI * denom * denom); 
}

// Geometric Shadowing function --------------------------------------
float G_SchlicksmithGGX(float dotNL, float dotNV, float roughness) {
	float r = roughness + 1.0f;
	float k = (r * r) / 8.0f;
	float GL = dotNL / (dotNL * (1.0f - k) + k);
	float GV = dotNV / (dotNV * (1.0f - k) + k);
	return GL * GV;
}

// Fresnel function ----------------------------------------------------
vec3 F_Schlick(float cosTheta, vec3 F0) {
	return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5.0f);
}

vec3 F_SchlickR(float cosTheta, vec3 F0, float roughness) {
	return F0 + (max(vec3(1.0f - roughness), F0) - F0) * pow(1.0f - cosTheta, 5.0f);
}

vec3 prefilteredReflection(vec3 R, float roughness, samplerCube prefiltered) {
	float lod = roughness * float(textureQueryLevels(prefiltered));
	float lodf = floor(lod);
	float lodc = ceil(lod);
	vec3 a = textureLod(prefiltered, R, lodf).rgb;
	vec3 b = textureLod(prefiltered, R, lodc).rgb;
	return mix(a, b, lod - lodf);
}

vec3 specularContribution(vec3 diffuse, vec3 L, vec3 V, vec3 N, vec3 F0, float metallic, float roughness) {
	// Precalculate vectors and dot products	
	vec3 H = normalize(V + L);
	float dotNH = clamp(dot(N, H), 0.0f, 1.0f);
	float dotNV = clamp(dot(N, V), 0.0f, 1.0f);
	float dotNL = clamp(dot(N, L), 0.0f, 1.0f);

	vec3 colour = vec3(0.0f);

	if (dotNL > 0.0f) {
		// D = Normal distribution (Distribution of the microfacets)
		float D = D_GGX(dotNH, roughness); 
		// G = Geometric shadowing term (Microfacets shadowing)
		float G = G_SchlicksmithGGX(dotNL, dotNV, roughness);
		// F = Fresnel factor (Reflectance depending on angle of incidence)
		vec3 F = F_Schlick(dotNV, F0);		
		vec3 spec = D * F * G / (4.0f * dotNL * dotNV + 0.001f);
		vec3 kD = (vec3(1.0f) - F) * (1.0f - metallic);			
		colour += (kD * diffuse / PI + spec) * dotNL;
	}

	return colour;
}

/*float shadowFactor(vec4 shadowCoords) {
	vec3 ndc = shadowCoords.xyz /= shadowCoords.w;
	
	if (abs(ndc.x) > 1.0f || abs(ndc.y) > 1.0f || abs(ndc.z) > 1.0f) {
		return 0.0f;
	}
	
	float shadowValue = texture(samplerShadows, shadowCoords.xy).r;

	if (ndc.z > shadowValue) {
		return 0.0f;
	}

	return 1.0f;
}*/
