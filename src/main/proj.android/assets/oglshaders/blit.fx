/*<Material>
  <Params>
	<var name="MainTex" UIName="MainTex"  default="system/texture/default_diffuse.texture" type="string" usage="texture" default="" texcoord="0"/>
  </Params>
  <technique name = "blit" vsEntry = "vsmain" psEntry = "psmain"/>
</Material>*/

#include "header.fx"

uniform sampler2D _MainTex;
varying vec2 uv;

#ifdef VS

void vsmain()
{	
	gl_Position = vec4(vPos,1.0);
	uv = vPos.xy * 0.5 + 0.5;
}

#endif


#ifdef PS
void psmain()
{	
    vec4 color = texture2D(_MainTex, uv);
	gl_FragColor = color;
}

#endif
