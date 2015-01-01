/*<Material>
  <Params>
  </Params>
  <technique name = "dof" vsEntry = "vsmain" psEntry = "psmain"/>
</Material>*/

#include "header.fx"

uniform vec4 g_DOFParam; // x: _FocusDistance, y: _FocusRange, z: _FadeRange;
uniform vec4 g_texSize;

uniform sampler2D _MainTex;
uniform sampler2D _DepthTex;

varying vec2 coord;

#ifdef VS

void vsmain()
{	
	gl_Position = vec4(vPos, 1.0);
	coord = vPos.xy * 0.5 + 0.5;
}

#endif

#ifdef PS

void psmain()
{
	float SampleWeights[9];
	SampleWeights[0] = 0.003;
	SampleWeights[1] = 0.024;
	SampleWeights[2] = 0.097;
	SampleWeights[3] = 0.226;
	SampleWeights[4] = 0.299;
	SampleWeights[5] = 0.226;
	SampleWeights[6] = 0.097;
	SampleWeights[7] = 0.024;
	SampleWeights[8] = 0.003;

	float depth = DecodeFloatFromRGBA(texture2D(_DepthTex, coord)) * 2.0 - 1.0;
	float scale = (abs(depth - g_DOFParam.x) - g_DOFParam.y) / g_DOFParam.z;
	scale = clamp(scale, 0.0, 1.0);
	scale = pow(scale, 1.5);

	vec3 sum = vec3(0, 0, 0);
	for(int i = 0; i < 9; ++i)
	{
		vec2 offset = g_texSize.xy * (float(i) - 4.0);
		sum += texture2D(_MainTex, coord + offset * scale).rgb * SampleWeights[i];
	}
	gl_FragColor = vec4(sum, 1.0);
}

#endif
