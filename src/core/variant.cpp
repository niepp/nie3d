#include "core.h"
#include "variant.h"

static const char* s_pTypeName[] =
{
	"null",
	"bool",
	"int",
	"float",
	"float2",
	"float3",
	"float4",
	"float4x4",
	"color",
	"string",
	"object",
	""
};

const char* Variant::TypeToName(VariantType type)
{
	return s_pTypeName[type];
}

VariantType Variant::NameToType(const char *pname)
{
	for (Uint32 i = cVarNull; i < cVarTypeCount; ++i)
	{
		if (strcmp(s_pTypeName[i], pname) == 0) {
			return (VariantType)i;
		}
	}
	assert(0);
	return cVarNull;
}
