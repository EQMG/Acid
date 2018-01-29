/*
 * mvk_datatypes.h
 *
 * Copyright (c) 2014-2017 The Brenwill Workshop Ltd. All rights reserved.
 * http://www.brenwill.com
 *
 * Use of this document is governed by the Molten License Agreement, as included
 * in the MoltenVK distribution package. CAREFULLY READ THAT LICENSE AGREEMENT BEFORE
 * READING AND USING THIS DOCUMENT. BY READING OR OTHERWISE USING THIS DOCUMENT,
 * YOU ACCEPT AND AGREE TO BE BOUND BY THE TERMS AND CONDITIONS OF THAT LICENSE
 * AGREEMENT. IF YOU DO NOT ACCEPT THE TERMS AND CONDITIONS OF THAT LICENSE AGREEMENT,
 * DO NOT READ OR USE THIS DOCUMENT.
 */


/* 
 * This file contains functions for converting between Vulkan and Metal data types.
 *
 * The functions here are used internally by MoltenVK, and are exposed here 
 * as a convenience for use elsewhere within applications using MoltenVK.
 */

#ifndef __mvkDataTypes_h_
#define __mvkDataTypes_h_ 1

#ifdef __cplusplus
extern "C" {
#endif	//  __cplusplus
	
#include <MoltenVK/vulkan/vulkan.h>

#import <Metal/Metal.h>
#import <CoreGraphics/CoreGraphics.h>


#pragma mark -
#pragma mark Image properties

#pragma mark Texture formats

/** Enumerates the data type of a format. */
typedef enum {
    kMVKFormatNone,             /**< Format type is unknown. */
    kMVKFormatColorFloat,		/**< A floating point color. */
    kMVKFormatColorInt,         /**< A signed integer color. */
    kMVKFormatColorUInt,		/**< An unsigned integer color. */
    kMVKFormatDepthStencil,     /**< A depth and stencil value. */
} MVKFormatType;

/** Returns the format type corresponding to the specified Vulkan VkFormat, */
MVKFormatType mvkFormatTypeFromVkFormat(VkFormat vkFormat);

/** Returns the format type corresponding to the specified Metal MTLPixelFormat, */
MVKFormatType mvkFormatTypeFromMTLPixelFormat(MTLPixelFormat mtlFormat);

/**
 * Returns the Metal MTLPixelFormat corresponding to the specified Vulkan VkFormat,
 * or returns MTLPixelFormatInvalid if no corresponding MTLPixelFormat exists.
 *
 * Not all MTLPixelFormats returned by this function are supported by all GPU's, 
 * and, internally, MoltenVK may substitute and use a different MTLPixelFormat than
 * is returned by this function for a particular Vulkan VkFormat value.
 *
 * Not all macOS GPU's support the MTLPixelFormatDepth24Unorm_Stencil8 pixel format.
 * Even though this function will return that value when passed the corresponding 
 * VkFormat value, internally, MoltenVK will use the MTLPixelFormatDepth32Float_Stencil8
 * instead when a GPU does not support the MTLPixelFormatDepth24Unorm_Stencil8 pixel format.
 * On an macOS device that has more than one GPU, one of the GPU's may support the
 * MTLPixelFormatDepth24Unorm_Stencil8 pixel format while another may not.
 */
MTLPixelFormat mvkMTLPixelFormatFromVkFormat(VkFormat vkFormat);

/**
 * Returns the Vulkan VkFormat corresponding to the specified Metal MTLPixelFormat,
 * or returns VK_FORMAT_UNDEFINED if no corresponding VkFormat exists.
 */
VkFormat mvkVkFormatFromMTLPixelFormat(MTLPixelFormat mtlFormat);

/**
 * Returns the size, in bytes, of a texel block of the specified Vulkan format.
 * For uncompressed formats, the returned value corresponds to the size in bytes of a single texel.
 */
uint32_t mvkVkFormatBytesPerBlock(VkFormat vkFormat);

/** 
 * Returns the size, in bytes, of a texel block of the specified Metal format.
 * For uncompressed formats, the returned value corresponds to the size in bytes of a single texel.
 */
uint32_t mvkMTLPixelFormatBytesPerBlock(MTLPixelFormat mtlFormat);

/**
 * Returns the size of the compression block, measured in texels for a Vulkan format.
 * The returned value will be {1, 1} for non-compressed formats.
 */
VkExtent2D mvkVkFormatBlockTexelSize(VkFormat vkFormat);

/**
 * Returns the size of the compression block, measured in texels for a Metal format.
 * The returned value will be {1, 1} for non-compressed formats.
 */
VkExtent2D mvkMTLPixelFormatBlockTexelSize(MTLPixelFormat mtlFormat);

/**
 * Returns the size, in bytes, of a texel of the specified Vulkan format.
 * The returned value may be fractional for certain compressed formats.
 */
float mvkVkFormatBytesPerTexel(VkFormat vkFormat);

/**
 * Returns the size, in bytes, of a texel of the specified Metal format.
 * The returned value may be fractional for certain compressed formats.
 */
float mvkMTLPixelFormatBytesPerTexel(MTLPixelFormat mtlFormat);

/**
 * Returns the size, in bytes, of a row of texels of the specified Vulkan format.
 * For compressed formats, this takes into consideration the compression block size,
 * and texelsPerRow should specify the width in texels, not blocks.
 */
size_t mvkVkFormatBytesPerRow(VkFormat vkFormat, uint32_t texelsPerRow);

/**
 * Returns the size, in bytes, of a row of texels of the specified Metal format. 
 * For compressed formats, this takes into consideration the compression block size,
 * and texelsPerRow should specify the width in texels, not blocks.
 */
size_t mvkMTLPixelFormatBytesPerRow(MTLPixelFormat mtlFormat, uint32_t texelsPerRow);

/**
 * Returns the size, in bytes, of a texture layer of the specified Vulkan format.
 * For compressed formats, this takes into consideration the compression block size,
 * and texelRowsPerLayer should specify the height in texels, not blocks.
 */
size_t mvkVkFormatBytesPerLayer(VkFormat vkFormat, size_t bytesPerRow, uint32_t texelRowsPerLayer);

/**
 * Returns the size, in bytes, of a texture layer of the specified Metal format.
 * For compressed formats, this takes into consideration the compression block size,
 * and texelRowsPerLayer should specify the height in texels, not blocks.
 */
size_t mvkMTLPixelFormatBytesPerLayer(MTLPixelFormat mtlFormat, size_t bytesPerRow, uint32_t texelRowsPerLayer);

/** 
 * Returns the default properties for the specified Vulkan format.
 *
 * Not all MTLPixelFormats returned by this function are supported by all GPU's,
 * and, as a result, MoltenVK may return a different value from the 
 * vkGetPhysicalDeviceFormatProperties() function than is returned here.
 *
 * Not all macOS GPU's support the MTLPixelFormatDepth24Unorm_Stencil8 
 * (VK_FORMAT_D24_UNORM_S8_UINT) pixel format. On an macOS device that has more 
 * than one GPU, one of the GPU's may support that format, while another may not.
 * Use the vkGetPhysicalDeviceFormatProperties() function to return the properties
 * for a particular GPU.
 */
VkFormatProperties mvkVkFormatProperties(VkFormat vkFormat);

/** Returns the name of the specified Vulkan format. */
const char* mvkVkFormatName(VkFormat vkFormat);

/** Returns the name of the specified Metal pixel format. */
const char* mvkMTLPixelFormatName(MTLPixelFormat mtlFormat);

/**
 * Returns the MTLClearColor value corresponding to the color value in the VkClearValue,
 * extracting the color value that is VkFormat for the VkFormat.
 */
MTLClearColor mvkMTLClearColorFromVkClearValue(VkClearValue vkClearValue,
											   VkFormat vkFormat);

/** Returns the Metal depth value corresponding to the depth value in the specified VkClearValue. */
double mvkMTLClearDepthFromVkClearValue(VkClearValue vkClearValue);

/** Returns the Metal stencil value corresponding to the stencil value in the specified VkClearValue. */
uint32_t mvkMTLClearStencilFromVkClearValue(VkClearValue vkClearValue);

/** Returns whether the specified Metal MTLPixelFormat can be used as a depth format. */
bool mvkMTLPixelFormatIsDepthFormat(MTLPixelFormat mtlFormat);

/** Returns whether the specified Metal MTLPixelFormat can be used as a stencil format. */
bool mvkMTLPixelFormatIsStencilFormat(MTLPixelFormat mtlFormat);

/** Returns the Metal texture type from the specified Vulkan image properties. */
MTLTextureType mvkMTLTextureTypeFromVkImageType(VkImageType vkImageType,
												uint32_t arraySize,
												bool isMultisample);

/** Returns the Vulkan image type from the Metal texture type. */
VkImageType mvkVkImageTypeFromMTLTextureType(MTLTextureType mtlTextureType);

/** Returns the Metal MTLTextureType corresponding to the Vulkan VkImageViewType. */
MTLTextureType mvkMTLTextureTypeFromVkImageViewType(VkImageViewType vkImageViewType, bool isMultisample);

/** Returns the Metal texture usage from the Vulkan image usage. */
MTLTextureUsage mvkMTLTextureUsageFromVkImageUsageFlags(VkImageUsageFlags vkImageUsageFlags);

/** Returns the Vulkan image usage from the Metal texture usage and format. */
VkImageUsageFlags mvkVkImageUsageFlagsFromMTLTextureUsage(MTLTextureUsage mtlUsage, MTLPixelFormat mtlFormat);

/**
 * Returns the numeric sample count corresponding to the specified Vulkan sample count flag.
 *
 * The specified flags value should have only one bit set, otherwise an invalid numeric value will be returned.
 */
uint32_t mvkSampleCountFromVkSampleCountFlagBits(VkSampleCountFlagBits vkSampleCountFlag);

/** Returns the Vulkan bit flags corresponding to the numeric sample count, which must be a PoT value. */
VkSampleCountFlagBits mvkVkSampleCountFlagBitsFromSampleCount(NSUInteger sampleCount);


#pragma mark Mipmaps

/**
 * Returns the number of mipmap levels available to an image with the specified side dimension.
 * 
 * If the specified dimension is a power-of-two, the value returned is (log2(dim) + 1).
 * If the specified dimension is NOT a power-of-two, the value returned is 0, indicating
 * that the image cannot support mipmaps.
 */
uint32_t mvkMipmapLevels(uint32_t dim);

/**
 * Returns the number of mipmap levels available to an image with the specified extent.
 *
 * If each dimension in the specified extent is a power-of-two, the value returned
 * is MAX(log2(dim) + 1) across both dimensions. If either dimension in the specified 
 * extent is NOT a power-of-two, the value returned is 1, indicating that the image 
 * cannot support mipmaps, and that only the base mip level can be used.
 */
uint32_t mvkMipmapLevels2D(VkExtent2D extent);

/**
 * Returns the number of mipmap levels available to an image with the specified extent.
 *
 * If each dimension in the specified extent is a power-of-two, the value returned
 * is MAX(log2(dim) + 1) across all dimensions. If either dimension in the specified
 * extent is NOT a power-of-two, the value returned is 1, indicating that the image
 * cannot support mipmaps, and that only the base mip level can be used.
 */
uint32_t mvkMipmapLevels3D(VkExtent3D extent);

/** 
 * Returns the size of the specified zero-based mipmap level, 
 * when the size of the base level is the specified size. 
 */
VkExtent2D mvkMipmapLevelSizeFromBaseSize(VkExtent2D baseSize, uint32_t level);

/** 
 * Returns the size of the mipmap base level, when the size of 
 * the specified zero-based mipmap level is the specified size.
 */
VkExtent2D mvkMipmapBaseSizeFromLevelSize(VkExtent2D levelSize, uint32_t level);


#pragma mark Samplers

/**
 * Returns the Metal MTLSamplerAddressMode corresponding to the specified Vulkan VkSamplerAddressMode,
 * or returns MTLSamplerAddressModeMirrorClampToEdge if no corresponding MTLSamplerAddressMode exists.
 */
MTLSamplerAddressMode mvkMTLSamplerAddressModeFromVkSamplerAddressMode(VkSamplerAddressMode vkMode);

/**
 * Returns the Metal MTLSamplerMinMagFilter corresponding to the specified Vulkan VkFilter,
 * or returns MTLSamplerMinMagFilterNearest if no corresponding MTLSamplerMinMagFilter exists.
 */
MTLSamplerMinMagFilter mvkMTLSamplerMinMagFilterFromVkFilter(VkFilter vkFilter);

/**
 * Returns the Metal MTLSamplerMipFilter corresponding to the specified Vulkan VkSamplerMipmapMode,
 * or returns MTLSamplerMipFilterNotMipmapped if no corresponding MTLSamplerMipFilter exists.
 */
MTLSamplerMipFilter mvkMTLSamplerMipFilterFromVkSamplerMipmapMode(VkSamplerMipmapMode vkMode);


#pragma mark -
#pragma mark Render pipeline

/** Returns the Metal MTLColorWriteMask corresponding to the specified Vulkan VkColorComponentFlags. */
MTLColorWriteMask mvkMTLColorWriteMaskFromVkChannelFlags(VkColorComponentFlags vkWriteFlags);

/** Returns the Metal MTLBlendOperation corresponding to the specified Vulkan VkBlendOp. */
MTLBlendOperation mvkMTLBlendOperationFromVkBlendOp(VkBlendOp vkBlendOp);

/** Returns the Metal MTLBlendFactor corresponding to the specified Vulkan VkBlendFactor. */
MTLBlendFactor mvkMTLBlendFactorFromVkBlendFactor(VkBlendFactor vkBlendFactor);

/**
 * Returns the Metal MTLVertexFormat corresponding to the specified
 * Vulkan VkFormat as used as a vertex attribute format.
 */
MTLVertexFormat mvkMTLVertexFormatFromVkFormat(VkFormat vkFormat);

/** Returns the Metal MTLVertexStepFunction corresponding to the specified Vulkan VkVertexInputRate. */
MTLVertexStepFunction mvkMTLVertexStepFunctionFromVkVertexInputRate(VkVertexInputRate vkVtxStep);

/** Returns the Metal MTLPrimitiveType corresponding to the specified Vulkan VkPrimitiveTopology. */
MTLPrimitiveType mvkMTLPrimitiveTypeFromVkPrimitiveTopology(VkPrimitiveTopology vkTopology);

#if MLN_MACOS
/** Returns the Metal MTLPrimitiveTopologyClass corresponding to the specified Vulkan VkPrimitiveTopology. */
MTLPrimitiveTopologyClass mvkMTLPrimitiveTopologyClassFromVkPrimitiveTopology(VkPrimitiveTopology vkTopology);
#endif

/** Returns the Metal MTLLoadAction corresponding to the specified Vulkan VkAttachmentLoadOp. */
MTLLoadAction mvkMTLLoadActionFromVkAttachmentLoadOp(VkAttachmentLoadOp vkLoadOp);

/** Returns the Metal MTLStoreAction corresponding to the specified Vulkan VkAttachmentStoreOp. */
MTLStoreAction mvkMTLStoreActionFromVkAttachmentStoreOp(VkAttachmentStoreOp vkStoreOp);

/** Returns the Metal MTLViewport corresponding to the specified Vulkan VkViewport. */
MTLViewport mvkMTLViewportFromVkViewport(VkViewport vkViewport);

/** Returns the Metal MTLScissorRect corresponding to the specified Vulkan VkRect2D. */
MTLScissorRect mvkMTLScissorRectFromVkRect2D(VkRect2D vkRect);

/** Returns the Metal MTLCompareFunction corresponding to the specified Vulkan VkCompareOp, */
MTLCompareFunction mvkMTLCompareFunctionFromVkCompareOp(VkCompareOp vkOp);

/** Returns the Metal MTLStencilOperation corresponding to the specified Vulkan VkStencilOp, */
MTLStencilOperation mvkMTLStencilOperationFromVkStencilOp(VkStencilOp vkOp);

/** Returns the Metal MTLCullMode corresponding to the specified Vulkan VkCullModeFlags, */
MTLCullMode mvkMTLCullModeFromVkCullModeFlags(VkCullModeFlags vkCull);

/** Returns the Metal MTLWinding corresponding to the specified Vulkan VkFrontFace, */
MTLWinding mvkMTLWindingFromVkFrontFace(VkFrontFace vkWinding);

/** Returns the Metal MTLTriangleFillMode corresponding to the specified Vulkan VkPolygonMode, */
MTLTriangleFillMode mvkMTLTriangleFillModeFromVkPolygonMode(VkPolygonMode vkFillMode);

/** Returns the Metal MTLIndexType corresponding to the specified Vulkan VkIndexType, */
MTLIndexType mvkMTLIndexTypeFromVkIndexType(VkIndexType vkIdxType);

/** Returns the size, in bytes, of a vertex index of the specified type. */
size_t mvkMTLIndexTypeSizeInBytes(MTLIndexType mtlIdxType);


#pragma mark -
#pragma mark Geometry conversions

/** Returns a VkExtent2D that corresponds to the specified CGSize. */
static inline VkExtent2D mvkVkExtent2DFromCGSize(CGSize cgSize) {
	VkExtent2D vkExt;
	vkExt.width = cgSize.width;
	vkExt.height = cgSize.height;
	return vkExt;
}

/** Returns a Metal MTLOrigin constructed from the specified VkOffset3D. */
static inline MTLOrigin mvkMTLOriginFromVkOffset3D(VkOffset3D vkOffset) {
	return MTLOriginMake(vkOffset.x, vkOffset.y, vkOffset.z);
}

/** Returns a Metal MTLSize constructed from the specified VkExtent3D. */
static inline MTLSize mvkMTLSizeFromVkExtent3D(VkExtent3D vkExtent) {
	return MTLSizeMake(vkExtent.width, vkExtent.height, vkExtent.depth);
}


#pragma mark -
#pragma mark Memory options

/** Macro indicating the Vulkan memory type bits corresponding to Metal private memory (not host visible). */
#define MVK_VK_MEMORY_TYPE_METAL_PRIVATE	(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)

/** Macro indicating the Vulkan memory type bits corresponding to Metal managed memory (host visible and non-coherent). */
#define MVK_VK_MEMORY_TYPE_METAL_MANAGED	(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT)

/** Macro indicating the Vulkan memory type bits corresponding to Metal shared memory (host visible and coherent). */
#define MVK_VK_MEMORY_TYPE_METAL_SHARED		(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT)

/** Returns the Metal storage mode corresponding to the specified Vulkan memory flags. */
MTLStorageMode mvkMTLStorageModeFromVkMemoryPropertyFlags(VkMemoryPropertyFlags vkFlags);

/** Returns the Metal CPU cache mode corresponding to the specified Vulkan memory flags. */
MTLCPUCacheMode mvkMTLCPUCacheModeFromVkMemoryPropertyFlags(VkMemoryPropertyFlags vkFlags);

/** Returns the Metal resource option flags corresponding to the specified Vulkan memory flags. */
MTLResourceOptions mvkMTLResourceOptionsFromVkMemoryPropertyFlags(VkMemoryPropertyFlags vkFlags);


#pragma mark -
#pragma mark Shaders

/** 
 * Enumerates the magic number values to set in the MVKMSLSPIRVHeader when 
 * submitting a SPIR-V stream that contains either Metal Shading Language source 
 * code or Metal Shading Language compiled binary code in place of SPIR-V code.
 */
typedef enum {
	kMVKMagicNumberMSLSourceCode	= 0x19960412,	/**< SPIR-V stream contains Metal Shading Language source code. */
	kMVKMagicNumberMSLCompiledCode	= 0x19981215,	/**< SPIR-V stream contains Metal Shading Language compiled binary code. */
} MVKMSLMagicNumber;

/** 
 * Describes the header at the start of an SPIR-V stream, when it contains either
 * Metal Shading Language source code or Metal Shading Language compiled binary code.
 *
 * To submit MSL source code to the vkCreateShaderModule() function in place of SPIR-V
 * code, prepend a MVKMSLSPIRVHeader containing the kMVKMagicNumberMSLSourceCode magic
 * number to the MSL source code. The MSL source code must be null-terminated.
 *
 * To submit MSL compiled binary code to the vkCreateShaderModule() function in place of
 * SPIR-V code, prepend a MVKMSLSPIRVHeader containing the kMVKMagicNumberMSLCompiledCode
 * magic number to the MSL compiled binary code.
 *
 * In both cases, the pCode element of VkShaderModuleCreateInfo should pointer to the
 * location of the MVKMSLSPIRVHeader, and the MSL code should start at the byte immediately
 * after the MVKMSLSPIRVHeader.
 *
 * The codeSize element of VkShaderModuleCreateInfo should be set to the entire size of
 * the submitted code memory, including the additional sizeof(MVKMSLSPIRVHeader) bytes 
 * taken up by the MVKMSLSPIRVHeader, and, in the case of MSL source code, including 
 * the null-terminator byte.
 */
typedef uint32_t MVKMSLSPIRVHeader;


#ifdef __cplusplus
}
#endif	//  __cplusplus

#endif
