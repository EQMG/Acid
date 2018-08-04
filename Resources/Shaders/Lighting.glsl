float sqr(float x)
{
	return x * x;
}

float D_GGX(float NoH, float roughness)
{
	float a2 = sqr(sqr(roughness));
	float denom = sqr(sqr(NoH) * (a2 - 1.0f) + 1.0f) * PI;
	return a2 / denom;
}

float G_sub_GGX(float NoV, float k)
{
	float denom = NoV * (1.0f - k) + k;
	return NoV / denom;
}

float G_GGX(float NoV, float NoL, float k)
{
	// k for direct lighting = sqr(roughness + 1.0) / 8.0;
	// k for IBL = sqr(roughness) / 2.0;
	float ggx_v = G_sub_GGX(NoV, k);
	float ggx_l = G_sub_GGX(NoL, k);
	return ggx_v * ggx_l;
}

vec3 approx_F0(float metallic, vec3 albedo)
{
	return mix(vec3(0.04), albedo, metallic);
}

vec3 F_Schlick(float NoV, vec3 F0)
{
	return F0 + (vec3(1.0f) - F0) * pow(1.0f - NoV, 5.0f);
}

vec3 F_Schlick(float NoV, vec3 F0, float roughness)
{
	return F0 + (max(vec3(1.0f - roughness), F0) - F0) * pow(1.0f - NoV, 5.0f);
}

float brdf_cook_torrance(float NoH, float NoV, float NoL, float roughness)
{
	float D = D_GGX(NoH, roughness);

	float k_direct = sqr(roughness + 1.0f) / 8.0f;
//	float k_ibl = sqr(roughness) / 2.0f;
	float G = G_GGX(NoV, NoL, k_direct);

	float denom = 4.0f * NoL * NoV + EPSILON;

	return D * G / denom;
}

float brdf_lambert()
{
	return 1.0f / PI;
}

float attenuation(float distance, float radius)
{
	if (radius > 0.0f)
	{
		float x = min(distance, radius);
		return sqr(1.0f - sqr(sqr(x / radius))) / (sqr(x) + 1.0f);
	}

	return 1.0f;
}

vec3 L0(vec3 N, vec3 L, vec3 V, float roughness, float metallic, vec3 albedo)
{
	vec3 H = normalize(L + V);
	float NoH = max(0.0f, dot(N, H));
	float NoV = max(0.0f, dot(N, V));
	float NoL = max(0.0f, dot(N, L));
	float HoV = max(0.0f, dot(H, V));

	vec3 F = F_Schlick(HoV, approx_F0(metallic, albedo));

	vec3 kS = F;
	vec3 kD = (vec3(1.0f) - kS) * (1.0f - metallic);

	vec3 specular = kS * brdf_cook_torrance(NoH, NoV, NoL, roughness);
	vec3 diffuse = kD * albedo * brdf_lambert();

	return (diffuse + specular) * NoL;
}

vec3 ibl_irradiance(samplerCube probe, sampler2D brdf_lut, vec3 N, vec3 V, float roughness, float metallic, vec3 albedo)
{
	uint probe_mips = textureQueryLevels(probe);
	float NoV = max(0.0f, dot(N, V));

	vec3 kS = F_Schlick(NoV, approx_F0(metallic, albedo), roughness);
	vec3 kD = (vec3(1.0f) - kS) * (1.0f - metallic);

	vec3 irradiance = textureLod(probe, N, probe_mips - 1).rgb;
	vec3 diffuse = kD * irradiance * albedo;

	vec3 reflected = reflect(-V, N);
	vec2 brdf = texture(brdf_lut, vec2(NoV, roughness)).xy;
	vec3 specular = (kS * brdf.x + brdf.y) * textureLod(probe, reflected, roughness * probe_mips).rgb;

	return diffuse + specular;
}

vec3 decodeWorldPosition(vec2 uv, float depth)
{
	vec3 ndc = vec3(uv * 2.0f - vec2(1.0f), depth);
	vec4 p = inverse(scene.projection * scene.view) * vec4(ndc, 1.0);
	return p.xyz / p.w;
}
