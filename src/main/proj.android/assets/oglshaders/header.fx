#ifdef WINDOWS
	uniform vec4 g_DisFogColor;
	uniform vec4 g_HeightFogColor;
	uniform vec4 g_FogParam;
	uniform vec4 g_Eye;
	uniform float g_Time;
	uniform vec4 g_Color;
	uniform vec4 g_BoneMatrix[180];
	uniform vec4 g_Dirlight;
	uniform vec4 g_PointlightPositionRange;		// xyz:position, w:range
	uniform vec4 g_PointlightColorAtten; // xyz: color*intensity, w:attenuation

	uniform mat4 g_WorldViewProj;
	uniform mat4 g_WorldView;
	uniform mat4 g_ViewProj;
	uniform mat4 g_LocalToWorld;
	uniform mat4 g_WorldToLocal;

	#ifdef VS
	attribute vec3 vPos;
	attribute vec4 blendweights; 
	attribute vec4 blendindices;
	attribute vec3 vNormal;
	attribute vec4 vTangent;
	attribute vec3 vBinormal;
	attribute vec4 vColor;
	attribute vec2 vTexCoord0;
	attribute vec2 vTexCoord1;
	#endif
#endif
#ifdef MOBILE
	precision highp float;
	uniform highp vec4 g_DisFogColor;
	uniform highp vec4 g_HeightFogColor;
	uniform highp vec4 g_FogParam;
	uniform highp vec4 g_Eye;
	uniform highp float g_Time;
	uniform highp vec4 g_Color;
	uniform highp vec4 g_BoneMatrix[180];
	uniform highp vec4 g_Dirlight;
	uniform highp vec4 g_PointlightPositionRange;		// xyz:position, w:range
	uniform highp vec4 g_PointlightColorAtten; // xyz: color*intensity, w:attenuation

	uniform highp mat4 g_WorldViewProj;
	uniform highp mat4 g_WorldView;
	uniform highp mat4 g_ViewProj;
	uniform highp mat4 g_LocalToWorld;
	uniform highp mat4 g_WorldToLocal;

	#ifdef VS
	attribute highp vec3 vPos;
	attribute highp vec4 blendweights; 
	attribute highp vec4 blendindices;
	attribute highp vec3 vNormal;
	attribute highp vec4 vTangent;
	attribute highp vec3 vBinormal;
	attribute highp vec4 vColor;
	attribute highp vec2 vTexCoord0;
	attribute highp vec2 vTexCoord1;
	#endif
#endif

vec4 EncodeFloatToRGBA(float v) {
	vec4 enc = vec4(1.0, 255.0, 65025.0, 16581375.0) * v;
	enc = fract(enc);
	enc -= enc.yzww * vec4(1.0/255.0, 1.0/255.0, 1.0/255.0, 0.0);
	return enc;
}

float DecodeFloatFromRGBA(vec4 rgba) {
	return dot(rgba, vec4(1.0, 1.0/255.0, 1.0/65025.0, 1.0/16581375.0));
}

vec3 EncodeVec2ToRGB(vec2 v)
{
	vec2 enc255 = v * 255.0;
	vec2 residual = floor(fract(enc255) * 16.0);
	return vec3(floor(enc255), residual.x * 16.0 + residual.y) / 255.0;
}

vec2 DecodeRGBFromVec2(vec3 v)
{
	float nz = floor(v.z * 255.0) / 16.0;
	return v.xy + vec2(floor(nz) / 16.0, fract(nz)) / 255.0;
}

vec4 packGbuffer(in vec3 wNormal, in float specularGlossy)
{
	vec3 n = wNormal * 0.5 + 0.5;
	float s = sign(wNormal.z) * specularGlossy / 64.0;
	return vec4(EncodeVec2ToRGB(n.xy), s * 0.5 + 0.5);
}

void unpackGbuffer(in vec4 gbuf, out vec3 wNor, out float glossy)
{
	float s = gbuf.w * 2.0 - 1.0;
	vec2 v = DecodeRGBFromVec2(gbuf.xyz);
	v = v * 2.0 - 1.0;
	float z = sqrt(1.0 - (dot(v,v))) * sign(s);
	wNor = vec3(v, z);
	glossy = abs(s) * 64.0;
}

vec4 packLight(in vec3 diffuse, in float specular)
{
	return vec4(diffuse, specular);
}

void unpackLight(in vec4 light, out vec3 diffuse, out vec3 specular)
{
	diffuse = light.xyz;
	float luminance = dot(light.xyz, vec3(0.299, 0.587, 0.114)) + 0.0001;
//	specular = light.xyz * light.w / luminance;
	specular = vec3(light.w, light.w, light.w);
}

