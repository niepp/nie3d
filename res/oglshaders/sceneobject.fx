/*<Material>
  <Params>
	<var name="diffuseTex" UIName="DiffuseTex" UIType="texture" default="system/texture/default_diffuse.texture" type="string" default="" texcoord="0"/>
	<var name="normalTex" UIName="NormalTex" UIType="texture" default="system/texture/default_normal.texture" type="string" default="" texcoord="0"/>
	<var name="specularTex" UIName="SpecularTex" UIType="texture" default="system/texture/default_specular.texture" type="string" default="" texcoord="0"/>
	<var name="specularGlossy" UIName="Glossy" UIType="float" default="25" type="float" min="0.1" max="64.0"/>
  </Params>
  <Macros>
  </Macros>
  <technique name = "specular" vsEntry = "vsmain" psEntry = "psmain"/>
</Material>*/

#include "header.fx"

uniform sampler2D diffuseTex;
uniform sampler2D normalTex;
uniform sampler2D specularTex;
uniform sampler2D _LightTex;

varying vec2 uv0;
varying vec3 nor;
varying vec4 tan;
varying vec4 screenPos;

#ifdef VS

void vsmain()
{
	screenPos = g_WorldViewProj * vec4(vPos, 1.0);
	gl_Position = screenPos;
	uv0 = vTexCoord0;
	nor = vNormal;
	tan = vTangent;
}

#endif

#ifdef PS

void psmain()
{
	vec4 albedo = texture2D(diffuseTex, uv0);

	// vec3 normal = texture2D(normalTex, uv0).xyz;
	// normal = normalize(normal * 2.0 - 1.0);
	vec3 n = normalize(nor);
	// vec3 t = normalize(tan.xyz);
	// vec3 b = normalize(cross(n, t) * tan.w);
	// mat3 rotMat = mat3(t, b, n);
	// normal = rotMat * normal;
	
	vec2 projUV = (screenPos.xy / screenPos.w) * 0.5 + 0.5;
	vec4 light = texture2D(_LightTex, projUV);
	vec3 diffuse = vec3(0,0,0);
	vec3 specular = vec3(0,0,0);
	unpackLight(light, diffuse, specular);
	vec3 clr = diffuse * albedo.xyz + specular + vec3(0.2, 0.2, 0.2);
	gl_FragColor = vec4(clr, 1.0);
}

#endif

