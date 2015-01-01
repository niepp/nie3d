/*<Material>
  <Params>
  </Params>
  <technique name = "light" vsEntry = "vsmain" psEntry = "psmain"/>
</Material>*/

#include "header.fx"

uniform sampler2D _DepthTex;
uniform sampler2D _GBufferTex;
uniform mat4 g_InverseViewProj;

varying vec4 screenPos;

#ifdef VS

void vsmain()
{
	vec3 pos = vPos * g_PointlightPositionRange.w + g_PointlightPositionRange.xyz;
	screenPos = g_ViewProj * vec4(pos, 1.0);
	gl_Position = screenPos;
}

#endif

#ifdef PS

void psmain()
{
	vec2 projUV = (screenPos.xy / screenPos.w) * 0.5 + 0.5;
	vec4 gbuf = texture2D(_GBufferTex, projUV);
	vec3 wNor;
	float glossy;
	unpackGbuffer(gbuf, wNor, glossy);

	float depth = texture2D(_DepthTex, projUV).x;
	vec3 pos = vec3(projUV, depth);
	vec4 clip = g_InverseViewProj * vec4(pos * 2.0 - 1.0, 1.0);
	pos = clip.xyz / clip.w;

	vec3 viewdir = normalize(g_Eye.xyz - pos);
	vec3 lightDirection = g_PointlightPositionRange.xyz - pos;
	float distSqu = dot(lightDirection, lightDirection);
	vec3 lightDir = normalize(lightDirection);
	float range = g_PointlightPositionRange.w;
	float atten = max(0.0, 1.0 - distSqu/(range * range)) * g_PointlightColorAtten.w;
	float diff = clamp(dot(wNor, lightDir), 0.0, 1.0);
	vec3 halfway = normalize(lightDir + viewdir);
	float ndoth = max(dot(wNor, halfway), 0.0);
	float spec = pow(ndoth, glossy);
	vec4 light = packLight(g_PointlightColorAtten.xyz * diff * atten, spec * atten);
    gl_FragColor = light;

}

#endif

