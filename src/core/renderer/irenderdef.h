#pragma once
#ifndef __IRENDER_DEF_H__
#define __IRENDER_DEF_H__

enum ClearMode
{
	cClearStencil = 1,
	cClearTarget  = 2,
	cClearZBuffer = 4,
};

enum LockMode
{
	cLockRead = 0,
	cLockWrite,
	cLockReadWrite,
	cLockFlagCount,
};

enum CullMode
{
	cCullNone = 0,
	cCullCW,
	cCullCCW,
	cCullModeCount,
};

enum FillMode
{
	cFillPoint = 0,
	cFillWireframe,
	cFillSolid,
	cFillModeCount,
};

enum PrimitiveType
{
	cPointList = 0,
	cLineList,
	cLineStrip,
	cTriangleList,
	cTriangleStrip,
	cTriangleFan,
	cPrimitiveTypeCount,
};

enum BlendFuncType
{
	cBlendFuncAdd = 0,
	cBlendFuncSubtract,
	cBlendFuncRevSubtract,
	cBlendFuncMin,
	cBlendFuncMax,
	cBlendFuncTypeCount,
};

enum CmpFunc
{
	cCmpNever = 0,
	cCmpLess,
	cCmpEqual,
	cCmpLEqual,
	cCmpGreater,
	cCmpNotEqual,
	cCmpGEqual,
	cCmpAlways,
	cCmpFuncCount,
};

enum BlendMode
{
	cBlendOpaque = 0,
	cBlendAlphaBlend,
	cBlendAlphaTest,
	cBlendAddMulColor,
	cBlendAddMulAlpha,
	cBlendModeCount,
};

enum BlendFactor
{
	cBlendZero = 0,
	cBlendOne,
	cBlendSrcColor,
	cBlendInvSrcColor,
	cBlendSrcAlpha,
	cBlendInvSrcAlpha,
	cBlendDestAlpha,
	cBlendInvDestAlpha,
	cBlendDestColor,
	cBlendInvDestColor,
	cBlendSrcAlphaSat,
	cBlendBothSrcAlpha,
	cBlendBothInvSrcAlpha,
	cBlendFactor,
	cBlendInvcBlendFactor,
	cBlendSrcColor2,
	cBlendInvsrccolor2,
	cBlendFactorCount,
};

enum RenderStateType
{
	cColorWriteMask = 0,

	cBlendEnable,
	cBlendFunc,
	cSrcBlend,
	cDestBlend,

	cDepthTestEnable,
	cDepthWriteEnable,
	cDepthFunc,

	cStencilEnable,

	cAlphaTestEnable,
	cAlphaFunc,
	cAlphaRefValue,

	cCullMode,
	cFillMode,

	cClipPlane,

	cRenderStateTypeCount,
};

enum FilterState
{
	cFilterTrilinear = 0,
	cFilterBilinear,
	cFilterPoint,
	cFilterAni,
	cFilterStateCount,
};

enum AddressState
{
	cAddressWarp = 0,
	cAddressClamp,
	cAddressBorder,
	cAddressMirror,
	cAddressStateCount,
};

enum SRGBState
{
	cSRGBLookupWithoutCorrection = 0,
	cSRGBLookupWithCorrection,
};

enum InputUsage
{
	cInputUsagePosition = 0,
	cInputUsageBlendWeight,
	cInputUsageBlendIndices,
	cInputUsageNormal,
	cInputUsageTangent,
	cInputUsageBinormal,
	cInputUsageColor,
	cInputUsageTexcoord,

	cInputUsageCount,
};

enum IndexType
{
	cIndex32,
	cIndex16,
	cIndex8,
};

enum UsageType
{
	cUsageDynamic,
	cUsageStatic,

	cUsageTypeCount,
};

enum TextureType
{
	cTexture1D = 1,
	cTexture2D,
	cTexture3D,
	cTextureCube
};

enum TextureUsage
{
	cUsageTexture,
	cUsageDepthStencil,
	cUsageFloat,		// float format texture need gles 3.0
	cTextureUsageCount,
};

enum PixelFormat
{
	cPixelFormatUnknown = 0,
	cPixelFormatR5G6B5,
	cPixelFormatR5G5B5A1,
	cPixelFormatR4G4B4A4,
	cPixelFormatR8G8B8,
	cPixelFormatR8G8B8A8,
	cPixelFormatR8G8B8A8_Unorm,
	cPixelFormatR8G8B8A8_UInt,
	cPixelFormatR8G8B8A8_SNorm,
	cPixelFormatR8G8B8A8_SInt,
	cPixelFormatA8,
	cPixelFormatR10G10B10A2,
	cPixelFormatR16_Float,
	cPixelFormatR16_UInt,
	cPixelFormatR32_Float,
	cPixelFormatR32_UInt,
	cPixelFormatR32G32_Float,
	cPixelFormatR32G32_UInt,
	cPixelFormatR16G16_Float,
	cPixelFormatR16G16_UInt,

	cPixelFormatR32G32B32_Float,
	cPixelFormatR32G32B32A32_Float,
	cPixelFormatBC1,
	cPixelFormatBC2,
	cPixelFormatBC3,

	cPixelFormatD16_Lockable,
	cPixelFormatD32,
	cPixelFormatD15S1,
	cPixelFormatD24S8,
	cPixelFormatD24X8,
	cPixelFormatD24X4S4,
	cPixelFormatD16,
	cPixelFormatD32F_Lockable,
	cPixelFormatD24FS8,
	cPixelFormatR16G16B16A16_Float,
	cPixelFormatL8,
	cPixelFormatR16G16B16A16,
	cPixelFormatINTZ,

	cPixelFormatCount,
};

enum ConstantType
{
	cConstantFloat = 0,
	cConstantFloat2,
	cConstantFloat3,
	cConstantFloat4,
	cConstantInt,
	cConstantInt2,
	cConstantInt3,
	cConstantInt4,
	cConstantUint,
	cConstantUint2,
	cConstantUint3,
	cConstantUint4,
	cConstantMatrix,
	cConstantTexture,
	cConstantTypeCount,
};

enum ShaderParam
{
	cFogColor = 0,
	cHeightFogColor,
	cFogParam,
	cEyePos,
	cTime,
	cColor,
	cBoneMatrix,

	cDirLight,
	cPointLightPositionRange,
	cPointlightColorAtten,

	cWorldViewProj,
	cWorldView,
	cViewProj,
	cLocalToWorld,
	cWorldToLocal,

	cInverseViewProj,
	cNormalSpecularParam,

	cShaderParamCount,
};

#endif //__IRENDER_DEF_H__

