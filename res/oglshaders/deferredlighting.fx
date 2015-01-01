/*<Material>
  <Params>
  </Params>
  <technique name = "depth" vsEntry = "depth_vs" psEntry = "depth_ps"/>
  <technique name = "gbuffer" vsEntry = "vsmain" psEntry = "psmain"/>
</Material>*/

#include "header.fx"

uniform vec4 g_NormalSpecularParam; // x: specular glossy

varying vec3 wNor;
varying vec2 depth;

#ifdef VS

void depth_vs()
{
	gl_Position = g_WorldViewProj * vec4(vPos, 1.0);
	depth = gl_Position.zw;
}

void vsmain()
{
	gl_Position = g_WorldViewProj * vec4(vPos, 1.0);
	wNor = (g_LocalToWorld * vec4(vNormal, 0.0)).xyz;
}

#endif

#ifdef PS
void depth_ps()
{
	float d = depth.x / depth.y;
	gl_FragColor = EncodeFloatToRGBA(d * 0.5 + 0.5);
}

void psmain()
{
	gl_FragColor = packGbuffer(normalize(wNor), g_NormalSpecularParam.x);
}

#endif

