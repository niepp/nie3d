/*<Material>
  <Params/>
  <technique name = "tech0" vsEntry = "vsmain" psEntry = "psmain"/>
</Material>*/

#include "header.fx"

varying vec4 color;

#ifdef VS

void vsmain()
{
	gl_Position = g_WorldViewProj * vec4(vPos, 1.0);
	color = vColor;
}

#endif

#ifdef PS

void psmain()
{
	gl_FragColor = color;
}

#endif

