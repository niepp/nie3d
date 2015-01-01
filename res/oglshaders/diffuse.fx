/*<Material>
  <Params>
	<var name="tex0" UIName="Diffuse"  default ="system/texture/default_diffuse.texture" type = "string" usage="texture" default="" texcoord="0"/>
  </Params>
  <technique name = "diffuse" vsname = "vsmain" psname = "psmain"/>
</Material>*/

#include "header.fx"

uniform sampler2D tex0;

varying vec2 uv0;

#ifdef VS

void vsmain()
{
	gl_Position = g_WorldViewProj * vec4(vPos, 1.0);
	uv0 = vTexCoord0;
}

#endif

#ifdef PS

void psmain()
{
	vec4 clr = texture2D(tex0, uv0);
	gl_FragColor = clr;
	//gl_FragColor = vec4(1,0,0,1);
}

#endif

