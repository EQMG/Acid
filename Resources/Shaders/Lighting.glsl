const float pi = 3.1415926535897932384626433832795f;
const float epsilon = 0.001f;

float sqr(float x)
{
	return x * x;
}

vec2 hammersley(uint i, uint N)
{
	uint bits = (i << 16u) | (i >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	float radical_inverse = float(bits) * 2.3283064365386963e-10f;
	return vec2(float(i) / float(N), radical_inverse);
}

vec3 cube_dir(vec2 texCoord, uint side)
{
	vec2 tex = texCoord * 2.f - 1.0f;
	if(side == 0) return vec3(1.0f, -tex.y, -tex.x); // Front.
	if(side == 1) return vec3(-1.0f, -tex.y, tex.x); // Back,
	if(side == 2) return vec3(tex.x, 1.0f, tex.y); // Right.
	if(side == 3) return vec3(tex.x, -1.0f, -tex.y); // Left.
	if(side == 4) return vec3(tex.x, -tex.y, 1.0f); // Top.
	if(side == 5) return vec3(-tex.x, -tex.y, -1.0f); // Bottom.
	return vec3(1.0);
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

float D_GGX(float NoH, float roughness)
{
	float a2 = sqr(sqr(roughness));
	float denom = sqr(sqr(NoH) * (a2 - 1.0f) + 1.0f) * pi;
	return a2 / denom;
}


float G_sub_GGX(float NoV, float k)
{
	float denom = NoV * (1.0f - k) + k;
	return NoV / denom;
}

float G_GGX(float NoV, float NoL, float k)
{
	float ggx_v = G_sub_GGX(NoV, k);
	float ggx_l = G_sub_GGX(NoL, k);
	return ggx_v * ggx_l;
}

vec3 approx_F0(float metallic, vec3 colour)
{
	return mix(vec3(0.04), colour, metallic);
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

	float denom = 4.0f * NoL * NoV + epsilon;

	return D * G / denom;
}

float brdf_lambert()
{
	return 1.0f / pi;
}

vec3 L0(vec3 N, vec3 L, vec3 V, float roughness, float metallic, vec3 colour)
{
	vec3 H = normalize(L + V);
	float NoH = max(0.0f, dot(N, H));
	float NoV = max(0.0f, dot(N, V));
	float NoL = max(0.0f, dot(N, L));
	float HoV = max(0.0f, dot(H, V));

	vec3 F = F_Schlick(HoV, approx_F0(metallic, colour));

	vec3 kS = F;
	vec3 kD = (vec3(1.0f) - kS) * (1.0f - metallic);

	vec3 specular = kS * brdf_cook_torrance(NoH, NoV, NoL, roughness);
	vec3 diffuse = kD * colour * brdf_lambert();

	return (diffuse + specular) * NoL;
}

vec3 importance_sample_GGX(vec2 Xi, vec3 normal, float roughness)
{
	float a2 = sqr(sqr(roughness));

	float phi = 2.0f * pi * Xi.x;
	float cos_theta = sqrt((1.0f - Xi.y) / (1.0f + (a2 - 1.0f) * Xi.y));
	float sin_theta = sqrt(1.0f - sqr(cos_theta));

	vec3 half_vec = vec3(cos(phi) * sin_theta, sin(phi) * sin_theta, cos_theta);

	vec3 up = abs(normal.z) < 0.999f ? vec3(0.0f, 0.0f, 1.0f) : vec3(1.0f, 0.0f, 0.0f);
	vec3 tangent = normalize(cross(up, normal));
	vec3 bitangent = cross(normal, tangent);

	return normalize(tangent * half_vec.x + bitangent * half_vec.y + normal * half_vec.z);
}

vec2 integrate_brdf(float NoV, float roughness)
{
	vec3 V = vec3(sqrt(1.0f - NoV * NoV), 0.0f, NoV);

	vec2 integr = vec2(0.0f);

	vec3 N = vec3(0.0f, 0.0f, 1.0f);
	const uint SAMPLE_COUNT = 1024;

	for (uint i = 0; i != SAMPLE_COUNT; ++i)
	{
		vec2 Xi = hammersley(i, SAMPLE_COUNT);
		vec3 H  = importance_sample_GGX(Xi, N, roughness);
		vec3 L  = normalize(2.0f * dot(V, H) * H - V);

		float NoL = max(0.0f, L.z);
		float NoH = max(0.0f, H.z);
		float VoH = max(0.0f, dot(V, H));

		if (NoL > 0.0f)
		{
			float k_ibl = sqr(roughness) * 0.5f;
			float G = G_GGX(NoV, NoL, k_ibl);
			float G_Vis = (G * VoH) / (NoH * NoV);
			float Fc = pow(1.0f - VoH, 5.0f);

			integr += vec2((1.0f - Fc) * G_Vis, Fc * G_Vis);
		}
	}

	return integr / SAMPLE_COUNT;
}

vec3 ibl_irradiance(samplerCube probe, sampler2D brdf_lut, vec3 N, vec3 V, float roughness, float metallic, vec3 colour)
{
	uint probe_mips = textureQueryLevels(probe);
	float NoV = max(0.0f, dot(N, V));

	vec3 kS = F_Schlick(NoV, approx_F0(metallic, colour), roughness);
	vec3 kD = (vec3(1.0f) - kS) * (1.0f - metallic);

	vec3 irradiance = textureLod(probe, N, probe_mips - 1).rgb;
	vec3 diffuse = kD * irradiance * colour;

	vec3 reflected = reflect(-V, N);
	vec2 brdf = texture(brdf_lut, vec2(NoV, roughness)).xy;
	vec3 specular = (kS * brdf.x + brdf.y) * textureLod(probe, reflected, roughness * probe_mips).rgb;

	return diffuse + specular;
}

vec3 diffuse_convolution(samplerCube envmap, vec3 normal)
{
	vec3 acc = vec3(0.0f);
	vec3 up = abs(normal.z) < 0.999f ? vec3(0.0f, 0.0f, 1.0f) : vec3(1.0f, 0.0f, 0.0f);
	vec3 right = normalize(cross(up, normal));
	up = cross(normal, right);
	float sample_delta = 0.05f;
	float samples = 0.0f;

	for (float phi = 0.0f; phi < 2.0f * pi; phi += sample_delta)
	{
		for (float theta = 0.0f; theta < 0.5f * pi; theta += sample_delta)
		{
			vec3 tangent_sample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
			vec3 sample_dir = tangent_sample.x * right + tangent_sample.y * up + tangent_sample.z * normal;

			acc += texture(envmap, sample_dir).rgb * cos(theta) * sin(theta);
			++samples;
		}
	}

	return acc / samples * pi;
}

vec3 specular_convolution(samplerCube envmap, vec3 N, float roughness)
{
	vec3 V = N;
	float total = 0.0f;
	vec3 acc = vec3(0.0f);
	const uint SAMPLE_COUNT = 1024;

	for (uint i = 0; i != SAMPLE_COUNT; ++i)
	{
		vec2 Xi = hammersley(i, SAMPLE_COUNT);
		vec3 H  = importance_sample_GGX(Xi, N, roughness);
		vec3 L  = normalize(2.0f * dot(V, H) * H - V);
		float NoL = max(0.0f, dot(N, L));

		if (NoL > 0.0f)
		{
			acc += texture(envmap, L).rgb * NoL;
			total += NoL;
		}
	}

	return acc / total;
}
