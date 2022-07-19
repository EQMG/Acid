/**
 * volk
 *
 * Copyright (C) 2018-2019, by Arseny Kapoulkine (arseny.kapoulkine@gmail.com)
 * Report bugs and download new versions at https://github.com/zeux/volk
 *
 * This library is distributed under the MIT License. See notice at the end of this file.
 */
/* clang-format off */
#ifndef VOLK_H_
#define VOLK_H_

#if defined(VULKAN_H_) && !defined(VK_NO_PROTOTYPES)
#	error To use volk, you need to define VK_NO_PROTOTYPES before including vulkan.h
#endif

/* VOLK_GENERATE_VERSION_DEFINE */
#define VOLK_HEADER_VERSION 211
/* VOLK_GENERATE_VERSION_DEFINE */

#ifndef VK_NO_PROTOTYPES
#	define VK_NO_PROTOTYPES
#endif

#ifndef VULKAN_H_
#       ifdef VOLK_VULKAN_H_PATH
#               include VOLK_VULKAN_H_PATH
#	elif defined(VK_USE_PLATFORM_WIN32_KHR)
#		include <vulkan/vk_platform.h>
#		include <vulkan/vulkan_core.h>

		/* When VK_USE_PLATFORM_WIN32_KHR is defined, instead of including vulkan.h directly, we include individual parts of the SDK
		 * This is necessary to avoid including <windows.h> which is very heavy - it takes 200ms to parse without WIN32_LEAN_AND_MEAN
		 * and 100ms to parse with it. vulkan_win32.h only needs a few symbols that are easy to redefine ourselves.
		 */
		typedef unsigned long DWORD;
		typedef const wchar_t* LPCWSTR;
		typedef void* HANDLE;
		typedef struct HINSTANCE__* HINSTANCE;
		typedef struct HWND__* HWND;
		typedef struct HMONITOR__* HMONITOR;
		typedef struct _SECURITY_ATTRIBUTES SECURITY_ATTRIBUTES;

#		include <vulkan/vulkan_win32.h>

#		ifdef VK_ENABLE_BETA_EXTENSIONS
#			include <vulkan/vulkan_beta.h>
#		endif
#	else
#		include <vulkan/vulkan.h>
#	endif
#endif

#include "ContextExport.hpp"

/* Disable several extensions on earlier SDKs because later SDKs introduce a backwards incompatible change to function signatures */
#if VK_HEADER_VERSION < 140
#	undef VK_NVX_image_view_handle
#endif
#if VK_HEADER_VERSION < 184
#	undef VK_HUAWEI_subpass_shading
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct VolkDeviceTable;

/**
 * Initialize library by loading Vulkan loader; call this function before creating the Vulkan instance.
 *
 * Returns VK_SUCCESS on success and VK_ERROR_INITIALIZATION_FAILED otherwise.
 */
ACID_CONTEXT_EXPORT VkResult volkInitialize(void);

/**
 * Initialize library by providing a custom handler to load global symbols.
 *
 * This function can be used instead of volkInitialize.
 * The handler function pointer will be asked to load global Vulkan symbols which require no instance
 * (such as vkCreateInstance, vkEnumerateInstance* and vkEnumerateInstanceVersion if available).
 */
ACID_CONTEXT_EXPORT void volkInitializeCustom(PFN_vkGetInstanceProcAddr handler);

/**
 * Get Vulkan instance version supported by the Vulkan loader, or 0 if Vulkan isn't supported
 *
 * Returns 0 if volkInitialize wasn't called or failed.
 */
ACID_CONTEXT_EXPORT uint32_t volkGetInstanceVersion(void);

/**
 * Load global function pointers using application-created VkInstance; call this function after creating the Vulkan instance.
 */
ACID_CONTEXT_EXPORT void volkLoadInstance(VkInstance instance);

/**
 * Load global function pointers using application-created VkInstance; call this function after creating the Vulkan instance.
 * Skips loading device-based function pointers, requires usage of volkLoadDevice afterwards.
 */
ACID_CONTEXT_EXPORT void volkLoadInstanceOnly(VkInstance instance);

/**
 * Load global function pointers using application-created VkDevice; call this function after creating the Vulkan device.
 *
 * Note: this is not suitable for applications that want to use multiple VkDevice objects concurrently.
 */
ACID_CONTEXT_EXPORT void volkLoadDevice(VkDevice device);

/**
 * Return last VkInstance for which global function pointers have been loaded via volkLoadInstance(),
 * or VK_NULL_HANDLE if volkLoadInstance() has not been called.
 */
ACID_CONTEXT_EXPORT VkInstance volkGetLoadedInstance(void);

/**
 * Return last VkDevice for which global function pointers have been loaded via volkLoadDevice(),
 * or VK_NULL_HANDLE if volkLoadDevice() has not been called.
 */
ACID_CONTEXT_EXPORT VkDevice volkGetLoadedDevice(void);

/**
 * Load function pointers using application-created VkDevice into a table.
 * Application should use function pointers from that table instead of using global function pointers.
 */
ACID_CONTEXT_EXPORT void volkLoadDeviceTable(struct VolkDeviceTable* table, VkDevice device);

/**
 * Device-specific function pointer table
 */
struct VolkDeviceTable
{
	/* VOLK_GENERATE_DEVICE_TABLE */
#if defined(VK_VERSION_1_0)
	PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
	PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets;
	PFN_vkAllocateMemory vkAllocateMemory;
	PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
	PFN_vkBindBufferMemory vkBindBufferMemory;
	PFN_vkBindImageMemory vkBindImageMemory;
	PFN_vkCmdBeginQuery vkCmdBeginQuery;
	PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass;
	PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets;
	PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer;
	PFN_vkCmdBindPipeline vkCmdBindPipeline;
	PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers;
	PFN_vkCmdBlitImage vkCmdBlitImage;
	PFN_vkCmdClearAttachments vkCmdClearAttachments;
	PFN_vkCmdClearColorImage vkCmdClearColorImage;
	PFN_vkCmdClearDepthStencilImage vkCmdClearDepthStencilImage;
	PFN_vkCmdCopyBuffer vkCmdCopyBuffer;
	PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage;
	PFN_vkCmdCopyImage vkCmdCopyImage;
	PFN_vkCmdCopyImageToBuffer vkCmdCopyImageToBuffer;
	PFN_vkCmdCopyQueryPoolResults vkCmdCopyQueryPoolResults;
	PFN_vkCmdDispatch vkCmdDispatch;
	PFN_vkCmdDispatchIndirect vkCmdDispatchIndirect;
	PFN_vkCmdDraw vkCmdDraw;
	PFN_vkCmdDrawIndexed vkCmdDrawIndexed;
	PFN_vkCmdDrawIndexedIndirect vkCmdDrawIndexedIndirect;
	PFN_vkCmdDrawIndirect vkCmdDrawIndirect;
	PFN_vkCmdEndQuery vkCmdEndQuery;
	PFN_vkCmdEndRenderPass vkCmdEndRenderPass;
	PFN_vkCmdExecuteCommands vkCmdExecuteCommands;
	PFN_vkCmdFillBuffer vkCmdFillBuffer;
	PFN_vkCmdNextSubpass vkCmdNextSubpass;
	PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier;
	PFN_vkCmdPushConstants vkCmdPushConstants;
	PFN_vkCmdResetEvent vkCmdResetEvent;
	PFN_vkCmdResetQueryPool vkCmdResetQueryPool;
	PFN_vkCmdResolveImage vkCmdResolveImage;
	PFN_vkCmdSetBlendConstants vkCmdSetBlendConstants;
	PFN_vkCmdSetDepthBias vkCmdSetDepthBias;
	PFN_vkCmdSetDepthBounds vkCmdSetDepthBounds;
	PFN_vkCmdSetEvent vkCmdSetEvent;
	PFN_vkCmdSetLineWidth vkCmdSetLineWidth;
	PFN_vkCmdSetScissor vkCmdSetScissor;
	PFN_vkCmdSetStencilCompareMask vkCmdSetStencilCompareMask;
	PFN_vkCmdSetStencilReference vkCmdSetStencilReference;
	PFN_vkCmdSetStencilWriteMask vkCmdSetStencilWriteMask;
	PFN_vkCmdSetViewport vkCmdSetViewport;
	PFN_vkCmdUpdateBuffer vkCmdUpdateBuffer;
	PFN_vkCmdWaitEvents vkCmdWaitEvents;
	PFN_vkCmdWriteTimestamp vkCmdWriteTimestamp;
	PFN_vkCreateBuffer vkCreateBuffer;
	PFN_vkCreateBufferView vkCreateBufferView;
	PFN_vkCreateCommandPool vkCreateCommandPool;
	PFN_vkCreateComputePipelines vkCreateComputePipelines;
	PFN_vkCreateDescriptorPool vkCreateDescriptorPool;
	PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout;
	PFN_vkCreateEvent vkCreateEvent;
	PFN_vkCreateFence vkCreateFence;
	PFN_vkCreateFramebuffer vkCreateFramebuffer;
	PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
	PFN_vkCreateImage vkCreateImage;
	PFN_vkCreateImageView vkCreateImageView;
	PFN_vkCreatePipelineCache vkCreatePipelineCache;
	PFN_vkCreatePipelineLayout vkCreatePipelineLayout;
	PFN_vkCreateQueryPool vkCreateQueryPool;
	PFN_vkCreateRenderPass vkCreateRenderPass;
	PFN_vkCreateSampler vkCreateSampler;
	PFN_vkCreateSemaphore vkCreateSemaphore;
	PFN_vkCreateShaderModule vkCreateShaderModule;
	PFN_vkDestroyBuffer vkDestroyBuffer;
	PFN_vkDestroyBufferView vkDestroyBufferView;
	PFN_vkDestroyCommandPool vkDestroyCommandPool;
	PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool;
	PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout;
	PFN_vkDestroyDevice vkDestroyDevice;
	PFN_vkDestroyEvent vkDestroyEvent;
	PFN_vkDestroyFence vkDestroyFence;
	PFN_vkDestroyFramebuffer vkDestroyFramebuffer;
	PFN_vkDestroyImage vkDestroyImage;
	PFN_vkDestroyImageView vkDestroyImageView;
	PFN_vkDestroyPipeline vkDestroyPipeline;
	PFN_vkDestroyPipelineCache vkDestroyPipelineCache;
	PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout;
	PFN_vkDestroyQueryPool vkDestroyQueryPool;
	PFN_vkDestroyRenderPass vkDestroyRenderPass;
	PFN_vkDestroySampler vkDestroySampler;
	PFN_vkDestroySemaphore vkDestroySemaphore;
	PFN_vkDestroyShaderModule vkDestroyShaderModule;
	PFN_vkDeviceWaitIdle vkDeviceWaitIdle;
	PFN_vkEndCommandBuffer vkEndCommandBuffer;
	PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges;
	PFN_vkFreeCommandBuffers vkFreeCommandBuffers;
	PFN_vkFreeDescriptorSets vkFreeDescriptorSets;
	PFN_vkFreeMemory vkFreeMemory;
	PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements;
	PFN_vkGetDeviceMemoryCommitment vkGetDeviceMemoryCommitment;
	PFN_vkGetDeviceQueue vkGetDeviceQueue;
	PFN_vkGetEventStatus vkGetEventStatus;
	PFN_vkGetFenceStatus vkGetFenceStatus;
	PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements;
	PFN_vkGetImageSparseMemoryRequirements vkGetImageSparseMemoryRequirements;
	PFN_vkGetImageSubresourceLayout vkGetImageSubresourceLayout;
	PFN_vkGetPipelineCacheData vkGetPipelineCacheData;
	PFN_vkGetQueryPoolResults vkGetQueryPoolResults;
	PFN_vkGetRenderAreaGranularity vkGetRenderAreaGranularity;
	PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges;
	PFN_vkMapMemory vkMapMemory;
	PFN_vkMergePipelineCaches vkMergePipelineCaches;
	PFN_vkQueueBindSparse vkQueueBindSparse;
	PFN_vkQueueSubmit vkQueueSubmit;
	PFN_vkQueueWaitIdle vkQueueWaitIdle;
	PFN_vkResetCommandBuffer vkResetCommandBuffer;
	PFN_vkResetCommandPool vkResetCommandPool;
	PFN_vkResetDescriptorPool vkResetDescriptorPool;
	PFN_vkResetEvent vkResetEvent;
	PFN_vkResetFences vkResetFences;
	PFN_vkSetEvent vkSetEvent;
	PFN_vkUnmapMemory vkUnmapMemory;
	PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets;
	PFN_vkWaitForFences vkWaitForFences;
#endif /* defined(VK_VERSION_1_0) */
#if defined(VK_VERSION_1_1)
	PFN_vkBindBufferMemory2 vkBindBufferMemory2;
	PFN_vkBindImageMemory2 vkBindImageMemory2;
	PFN_vkCmdDispatchBase vkCmdDispatchBase;
	PFN_vkCmdSetDeviceMask vkCmdSetDeviceMask;
	PFN_vkCreateDescriptorUpdateTemplate vkCreateDescriptorUpdateTemplate;
	PFN_vkCreateSamplerYcbcrConversion vkCreateSamplerYcbcrConversion;
	PFN_vkDestroyDescriptorUpdateTemplate vkDestroyDescriptorUpdateTemplate;
	PFN_vkDestroySamplerYcbcrConversion vkDestroySamplerYcbcrConversion;
	PFN_vkGetBufferMemoryRequirements2 vkGetBufferMemoryRequirements2;
	PFN_vkGetDescriptorSetLayoutSupport vkGetDescriptorSetLayoutSupport;
	PFN_vkGetDeviceGroupPeerMemoryFeatures vkGetDeviceGroupPeerMemoryFeatures;
	PFN_vkGetDeviceQueue2 vkGetDeviceQueue2;
	PFN_vkGetImageMemoryRequirements2 vkGetImageMemoryRequirements2;
	PFN_vkGetImageSparseMemoryRequirements2 vkGetImageSparseMemoryRequirements2;
	PFN_vkTrimCommandPool vkTrimCommandPool;
	PFN_vkUpdateDescriptorSetWithTemplate vkUpdateDescriptorSetWithTemplate;
#endif /* defined(VK_VERSION_1_1) */
#if defined(VK_VERSION_1_2)
	PFN_vkCmdBeginRenderPass2 vkCmdBeginRenderPass2;
	PFN_vkCmdDrawIndexedIndirectCount vkCmdDrawIndexedIndirectCount;
	PFN_vkCmdDrawIndirectCount vkCmdDrawIndirectCount;
	PFN_vkCmdEndRenderPass2 vkCmdEndRenderPass2;
	PFN_vkCmdNextSubpass2 vkCmdNextSubpass2;
	PFN_vkCreateRenderPass2 vkCreateRenderPass2;
	PFN_vkGetBufferDeviceAddress vkGetBufferDeviceAddress;
	PFN_vkGetBufferOpaqueCaptureAddress vkGetBufferOpaqueCaptureAddress;
	PFN_vkGetDeviceMemoryOpaqueCaptureAddress vkGetDeviceMemoryOpaqueCaptureAddress;
	PFN_vkGetSemaphoreCounterValue vkGetSemaphoreCounterValue;
	PFN_vkResetQueryPool vkResetQueryPool;
	PFN_vkSignalSemaphore vkSignalSemaphore;
	PFN_vkWaitSemaphores vkWaitSemaphores;
#endif /* defined(VK_VERSION_1_2) */
#if defined(VK_VERSION_1_3)
	PFN_vkCmdBeginRendering vkCmdBeginRendering;
	PFN_vkCmdBindVertexBuffers2 vkCmdBindVertexBuffers2;
	PFN_vkCmdBlitImage2 vkCmdBlitImage2;
	PFN_vkCmdCopyBuffer2 vkCmdCopyBuffer2;
	PFN_vkCmdCopyBufferToImage2 vkCmdCopyBufferToImage2;
	PFN_vkCmdCopyImage2 vkCmdCopyImage2;
	PFN_vkCmdCopyImageToBuffer2 vkCmdCopyImageToBuffer2;
	PFN_vkCmdEndRendering vkCmdEndRendering;
	PFN_vkCmdPipelineBarrier2 vkCmdPipelineBarrier2;
	PFN_vkCmdResetEvent2 vkCmdResetEvent2;
	PFN_vkCmdResolveImage2 vkCmdResolveImage2;
	PFN_vkCmdSetCullMode vkCmdSetCullMode;
	PFN_vkCmdSetDepthBiasEnable vkCmdSetDepthBiasEnable;
	PFN_vkCmdSetDepthBoundsTestEnable vkCmdSetDepthBoundsTestEnable;
	PFN_vkCmdSetDepthCompareOp vkCmdSetDepthCompareOp;
	PFN_vkCmdSetDepthTestEnable vkCmdSetDepthTestEnable;
	PFN_vkCmdSetDepthWriteEnable vkCmdSetDepthWriteEnable;
	PFN_vkCmdSetEvent2 vkCmdSetEvent2;
	PFN_vkCmdSetFrontFace vkCmdSetFrontFace;
	PFN_vkCmdSetPrimitiveRestartEnable vkCmdSetPrimitiveRestartEnable;
	PFN_vkCmdSetPrimitiveTopology vkCmdSetPrimitiveTopology;
	PFN_vkCmdSetRasterizerDiscardEnable vkCmdSetRasterizerDiscardEnable;
	PFN_vkCmdSetScissorWithCount vkCmdSetScissorWithCount;
	PFN_vkCmdSetStencilOp vkCmdSetStencilOp;
	PFN_vkCmdSetStencilTestEnable vkCmdSetStencilTestEnable;
	PFN_vkCmdSetViewportWithCount vkCmdSetViewportWithCount;
	PFN_vkCmdWaitEvents2 vkCmdWaitEvents2;
	PFN_vkCmdWriteTimestamp2 vkCmdWriteTimestamp2;
	PFN_vkCreatePrivateDataSlot vkCreatePrivateDataSlot;
	PFN_vkDestroyPrivateDataSlot vkDestroyPrivateDataSlot;
	PFN_vkGetDeviceBufferMemoryRequirements vkGetDeviceBufferMemoryRequirements;
	PFN_vkGetDeviceImageMemoryRequirements vkGetDeviceImageMemoryRequirements;
	PFN_vkGetDeviceImageSparseMemoryRequirements vkGetDeviceImageSparseMemoryRequirements;
	PFN_vkGetPrivateData vkGetPrivateData;
	PFN_vkQueueSubmit2 vkQueueSubmit2;
	PFN_vkSetPrivateData vkSetPrivateData;
#endif /* defined(VK_VERSION_1_3) */
#if defined(VK_AMD_buffer_marker)
	PFN_vkCmdWriteBufferMarkerAMD vkCmdWriteBufferMarkerAMD;
#endif /* defined(VK_AMD_buffer_marker) */
#if defined(VK_AMD_display_native_hdr)
	PFN_vkSetLocalDimmingAMD vkSetLocalDimmingAMD;
#endif /* defined(VK_AMD_display_native_hdr) */
#if defined(VK_AMD_draw_indirect_count)
	PFN_vkCmdDrawIndexedIndirectCountAMD vkCmdDrawIndexedIndirectCountAMD;
	PFN_vkCmdDrawIndirectCountAMD vkCmdDrawIndirectCountAMD;
#endif /* defined(VK_AMD_draw_indirect_count) */
#if defined(VK_AMD_shader_info)
	PFN_vkGetShaderInfoAMD vkGetShaderInfoAMD;
#endif /* defined(VK_AMD_shader_info) */
#if defined(VK_ANDROID_external_memory_android_hardware_buffer)
	PFN_vkGetAndroidHardwareBufferPropertiesANDROID vkGetAndroidHardwareBufferPropertiesANDROID;
	PFN_vkGetMemoryAndroidHardwareBufferANDROID vkGetMemoryAndroidHardwareBufferANDROID;
#endif /* defined(VK_ANDROID_external_memory_android_hardware_buffer) */
#if defined(VK_EXT_buffer_device_address)
	PFN_vkGetBufferDeviceAddressEXT vkGetBufferDeviceAddressEXT;
#endif /* defined(VK_EXT_buffer_device_address) */
#if defined(VK_EXT_calibrated_timestamps)
	PFN_vkGetCalibratedTimestampsEXT vkGetCalibratedTimestampsEXT;
#endif /* defined(VK_EXT_calibrated_timestamps) */
#if defined(VK_EXT_color_write_enable)
	PFN_vkCmdSetColorWriteEnableEXT vkCmdSetColorWriteEnableEXT;
#endif /* defined(VK_EXT_color_write_enable) */
#if defined(VK_EXT_conditional_rendering)
	PFN_vkCmdBeginConditionalRenderingEXT vkCmdBeginConditionalRenderingEXT;
	PFN_vkCmdEndConditionalRenderingEXT vkCmdEndConditionalRenderingEXT;
#endif /* defined(VK_EXT_conditional_rendering) */
#if defined(VK_EXT_debug_marker)
	PFN_vkCmdDebugMarkerBeginEXT vkCmdDebugMarkerBeginEXT;
	PFN_vkCmdDebugMarkerEndEXT vkCmdDebugMarkerEndEXT;
	PFN_vkCmdDebugMarkerInsertEXT vkCmdDebugMarkerInsertEXT;
	PFN_vkDebugMarkerSetObjectNameEXT vkDebugMarkerSetObjectNameEXT;
	PFN_vkDebugMarkerSetObjectTagEXT vkDebugMarkerSetObjectTagEXT;
#endif /* defined(VK_EXT_debug_marker) */
#if defined(VK_EXT_discard_rectangles)
	PFN_vkCmdSetDiscardRectangleEXT vkCmdSetDiscardRectangleEXT;
#endif /* defined(VK_EXT_discard_rectangles) */
#if defined(VK_EXT_display_control)
	PFN_vkDisplayPowerControlEXT vkDisplayPowerControlEXT;
	PFN_vkGetSwapchainCounterEXT vkGetSwapchainCounterEXT;
	PFN_vkRegisterDeviceEventEXT vkRegisterDeviceEventEXT;
	PFN_vkRegisterDisplayEventEXT vkRegisterDisplayEventEXT;
#endif /* defined(VK_EXT_display_control) */
#if defined(VK_EXT_extended_dynamic_state)
	PFN_vkCmdBindVertexBuffers2EXT vkCmdBindVertexBuffers2EXT;
	PFN_vkCmdSetCullModeEXT vkCmdSetCullModeEXT;
	PFN_vkCmdSetDepthBoundsTestEnableEXT vkCmdSetDepthBoundsTestEnableEXT;
	PFN_vkCmdSetDepthCompareOpEXT vkCmdSetDepthCompareOpEXT;
	PFN_vkCmdSetDepthTestEnableEXT vkCmdSetDepthTestEnableEXT;
	PFN_vkCmdSetDepthWriteEnableEXT vkCmdSetDepthWriteEnableEXT;
	PFN_vkCmdSetFrontFaceEXT vkCmdSetFrontFaceEXT;
	PFN_vkCmdSetPrimitiveTopologyEXT vkCmdSetPrimitiveTopologyEXT;
	PFN_vkCmdSetScissorWithCountEXT vkCmdSetScissorWithCountEXT;
	PFN_vkCmdSetStencilOpEXT vkCmdSetStencilOpEXT;
	PFN_vkCmdSetStencilTestEnableEXT vkCmdSetStencilTestEnableEXT;
	PFN_vkCmdSetViewportWithCountEXT vkCmdSetViewportWithCountEXT;
#endif /* defined(VK_EXT_extended_dynamic_state) */
#if defined(VK_EXT_extended_dynamic_state2)
	PFN_vkCmdSetDepthBiasEnableEXT vkCmdSetDepthBiasEnableEXT;
	PFN_vkCmdSetLogicOpEXT vkCmdSetLogicOpEXT;
	PFN_vkCmdSetPatchControlPointsEXT vkCmdSetPatchControlPointsEXT;
	PFN_vkCmdSetPrimitiveRestartEnableEXT vkCmdSetPrimitiveRestartEnableEXT;
	PFN_vkCmdSetRasterizerDiscardEnableEXT vkCmdSetRasterizerDiscardEnableEXT;
#endif /* defined(VK_EXT_extended_dynamic_state2) */
#if defined(VK_EXT_external_memory_host)
	PFN_vkGetMemoryHostPointerPropertiesEXT vkGetMemoryHostPointerPropertiesEXT;
#endif /* defined(VK_EXT_external_memory_host) */
#if defined(VK_EXT_full_screen_exclusive)
	PFN_vkAcquireFullScreenExclusiveModeEXT vkAcquireFullScreenExclusiveModeEXT;
	PFN_vkReleaseFullScreenExclusiveModeEXT vkReleaseFullScreenExclusiveModeEXT;
#endif /* defined(VK_EXT_full_screen_exclusive) */
#if defined(VK_EXT_hdr_metadata)
	PFN_vkSetHdrMetadataEXT vkSetHdrMetadataEXT;
#endif /* defined(VK_EXT_hdr_metadata) */
#if defined(VK_EXT_host_query_reset)
	PFN_vkResetQueryPoolEXT vkResetQueryPoolEXT;
#endif /* defined(VK_EXT_host_query_reset) */
#if defined(VK_EXT_image_drm_format_modifier)
	PFN_vkGetImageDrmFormatModifierPropertiesEXT vkGetImageDrmFormatModifierPropertiesEXT;
#endif /* defined(VK_EXT_image_drm_format_modifier) */
#if defined(VK_EXT_line_rasterization)
	PFN_vkCmdSetLineStippleEXT vkCmdSetLineStippleEXT;
#endif /* defined(VK_EXT_line_rasterization) */
#if defined(VK_EXT_multi_draw)
	PFN_vkCmdDrawMultiEXT vkCmdDrawMultiEXT;
	PFN_vkCmdDrawMultiIndexedEXT vkCmdDrawMultiIndexedEXT;
#endif /* defined(VK_EXT_multi_draw) */
#if defined(VK_EXT_pageable_device_local_memory)
	PFN_vkSetDeviceMemoryPriorityEXT vkSetDeviceMemoryPriorityEXT;
#endif /* defined(VK_EXT_pageable_device_local_memory) */
#if defined(VK_EXT_private_data)
	PFN_vkCreatePrivateDataSlotEXT vkCreatePrivateDataSlotEXT;
	PFN_vkDestroyPrivateDataSlotEXT vkDestroyPrivateDataSlotEXT;
	PFN_vkGetPrivateDataEXT vkGetPrivateDataEXT;
	PFN_vkSetPrivateDataEXT vkSetPrivateDataEXT;
#endif /* defined(VK_EXT_private_data) */
#if defined(VK_EXT_sample_locations)
	PFN_vkCmdSetSampleLocationsEXT vkCmdSetSampleLocationsEXT;
#endif /* defined(VK_EXT_sample_locations) */
#if defined(VK_EXT_transform_feedback)
	PFN_vkCmdBeginQueryIndexedEXT vkCmdBeginQueryIndexedEXT;
	PFN_vkCmdBeginTransformFeedbackEXT vkCmdBeginTransformFeedbackEXT;
	PFN_vkCmdBindTransformFeedbackBuffersEXT vkCmdBindTransformFeedbackBuffersEXT;
	PFN_vkCmdDrawIndirectByteCountEXT vkCmdDrawIndirectByteCountEXT;
	PFN_vkCmdEndQueryIndexedEXT vkCmdEndQueryIndexedEXT;
	PFN_vkCmdEndTransformFeedbackEXT vkCmdEndTransformFeedbackEXT;
#endif /* defined(VK_EXT_transform_feedback) */
#if defined(VK_EXT_validation_cache)
	PFN_vkCreateValidationCacheEXT vkCreateValidationCacheEXT;
	PFN_vkDestroyValidationCacheEXT vkDestroyValidationCacheEXT;
	PFN_vkGetValidationCacheDataEXT vkGetValidationCacheDataEXT;
	PFN_vkMergeValidationCachesEXT vkMergeValidationCachesEXT;
#endif /* defined(VK_EXT_validation_cache) */
#if defined(VK_EXT_vertex_input_dynamic_state)
	PFN_vkCmdSetVertexInputEXT vkCmdSetVertexInputEXT;
#endif /* defined(VK_EXT_vertex_input_dynamic_state) */
#if defined(VK_FUCHSIA_buffer_collection)
	PFN_vkCreateBufferCollectionFUCHSIA vkCreateBufferCollectionFUCHSIA;
	PFN_vkDestroyBufferCollectionFUCHSIA vkDestroyBufferCollectionFUCHSIA;
	PFN_vkGetBufferCollectionPropertiesFUCHSIA vkGetBufferCollectionPropertiesFUCHSIA;
	PFN_vkSetBufferCollectionBufferConstraintsFUCHSIA vkSetBufferCollectionBufferConstraintsFUCHSIA;
	PFN_vkSetBufferCollectionImageConstraintsFUCHSIA vkSetBufferCollectionImageConstraintsFUCHSIA;
#endif /* defined(VK_FUCHSIA_buffer_collection) */
#if defined(VK_FUCHSIA_external_memory)
	PFN_vkGetMemoryZirconHandleFUCHSIA vkGetMemoryZirconHandleFUCHSIA;
	PFN_vkGetMemoryZirconHandlePropertiesFUCHSIA vkGetMemoryZirconHandlePropertiesFUCHSIA;
#endif /* defined(VK_FUCHSIA_external_memory) */
#if defined(VK_FUCHSIA_external_semaphore)
	PFN_vkGetSemaphoreZirconHandleFUCHSIA vkGetSemaphoreZirconHandleFUCHSIA;
	PFN_vkImportSemaphoreZirconHandleFUCHSIA vkImportSemaphoreZirconHandleFUCHSIA;
#endif /* defined(VK_FUCHSIA_external_semaphore) */
#if defined(VK_GOOGLE_display_timing)
	PFN_vkGetPastPresentationTimingGOOGLE vkGetPastPresentationTimingGOOGLE;
	PFN_vkGetRefreshCycleDurationGOOGLE vkGetRefreshCycleDurationGOOGLE;
#endif /* defined(VK_GOOGLE_display_timing) */
#if defined(VK_HUAWEI_invocation_mask)
	PFN_vkCmdBindInvocationMaskHUAWEI vkCmdBindInvocationMaskHUAWEI;
#endif /* defined(VK_HUAWEI_invocation_mask) */
#if defined(VK_HUAWEI_subpass_shading)
	PFN_vkCmdSubpassShadingHUAWEI vkCmdSubpassShadingHUAWEI;
	PFN_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI;
#endif /* defined(VK_HUAWEI_subpass_shading) */
#if defined(VK_INTEL_performance_query)
	PFN_vkAcquirePerformanceConfigurationINTEL vkAcquirePerformanceConfigurationINTEL;
	PFN_vkCmdSetPerformanceMarkerINTEL vkCmdSetPerformanceMarkerINTEL;
	PFN_vkCmdSetPerformanceOverrideINTEL vkCmdSetPerformanceOverrideINTEL;
	PFN_vkCmdSetPerformanceStreamMarkerINTEL vkCmdSetPerformanceStreamMarkerINTEL;
	PFN_vkGetPerformanceParameterINTEL vkGetPerformanceParameterINTEL;
	PFN_vkInitializePerformanceApiINTEL vkInitializePerformanceApiINTEL;
	PFN_vkQueueSetPerformanceConfigurationINTEL vkQueueSetPerformanceConfigurationINTEL;
	PFN_vkReleasePerformanceConfigurationINTEL vkReleasePerformanceConfigurationINTEL;
	PFN_vkUninitializePerformanceApiINTEL vkUninitializePerformanceApiINTEL;
#endif /* defined(VK_INTEL_performance_query) */
#if defined(VK_KHR_acceleration_structure)
	PFN_vkBuildAccelerationStructuresKHR vkBuildAccelerationStructuresKHR;
	PFN_vkCmdBuildAccelerationStructuresIndirectKHR vkCmdBuildAccelerationStructuresIndirectKHR;
	PFN_vkCmdBuildAccelerationStructuresKHR vkCmdBuildAccelerationStructuresKHR;
	PFN_vkCmdCopyAccelerationStructureKHR vkCmdCopyAccelerationStructureKHR;
	PFN_vkCmdCopyAccelerationStructureToMemoryKHR vkCmdCopyAccelerationStructureToMemoryKHR;
	PFN_vkCmdCopyMemoryToAccelerationStructureKHR vkCmdCopyMemoryToAccelerationStructureKHR;
	PFN_vkCmdWriteAccelerationStructuresPropertiesKHR vkCmdWriteAccelerationStructuresPropertiesKHR;
	PFN_vkCopyAccelerationStructureKHR vkCopyAccelerationStructureKHR;
	PFN_vkCopyAccelerationStructureToMemoryKHR vkCopyAccelerationStructureToMemoryKHR;
	PFN_vkCopyMemoryToAccelerationStructureKHR vkCopyMemoryToAccelerationStructureKHR;
	PFN_vkCreateAccelerationStructureKHR vkCreateAccelerationStructureKHR;
	PFN_vkDestroyAccelerationStructureKHR vkDestroyAccelerationStructureKHR;
	PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizesKHR;
	PFN_vkGetAccelerationStructureDeviceAddressKHR vkGetAccelerationStructureDeviceAddressKHR;
	PFN_vkGetDeviceAccelerationStructureCompatibilityKHR vkGetDeviceAccelerationStructureCompatibilityKHR;
	PFN_vkWriteAccelerationStructuresPropertiesKHR vkWriteAccelerationStructuresPropertiesKHR;
#endif /* defined(VK_KHR_acceleration_structure) */
#if defined(VK_KHR_bind_memory2)
	PFN_vkBindBufferMemory2KHR vkBindBufferMemory2KHR;
	PFN_vkBindImageMemory2KHR vkBindImageMemory2KHR;
#endif /* defined(VK_KHR_bind_memory2) */
#if defined(VK_KHR_buffer_device_address)
	PFN_vkGetBufferDeviceAddressKHR vkGetBufferDeviceAddressKHR;
	PFN_vkGetBufferOpaqueCaptureAddressKHR vkGetBufferOpaqueCaptureAddressKHR;
	PFN_vkGetDeviceMemoryOpaqueCaptureAddressKHR vkGetDeviceMemoryOpaqueCaptureAddressKHR;
#endif /* defined(VK_KHR_buffer_device_address) */
#if defined(VK_KHR_copy_commands2)
	PFN_vkCmdBlitImage2KHR vkCmdBlitImage2KHR;
	PFN_vkCmdCopyBuffer2KHR vkCmdCopyBuffer2KHR;
	PFN_vkCmdCopyBufferToImage2KHR vkCmdCopyBufferToImage2KHR;
	PFN_vkCmdCopyImage2KHR vkCmdCopyImage2KHR;
	PFN_vkCmdCopyImageToBuffer2KHR vkCmdCopyImageToBuffer2KHR;
	PFN_vkCmdResolveImage2KHR vkCmdResolveImage2KHR;
#endif /* defined(VK_KHR_copy_commands2) */
#if defined(VK_KHR_create_renderpass2)
	PFN_vkCmdBeginRenderPass2KHR vkCmdBeginRenderPass2KHR;
	PFN_vkCmdEndRenderPass2KHR vkCmdEndRenderPass2KHR;
	PFN_vkCmdNextSubpass2KHR vkCmdNextSubpass2KHR;
	PFN_vkCreateRenderPass2KHR vkCreateRenderPass2KHR;
#endif /* defined(VK_KHR_create_renderpass2) */
#if defined(VK_KHR_deferred_host_operations)
	PFN_vkCreateDeferredOperationKHR vkCreateDeferredOperationKHR;
	PFN_vkDeferredOperationJoinKHR vkDeferredOperationJoinKHR;
	PFN_vkDestroyDeferredOperationKHR vkDestroyDeferredOperationKHR;
	PFN_vkGetDeferredOperationMaxConcurrencyKHR vkGetDeferredOperationMaxConcurrencyKHR;
	PFN_vkGetDeferredOperationResultKHR vkGetDeferredOperationResultKHR;
#endif /* defined(VK_KHR_deferred_host_operations) */
#if defined(VK_KHR_descriptor_update_template)
	PFN_vkCreateDescriptorUpdateTemplateKHR vkCreateDescriptorUpdateTemplateKHR;
	PFN_vkDestroyDescriptorUpdateTemplateKHR vkDestroyDescriptorUpdateTemplateKHR;
	PFN_vkUpdateDescriptorSetWithTemplateKHR vkUpdateDescriptorSetWithTemplateKHR;
#endif /* defined(VK_KHR_descriptor_update_template) */
#if defined(VK_KHR_device_group)
	PFN_vkCmdDispatchBaseKHR vkCmdDispatchBaseKHR;
	PFN_vkCmdSetDeviceMaskKHR vkCmdSetDeviceMaskKHR;
	PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR vkGetDeviceGroupPeerMemoryFeaturesKHR;
#endif /* defined(VK_KHR_device_group) */
#if defined(VK_KHR_display_swapchain)
	PFN_vkCreateSharedSwapchainsKHR vkCreateSharedSwapchainsKHR;
#endif /* defined(VK_KHR_display_swapchain) */
#if defined(VK_KHR_draw_indirect_count)
	PFN_vkCmdDrawIndexedIndirectCountKHR vkCmdDrawIndexedIndirectCountKHR;
	PFN_vkCmdDrawIndirectCountKHR vkCmdDrawIndirectCountKHR;
#endif /* defined(VK_KHR_draw_indirect_count) */
#if defined(VK_KHR_dynamic_rendering)
	PFN_vkCmdBeginRenderingKHR vkCmdBeginRenderingKHR;
	PFN_vkCmdEndRenderingKHR vkCmdEndRenderingKHR;
#endif /* defined(VK_KHR_dynamic_rendering) */
#if defined(VK_KHR_external_fence_fd)
	PFN_vkGetFenceFdKHR vkGetFenceFdKHR;
	PFN_vkImportFenceFdKHR vkImportFenceFdKHR;
#endif /* defined(VK_KHR_external_fence_fd) */
#if defined(VK_KHR_external_fence_win32)
	PFN_vkGetFenceWin32HandleKHR vkGetFenceWin32HandleKHR;
	PFN_vkImportFenceWin32HandleKHR vkImportFenceWin32HandleKHR;
#endif /* defined(VK_KHR_external_fence_win32) */
#if defined(VK_KHR_external_memory_fd)
	PFN_vkGetMemoryFdKHR vkGetMemoryFdKHR;
	PFN_vkGetMemoryFdPropertiesKHR vkGetMemoryFdPropertiesKHR;
#endif /* defined(VK_KHR_external_memory_fd) */
#if defined(VK_KHR_external_memory_win32)
	PFN_vkGetMemoryWin32HandleKHR vkGetMemoryWin32HandleKHR;
	PFN_vkGetMemoryWin32HandlePropertiesKHR vkGetMemoryWin32HandlePropertiesKHR;
#endif /* defined(VK_KHR_external_memory_win32) */
#if defined(VK_KHR_external_semaphore_fd)
	PFN_vkGetSemaphoreFdKHR vkGetSemaphoreFdKHR;
	PFN_vkImportSemaphoreFdKHR vkImportSemaphoreFdKHR;
#endif /* defined(VK_KHR_external_semaphore_fd) */
#if defined(VK_KHR_external_semaphore_win32)
	PFN_vkGetSemaphoreWin32HandleKHR vkGetSemaphoreWin32HandleKHR;
	PFN_vkImportSemaphoreWin32HandleKHR vkImportSemaphoreWin32HandleKHR;
#endif /* defined(VK_KHR_external_semaphore_win32) */
#if defined(VK_KHR_fragment_shading_rate)
	PFN_vkCmdSetFragmentShadingRateKHR vkCmdSetFragmentShadingRateKHR;
#endif /* defined(VK_KHR_fragment_shading_rate) */
#if defined(VK_KHR_get_memory_requirements2)
	PFN_vkGetBufferMemoryRequirements2KHR vkGetBufferMemoryRequirements2KHR;
	PFN_vkGetImageMemoryRequirements2KHR vkGetImageMemoryRequirements2KHR;
	PFN_vkGetImageSparseMemoryRequirements2KHR vkGetImageSparseMemoryRequirements2KHR;
#endif /* defined(VK_KHR_get_memory_requirements2) */
#if defined(VK_KHR_maintenance1)
	PFN_vkTrimCommandPoolKHR vkTrimCommandPoolKHR;
#endif /* defined(VK_KHR_maintenance1) */
#if defined(VK_KHR_maintenance3)
	PFN_vkGetDescriptorSetLayoutSupportKHR vkGetDescriptorSetLayoutSupportKHR;
#endif /* defined(VK_KHR_maintenance3) */
#if defined(VK_KHR_maintenance4)
	PFN_vkGetDeviceBufferMemoryRequirementsKHR vkGetDeviceBufferMemoryRequirementsKHR;
	PFN_vkGetDeviceImageMemoryRequirementsKHR vkGetDeviceImageMemoryRequirementsKHR;
	PFN_vkGetDeviceImageSparseMemoryRequirementsKHR vkGetDeviceImageSparseMemoryRequirementsKHR;
#endif /* defined(VK_KHR_maintenance4) */
#if defined(VK_KHR_performance_query)
	PFN_vkAcquireProfilingLockKHR vkAcquireProfilingLockKHR;
	PFN_vkReleaseProfilingLockKHR vkReleaseProfilingLockKHR;
#endif /* defined(VK_KHR_performance_query) */
#if defined(VK_KHR_pipeline_executable_properties)
	PFN_vkGetPipelineExecutableInternalRepresentationsKHR vkGetPipelineExecutableInternalRepresentationsKHR;
	PFN_vkGetPipelineExecutablePropertiesKHR vkGetPipelineExecutablePropertiesKHR;
	PFN_vkGetPipelineExecutableStatisticsKHR vkGetPipelineExecutableStatisticsKHR;
#endif /* defined(VK_KHR_pipeline_executable_properties) */
#if defined(VK_KHR_present_wait)
	PFN_vkWaitForPresentKHR vkWaitForPresentKHR;
#endif /* defined(VK_KHR_present_wait) */
#if defined(VK_KHR_push_descriptor)
	PFN_vkCmdPushDescriptorSetKHR vkCmdPushDescriptorSetKHR;
#endif /* defined(VK_KHR_push_descriptor) */
#if defined(VK_KHR_ray_tracing_pipeline)
	PFN_vkCmdSetRayTracingPipelineStackSizeKHR vkCmdSetRayTracingPipelineStackSizeKHR;
	PFN_vkCmdTraceRaysIndirectKHR vkCmdTraceRaysIndirectKHR;
	PFN_vkCmdTraceRaysKHR vkCmdTraceRaysKHR;
	PFN_vkCreateRayTracingPipelinesKHR vkCreateRayTracingPipelinesKHR;
	PFN_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR vkGetRayTracingCaptureReplayShaderGroupHandlesKHR;
	PFN_vkGetRayTracingShaderGroupHandlesKHR vkGetRayTracingShaderGroupHandlesKHR;
	PFN_vkGetRayTracingShaderGroupStackSizeKHR vkGetRayTracingShaderGroupStackSizeKHR;
#endif /* defined(VK_KHR_ray_tracing_pipeline) */
#if defined(VK_KHR_sampler_ycbcr_conversion)
	PFN_vkCreateSamplerYcbcrConversionKHR vkCreateSamplerYcbcrConversionKHR;
	PFN_vkDestroySamplerYcbcrConversionKHR vkDestroySamplerYcbcrConversionKHR;
#endif /* defined(VK_KHR_sampler_ycbcr_conversion) */
#if defined(VK_KHR_shared_presentable_image)
	PFN_vkGetSwapchainStatusKHR vkGetSwapchainStatusKHR;
#endif /* defined(VK_KHR_shared_presentable_image) */
#if defined(VK_KHR_swapchain)
	PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR;
	PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
	PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
	PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
	PFN_vkQueuePresentKHR vkQueuePresentKHR;
#endif /* defined(VK_KHR_swapchain) */
#if defined(VK_KHR_synchronization2)
	PFN_vkCmdPipelineBarrier2KHR vkCmdPipelineBarrier2KHR;
	PFN_vkCmdResetEvent2KHR vkCmdResetEvent2KHR;
	PFN_vkCmdSetEvent2KHR vkCmdSetEvent2KHR;
	PFN_vkCmdWaitEvents2KHR vkCmdWaitEvents2KHR;
	PFN_vkCmdWriteTimestamp2KHR vkCmdWriteTimestamp2KHR;
	PFN_vkQueueSubmit2KHR vkQueueSubmit2KHR;
#endif /* defined(VK_KHR_synchronization2) */
#if defined(VK_KHR_synchronization2) && defined(VK_AMD_buffer_marker)
	PFN_vkCmdWriteBufferMarker2AMD vkCmdWriteBufferMarker2AMD;
#endif /* defined(VK_KHR_synchronization2) && defined(VK_AMD_buffer_marker) */
#if defined(VK_KHR_synchronization2) && defined(VK_NV_device_diagnostic_checkpoints)
	PFN_vkGetQueueCheckpointData2NV vkGetQueueCheckpointData2NV;
#endif /* defined(VK_KHR_synchronization2) && defined(VK_NV_device_diagnostic_checkpoints) */
#if defined(VK_KHR_timeline_semaphore)
	PFN_vkGetSemaphoreCounterValueKHR vkGetSemaphoreCounterValueKHR;
	PFN_vkSignalSemaphoreKHR vkSignalSemaphoreKHR;
	PFN_vkWaitSemaphoresKHR vkWaitSemaphoresKHR;
#endif /* defined(VK_KHR_timeline_semaphore) */
#if defined(VK_KHR_video_decode_queue)
	PFN_vkCmdDecodeVideoKHR vkCmdDecodeVideoKHR;
#endif /* defined(VK_KHR_video_decode_queue) */
#if defined(VK_KHR_video_encode_queue)
	PFN_vkCmdEncodeVideoKHR vkCmdEncodeVideoKHR;
#endif /* defined(VK_KHR_video_encode_queue) */
#if defined(VK_KHR_video_queue)
	PFN_vkBindVideoSessionMemoryKHR vkBindVideoSessionMemoryKHR;
	PFN_vkCmdBeginVideoCodingKHR vkCmdBeginVideoCodingKHR;
	PFN_vkCmdControlVideoCodingKHR vkCmdControlVideoCodingKHR;
	PFN_vkCmdEndVideoCodingKHR vkCmdEndVideoCodingKHR;
	PFN_vkCreateVideoSessionKHR vkCreateVideoSessionKHR;
	PFN_vkCreateVideoSessionParametersKHR vkCreateVideoSessionParametersKHR;
	PFN_vkDestroyVideoSessionKHR vkDestroyVideoSessionKHR;
	PFN_vkDestroyVideoSessionParametersKHR vkDestroyVideoSessionParametersKHR;
	PFN_vkGetVideoSessionMemoryRequirementsKHR vkGetVideoSessionMemoryRequirementsKHR;
	PFN_vkUpdateVideoSessionParametersKHR vkUpdateVideoSessionParametersKHR;
#endif /* defined(VK_KHR_video_queue) */
#if defined(VK_NVX_binary_import)
	PFN_vkCmdCuLaunchKernelNVX vkCmdCuLaunchKernelNVX;
	PFN_vkCreateCuFunctionNVX vkCreateCuFunctionNVX;
	PFN_vkCreateCuModuleNVX vkCreateCuModuleNVX;
	PFN_vkDestroyCuFunctionNVX vkDestroyCuFunctionNVX;
	PFN_vkDestroyCuModuleNVX vkDestroyCuModuleNVX;
#endif /* defined(VK_NVX_binary_import) */
#if defined(VK_NVX_image_view_handle)
	PFN_vkGetImageViewAddressNVX vkGetImageViewAddressNVX;
	PFN_vkGetImageViewHandleNVX vkGetImageViewHandleNVX;
#endif /* defined(VK_NVX_image_view_handle) */
#if defined(VK_NV_clip_space_w_scaling)
	PFN_vkCmdSetViewportWScalingNV vkCmdSetViewportWScalingNV;
#endif /* defined(VK_NV_clip_space_w_scaling) */
#if defined(VK_NV_device_diagnostic_checkpoints)
	PFN_vkCmdSetCheckpointNV vkCmdSetCheckpointNV;
	PFN_vkGetQueueCheckpointDataNV vkGetQueueCheckpointDataNV;
#endif /* defined(VK_NV_device_diagnostic_checkpoints) */
#if defined(VK_NV_device_generated_commands)
	PFN_vkCmdBindPipelineShaderGroupNV vkCmdBindPipelineShaderGroupNV;
	PFN_vkCmdExecuteGeneratedCommandsNV vkCmdExecuteGeneratedCommandsNV;
	PFN_vkCmdPreprocessGeneratedCommandsNV vkCmdPreprocessGeneratedCommandsNV;
	PFN_vkCreateIndirectCommandsLayoutNV vkCreateIndirectCommandsLayoutNV;
	PFN_vkDestroyIndirectCommandsLayoutNV vkDestroyIndirectCommandsLayoutNV;
	PFN_vkGetGeneratedCommandsMemoryRequirementsNV vkGetGeneratedCommandsMemoryRequirementsNV;
#endif /* defined(VK_NV_device_generated_commands) */
#if defined(VK_NV_external_memory_rdma)
	PFN_vkGetMemoryRemoteAddressNV vkGetMemoryRemoteAddressNV;
#endif /* defined(VK_NV_external_memory_rdma) */
#if defined(VK_NV_external_memory_win32)
	PFN_vkGetMemoryWin32HandleNV vkGetMemoryWin32HandleNV;
#endif /* defined(VK_NV_external_memory_win32) */
#if defined(VK_NV_fragment_shading_rate_enums)
	PFN_vkCmdSetFragmentShadingRateEnumNV vkCmdSetFragmentShadingRateEnumNV;
#endif /* defined(VK_NV_fragment_shading_rate_enums) */
#if defined(VK_NV_mesh_shader)
	PFN_vkCmdDrawMeshTasksIndirectCountNV vkCmdDrawMeshTasksIndirectCountNV;
	PFN_vkCmdDrawMeshTasksIndirectNV vkCmdDrawMeshTasksIndirectNV;
	PFN_vkCmdDrawMeshTasksNV vkCmdDrawMeshTasksNV;
#endif /* defined(VK_NV_mesh_shader) */
#if defined(VK_NV_ray_tracing)
	PFN_vkBindAccelerationStructureMemoryNV vkBindAccelerationStructureMemoryNV;
	PFN_vkCmdBuildAccelerationStructureNV vkCmdBuildAccelerationStructureNV;
	PFN_vkCmdCopyAccelerationStructureNV vkCmdCopyAccelerationStructureNV;
	PFN_vkCmdTraceRaysNV vkCmdTraceRaysNV;
	PFN_vkCmdWriteAccelerationStructuresPropertiesNV vkCmdWriteAccelerationStructuresPropertiesNV;
	PFN_vkCompileDeferredNV vkCompileDeferredNV;
	PFN_vkCreateAccelerationStructureNV vkCreateAccelerationStructureNV;
	PFN_vkCreateRayTracingPipelinesNV vkCreateRayTracingPipelinesNV;
	PFN_vkDestroyAccelerationStructureNV vkDestroyAccelerationStructureNV;
	PFN_vkGetAccelerationStructureHandleNV vkGetAccelerationStructureHandleNV;
	PFN_vkGetAccelerationStructureMemoryRequirementsNV vkGetAccelerationStructureMemoryRequirementsNV;
	PFN_vkGetRayTracingShaderGroupHandlesNV vkGetRayTracingShaderGroupHandlesNV;
#endif /* defined(VK_NV_ray_tracing) */
#if defined(VK_NV_scissor_exclusive)
	PFN_vkCmdSetExclusiveScissorNV vkCmdSetExclusiveScissorNV;
#endif /* defined(VK_NV_scissor_exclusive) */
#if defined(VK_NV_shading_rate_image)
	PFN_vkCmdBindShadingRateImageNV vkCmdBindShadingRateImageNV;
	PFN_vkCmdSetCoarseSampleOrderNV vkCmdSetCoarseSampleOrderNV;
	PFN_vkCmdSetViewportShadingRatePaletteNV vkCmdSetViewportShadingRatePaletteNV;
#endif /* defined(VK_NV_shading_rate_image) */
#if defined(VK_VALVE_descriptor_set_host_mapping)
	PFN_vkGetDescriptorSetHostMappingVALVE vkGetDescriptorSetHostMappingVALVE;
	PFN_vkGetDescriptorSetLayoutHostMappingInfoVALVE vkGetDescriptorSetLayoutHostMappingInfoVALVE;
#endif /* defined(VK_VALVE_descriptor_set_host_mapping) */
#if (defined(VK_EXT_full_screen_exclusive) && defined(VK_KHR_device_group)) || (defined(VK_EXT_full_screen_exclusive) && defined(VK_VERSION_1_1))
	PFN_vkGetDeviceGroupSurfacePresentModes2EXT vkGetDeviceGroupSurfacePresentModes2EXT;
#endif /* (defined(VK_EXT_full_screen_exclusive) && defined(VK_KHR_device_group)) || (defined(VK_EXT_full_screen_exclusive) && defined(VK_VERSION_1_1)) */
#if (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor)) || (defined(VK_KHR_push_descriptor) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_push_descriptor) && defined(VK_KHR_descriptor_update_template))
	PFN_vkCmdPushDescriptorSetWithTemplateKHR vkCmdPushDescriptorSetWithTemplateKHR;
#endif /* (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor)) || (defined(VK_KHR_push_descriptor) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_push_descriptor) && defined(VK_KHR_descriptor_update_template)) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
	PFN_vkGetDeviceGroupPresentCapabilitiesKHR vkGetDeviceGroupPresentCapabilitiesKHR;
	PFN_vkGetDeviceGroupSurfacePresentModesKHR vkGetDeviceGroupSurfacePresentModesKHR;
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
	PFN_vkAcquireNextImage2KHR vkAcquireNextImage2KHR;
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
	/* VOLK_GENERATE_DEVICE_TABLE */
};

/* VOLK_GENERATE_PROTOTYPES_H */
#if defined(VK_VERSION_1_0)
ACID_CONTEXT_EXPORT extern PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
ACID_CONTEXT_EXPORT extern PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets;
ACID_CONTEXT_EXPORT extern PFN_vkAllocateMemory vkAllocateMemory;
ACID_CONTEXT_EXPORT extern PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
ACID_CONTEXT_EXPORT extern PFN_vkBindBufferMemory vkBindBufferMemory;
ACID_CONTEXT_EXPORT extern PFN_vkBindImageMemory vkBindImageMemory;
ACID_CONTEXT_EXPORT extern PFN_vkCmdBeginQuery vkCmdBeginQuery;
ACID_CONTEXT_EXPORT extern PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass;
ACID_CONTEXT_EXPORT extern PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets;
ACID_CONTEXT_EXPORT extern PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer;
ACID_CONTEXT_EXPORT extern PFN_vkCmdBindPipeline vkCmdBindPipeline;
ACID_CONTEXT_EXPORT extern PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers;
ACID_CONTEXT_EXPORT extern PFN_vkCmdBlitImage vkCmdBlitImage;
ACID_CONTEXT_EXPORT extern PFN_vkCmdClearAttachments vkCmdClearAttachments;
ACID_CONTEXT_EXPORT extern PFN_vkCmdClearColorImage vkCmdClearColorImage;
ACID_CONTEXT_EXPORT extern PFN_vkCmdClearDepthStencilImage vkCmdClearDepthStencilImage;
ACID_CONTEXT_EXPORT extern PFN_vkCmdCopyBuffer vkCmdCopyBuffer;
ACID_CONTEXT_EXPORT extern PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage;
ACID_CONTEXT_EXPORT extern PFN_vkCmdCopyImage vkCmdCopyImage;
ACID_CONTEXT_EXPORT extern PFN_vkCmdCopyImageToBuffer vkCmdCopyImageToBuffer;
ACID_CONTEXT_EXPORT extern PFN_vkCmdCopyQueryPoolResults vkCmdCopyQueryPoolResults;
ACID_CONTEXT_EXPORT extern PFN_vkCmdDispatch vkCmdDispatch;
ACID_CONTEXT_EXPORT extern PFN_vkCmdDispatchIndirect vkCmdDispatchIndirect;
ACID_CONTEXT_EXPORT extern PFN_vkCmdDraw vkCmdDraw;
ACID_CONTEXT_EXPORT extern PFN_vkCmdDrawIndexed vkCmdDrawIndexed;
ACID_CONTEXT_EXPORT extern PFN_vkCmdDrawIndexedIndirect vkCmdDrawIndexedIndirect;
ACID_CONTEXT_EXPORT extern PFN_vkCmdDrawIndirect vkCmdDrawIndirect;
ACID_CONTEXT_EXPORT extern PFN_vkCmdEndQuery vkCmdEndQuery;
ACID_CONTEXT_EXPORT extern PFN_vkCmdEndRenderPass vkCmdEndRenderPass;
ACID_CONTEXT_EXPORT extern PFN_vkCmdExecuteCommands vkCmdExecuteCommands;
ACID_CONTEXT_EXPORT extern PFN_vkCmdFillBuffer vkCmdFillBuffer;
ACID_CONTEXT_EXPORT extern PFN_vkCmdNextSubpass vkCmdNextSubpass;
ACID_CONTEXT_EXPORT extern PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier;
ACID_CONTEXT_EXPORT extern PFN_vkCmdPushConstants vkCmdPushConstants;
ACID_CONTEXT_EXPORT extern PFN_vkCmdResetEvent vkCmdResetEvent;
ACID_CONTEXT_EXPORT extern PFN_vkCmdResetQueryPool vkCmdResetQueryPool;
ACID_CONTEXT_EXPORT extern PFN_vkCmdResolveImage vkCmdResolveImage;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetBlendConstants vkCmdSetBlendConstants;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetDepthBias vkCmdSetDepthBias;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetDepthBounds vkCmdSetDepthBounds;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetEvent vkCmdSetEvent;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetLineWidth vkCmdSetLineWidth;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetScissor vkCmdSetScissor;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetStencilCompareMask vkCmdSetStencilCompareMask;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetStencilReference vkCmdSetStencilReference;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetStencilWriteMask vkCmdSetStencilWriteMask;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetViewport vkCmdSetViewport;
ACID_CONTEXT_EXPORT extern PFN_vkCmdUpdateBuffer vkCmdUpdateBuffer;
ACID_CONTEXT_EXPORT extern PFN_vkCmdWaitEvents vkCmdWaitEvents;
ACID_CONTEXT_EXPORT extern PFN_vkCmdWriteTimestamp vkCmdWriteTimestamp;
ACID_CONTEXT_EXPORT extern PFN_vkCreateBuffer vkCreateBuffer;
ACID_CONTEXT_EXPORT extern PFN_vkCreateBufferView vkCreateBufferView;
ACID_CONTEXT_EXPORT extern PFN_vkCreateCommandPool vkCreateCommandPool;
ACID_CONTEXT_EXPORT extern PFN_vkCreateComputePipelines vkCreateComputePipelines;
ACID_CONTEXT_EXPORT extern PFN_vkCreateDescriptorPool vkCreateDescriptorPool;
ACID_CONTEXT_EXPORT extern PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout;
ACID_CONTEXT_EXPORT extern PFN_vkCreateDevice vkCreateDevice;
ACID_CONTEXT_EXPORT extern PFN_vkCreateEvent vkCreateEvent;
ACID_CONTEXT_EXPORT extern PFN_vkCreateFence vkCreateFence;
ACID_CONTEXT_EXPORT extern PFN_vkCreateFramebuffer vkCreateFramebuffer;
ACID_CONTEXT_EXPORT extern PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
ACID_CONTEXT_EXPORT extern PFN_vkCreateImage vkCreateImage;
ACID_CONTEXT_EXPORT extern PFN_vkCreateImageView vkCreateImageView;
ACID_CONTEXT_EXPORT extern PFN_vkCreateInstance vkCreateInstance;
ACID_CONTEXT_EXPORT extern PFN_vkCreatePipelineCache vkCreatePipelineCache;
ACID_CONTEXT_EXPORT extern PFN_vkCreatePipelineLayout vkCreatePipelineLayout;
ACID_CONTEXT_EXPORT extern PFN_vkCreateQueryPool vkCreateQueryPool;
ACID_CONTEXT_EXPORT extern PFN_vkCreateRenderPass vkCreateRenderPass;
ACID_CONTEXT_EXPORT extern PFN_vkCreateSampler vkCreateSampler;
ACID_CONTEXT_EXPORT extern PFN_vkCreateSemaphore vkCreateSemaphore;
ACID_CONTEXT_EXPORT extern PFN_vkCreateShaderModule vkCreateShaderModule;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyBuffer vkDestroyBuffer;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyBufferView vkDestroyBufferView;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyCommandPool vkDestroyCommandPool;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyDevice vkDestroyDevice;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyEvent vkDestroyEvent;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyFence vkDestroyFence;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyFramebuffer vkDestroyFramebuffer;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyImage vkDestroyImage;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyImageView vkDestroyImageView;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyInstance vkDestroyInstance;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyPipeline vkDestroyPipeline;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyPipelineCache vkDestroyPipelineCache;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyQueryPool vkDestroyQueryPool;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyRenderPass vkDestroyRenderPass;
ACID_CONTEXT_EXPORT extern PFN_vkDestroySampler vkDestroySampler;
ACID_CONTEXT_EXPORT extern PFN_vkDestroySemaphore vkDestroySemaphore;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyShaderModule vkDestroyShaderModule;
ACID_CONTEXT_EXPORT extern PFN_vkDeviceWaitIdle vkDeviceWaitIdle;
ACID_CONTEXT_EXPORT extern PFN_vkEndCommandBuffer vkEndCommandBuffer;
ACID_CONTEXT_EXPORT extern PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
ACID_CONTEXT_EXPORT extern PFN_vkEnumerateDeviceLayerProperties vkEnumerateDeviceLayerProperties;
ACID_CONTEXT_EXPORT extern PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
ACID_CONTEXT_EXPORT extern PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
ACID_CONTEXT_EXPORT extern PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
ACID_CONTEXT_EXPORT extern PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges;
ACID_CONTEXT_EXPORT extern PFN_vkFreeCommandBuffers vkFreeCommandBuffers;
ACID_CONTEXT_EXPORT extern PFN_vkFreeDescriptorSets vkFreeDescriptorSets;
ACID_CONTEXT_EXPORT extern PFN_vkFreeMemory vkFreeMemory;
ACID_CONTEXT_EXPORT extern PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements;
ACID_CONTEXT_EXPORT extern PFN_vkGetDeviceMemoryCommitment vkGetDeviceMemoryCommitment;
ACID_CONTEXT_EXPORT extern PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
ACID_CONTEXT_EXPORT extern PFN_vkGetDeviceQueue vkGetDeviceQueue;
ACID_CONTEXT_EXPORT extern PFN_vkGetEventStatus vkGetEventStatus;
ACID_CONTEXT_EXPORT extern PFN_vkGetFenceStatus vkGetFenceStatus;
ACID_CONTEXT_EXPORT extern PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements;
ACID_CONTEXT_EXPORT extern PFN_vkGetImageSparseMemoryRequirements vkGetImageSparseMemoryRequirements;
ACID_CONTEXT_EXPORT extern PFN_vkGetImageSubresourceLayout vkGetImageSubresourceLayout;
ACID_CONTEXT_EXPORT extern PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceImageFormatProperties vkGetPhysicalDeviceImageFormatProperties;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceSparseImageFormatProperties vkGetPhysicalDeviceSparseImageFormatProperties;
ACID_CONTEXT_EXPORT extern PFN_vkGetPipelineCacheData vkGetPipelineCacheData;
ACID_CONTEXT_EXPORT extern PFN_vkGetQueryPoolResults vkGetQueryPoolResults;
ACID_CONTEXT_EXPORT extern PFN_vkGetRenderAreaGranularity vkGetRenderAreaGranularity;
ACID_CONTEXT_EXPORT extern PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges;
ACID_CONTEXT_EXPORT extern PFN_vkMapMemory vkMapMemory;
ACID_CONTEXT_EXPORT extern PFN_vkMergePipelineCaches vkMergePipelineCaches;
ACID_CONTEXT_EXPORT extern PFN_vkQueueBindSparse vkQueueBindSparse;
ACID_CONTEXT_EXPORT extern PFN_vkQueueSubmit vkQueueSubmit;
ACID_CONTEXT_EXPORT extern PFN_vkQueueWaitIdle vkQueueWaitIdle;
ACID_CONTEXT_EXPORT extern PFN_vkResetCommandBuffer vkResetCommandBuffer;
ACID_CONTEXT_EXPORT extern PFN_vkResetCommandPool vkResetCommandPool;
ACID_CONTEXT_EXPORT extern PFN_vkResetDescriptorPool vkResetDescriptorPool;
ACID_CONTEXT_EXPORT extern PFN_vkResetEvent vkResetEvent;
ACID_CONTEXT_EXPORT extern PFN_vkResetFences vkResetFences;
ACID_CONTEXT_EXPORT extern PFN_vkSetEvent vkSetEvent;
ACID_CONTEXT_EXPORT extern PFN_vkUnmapMemory vkUnmapMemory;
ACID_CONTEXT_EXPORT extern PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets;
ACID_CONTEXT_EXPORT extern PFN_vkWaitForFences vkWaitForFences;
#endif /* defined(VK_VERSION_1_0) */
#if defined(VK_VERSION_1_1)
ACID_CONTEXT_EXPORT extern PFN_vkBindBufferMemory2 vkBindBufferMemory2;
ACID_CONTEXT_EXPORT extern PFN_vkBindImageMemory2 vkBindImageMemory2;
ACID_CONTEXT_EXPORT extern PFN_vkCmdDispatchBase vkCmdDispatchBase;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetDeviceMask vkCmdSetDeviceMask;
ACID_CONTEXT_EXPORT extern PFN_vkCreateDescriptorUpdateTemplate vkCreateDescriptorUpdateTemplate;
ACID_CONTEXT_EXPORT extern PFN_vkCreateSamplerYcbcrConversion vkCreateSamplerYcbcrConversion;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyDescriptorUpdateTemplate vkDestroyDescriptorUpdateTemplate;
ACID_CONTEXT_EXPORT extern PFN_vkDestroySamplerYcbcrConversion vkDestroySamplerYcbcrConversion;
ACID_CONTEXT_EXPORT extern PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion;
ACID_CONTEXT_EXPORT extern PFN_vkEnumeratePhysicalDeviceGroups vkEnumeratePhysicalDeviceGroups;
ACID_CONTEXT_EXPORT extern PFN_vkGetBufferMemoryRequirements2 vkGetBufferMemoryRequirements2;
ACID_CONTEXT_EXPORT extern PFN_vkGetDescriptorSetLayoutSupport vkGetDescriptorSetLayoutSupport;
ACID_CONTEXT_EXPORT extern PFN_vkGetDeviceGroupPeerMemoryFeatures vkGetDeviceGroupPeerMemoryFeatures;
ACID_CONTEXT_EXPORT extern PFN_vkGetDeviceQueue2 vkGetDeviceQueue2;
ACID_CONTEXT_EXPORT extern PFN_vkGetImageMemoryRequirements2 vkGetImageMemoryRequirements2;
ACID_CONTEXT_EXPORT extern PFN_vkGetImageSparseMemoryRequirements2 vkGetImageSparseMemoryRequirements2;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceExternalBufferProperties vkGetPhysicalDeviceExternalBufferProperties;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceExternalFenceProperties vkGetPhysicalDeviceExternalFenceProperties;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceExternalSemaphoreProperties vkGetPhysicalDeviceExternalSemaphoreProperties;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceFeatures2 vkGetPhysicalDeviceFeatures2;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceFormatProperties2 vkGetPhysicalDeviceFormatProperties2;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceImageFormatProperties2 vkGetPhysicalDeviceImageFormatProperties2;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceMemoryProperties2 vkGetPhysicalDeviceMemoryProperties2;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceProperties2 vkGetPhysicalDeviceProperties2;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceQueueFamilyProperties2 vkGetPhysicalDeviceQueueFamilyProperties2;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceSparseImageFormatProperties2 vkGetPhysicalDeviceSparseImageFormatProperties2;
ACID_CONTEXT_EXPORT extern PFN_vkTrimCommandPool vkTrimCommandPool;
ACID_CONTEXT_EXPORT extern PFN_vkUpdateDescriptorSetWithTemplate vkUpdateDescriptorSetWithTemplate;
#endif /* defined(VK_VERSION_1_1) */
#if defined(VK_VERSION_1_2)
ACID_CONTEXT_EXPORT extern PFN_vkCmdBeginRenderPass2 vkCmdBeginRenderPass2;
ACID_CONTEXT_EXPORT extern PFN_vkCmdDrawIndexedIndirectCount vkCmdDrawIndexedIndirectCount;
ACID_CONTEXT_EXPORT extern PFN_vkCmdDrawIndirectCount vkCmdDrawIndirectCount;
ACID_CONTEXT_EXPORT extern PFN_vkCmdEndRenderPass2 vkCmdEndRenderPass2;
ACID_CONTEXT_EXPORT extern PFN_vkCmdNextSubpass2 vkCmdNextSubpass2;
ACID_CONTEXT_EXPORT extern PFN_vkCreateRenderPass2 vkCreateRenderPass2;
ACID_CONTEXT_EXPORT extern PFN_vkGetBufferDeviceAddress vkGetBufferDeviceAddress;
ACID_CONTEXT_EXPORT extern PFN_vkGetBufferOpaqueCaptureAddress vkGetBufferOpaqueCaptureAddress;
ACID_CONTEXT_EXPORT extern PFN_vkGetDeviceMemoryOpaqueCaptureAddress vkGetDeviceMemoryOpaqueCaptureAddress;
ACID_CONTEXT_EXPORT extern PFN_vkGetSemaphoreCounterValue vkGetSemaphoreCounterValue;
ACID_CONTEXT_EXPORT extern PFN_vkResetQueryPool vkResetQueryPool;
ACID_CONTEXT_EXPORT extern PFN_vkSignalSemaphore vkSignalSemaphore;
ACID_CONTEXT_EXPORT extern PFN_vkWaitSemaphores vkWaitSemaphores;
#endif /* defined(VK_VERSION_1_2) */
#if defined(VK_VERSION_1_3)
ACID_CONTEXT_EXPORT extern PFN_vkCmdBeginRendering vkCmdBeginRendering;
ACID_CONTEXT_EXPORT extern PFN_vkCmdBindVertexBuffers2 vkCmdBindVertexBuffers2;
ACID_CONTEXT_EXPORT extern PFN_vkCmdBlitImage2 vkCmdBlitImage2;
ACID_CONTEXT_EXPORT extern PFN_vkCmdCopyBuffer2 vkCmdCopyBuffer2;
ACID_CONTEXT_EXPORT extern PFN_vkCmdCopyBufferToImage2 vkCmdCopyBufferToImage2;
ACID_CONTEXT_EXPORT extern PFN_vkCmdCopyImage2 vkCmdCopyImage2;
ACID_CONTEXT_EXPORT extern PFN_vkCmdCopyImageToBuffer2 vkCmdCopyImageToBuffer2;
ACID_CONTEXT_EXPORT extern PFN_vkCmdEndRendering vkCmdEndRendering;
ACID_CONTEXT_EXPORT extern PFN_vkCmdPipelineBarrier2 vkCmdPipelineBarrier2;
ACID_CONTEXT_EXPORT extern PFN_vkCmdResetEvent2 vkCmdResetEvent2;
ACID_CONTEXT_EXPORT extern PFN_vkCmdResolveImage2 vkCmdResolveImage2;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetCullMode vkCmdSetCullMode;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetDepthBiasEnable vkCmdSetDepthBiasEnable;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetDepthBoundsTestEnable vkCmdSetDepthBoundsTestEnable;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetDepthCompareOp vkCmdSetDepthCompareOp;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetDepthTestEnable vkCmdSetDepthTestEnable;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetDepthWriteEnable vkCmdSetDepthWriteEnable;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetEvent2 vkCmdSetEvent2;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetFrontFace vkCmdSetFrontFace;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetPrimitiveRestartEnable vkCmdSetPrimitiveRestartEnable;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetPrimitiveTopology vkCmdSetPrimitiveTopology;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetRasterizerDiscardEnable vkCmdSetRasterizerDiscardEnable;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetScissorWithCount vkCmdSetScissorWithCount;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetStencilOp vkCmdSetStencilOp;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetStencilTestEnable vkCmdSetStencilTestEnable;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetViewportWithCount vkCmdSetViewportWithCount;
ACID_CONTEXT_EXPORT extern PFN_vkCmdWaitEvents2 vkCmdWaitEvents2;
ACID_CONTEXT_EXPORT extern PFN_vkCmdWriteTimestamp2 vkCmdWriteTimestamp2;
ACID_CONTEXT_EXPORT extern PFN_vkCreatePrivateDataSlot vkCreatePrivateDataSlot;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyPrivateDataSlot vkDestroyPrivateDataSlot;
ACID_CONTEXT_EXPORT extern PFN_vkGetDeviceBufferMemoryRequirements vkGetDeviceBufferMemoryRequirements;
ACID_CONTEXT_EXPORT extern PFN_vkGetDeviceImageMemoryRequirements vkGetDeviceImageMemoryRequirements;
ACID_CONTEXT_EXPORT extern PFN_vkGetDeviceImageSparseMemoryRequirements vkGetDeviceImageSparseMemoryRequirements;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceToolProperties vkGetPhysicalDeviceToolProperties;
ACID_CONTEXT_EXPORT extern PFN_vkGetPrivateData vkGetPrivateData;
ACID_CONTEXT_EXPORT extern PFN_vkQueueSubmit2 vkQueueSubmit2;
ACID_CONTEXT_EXPORT extern PFN_vkSetPrivateData vkSetPrivateData;
#endif /* defined(VK_VERSION_1_3) */
#if defined(VK_AMD_buffer_marker)
ACID_CONTEXT_EXPORT extern PFN_vkCmdWriteBufferMarkerAMD vkCmdWriteBufferMarkerAMD;
#endif /* defined(VK_AMD_buffer_marker) */
#if defined(VK_AMD_display_native_hdr)
ACID_CONTEXT_EXPORT extern PFN_vkSetLocalDimmingAMD vkSetLocalDimmingAMD;
#endif /* defined(VK_AMD_display_native_hdr) */
#if defined(VK_AMD_draw_indirect_count)
ACID_CONTEXT_EXPORT extern PFN_vkCmdDrawIndexedIndirectCountAMD vkCmdDrawIndexedIndirectCountAMD;
ACID_CONTEXT_EXPORT extern PFN_vkCmdDrawIndirectCountAMD vkCmdDrawIndirectCountAMD;
#endif /* defined(VK_AMD_draw_indirect_count) */
#if defined(VK_AMD_shader_info)
ACID_CONTEXT_EXPORT extern PFN_vkGetShaderInfoAMD vkGetShaderInfoAMD;
#endif /* defined(VK_AMD_shader_info) */
#if defined(VK_ANDROID_external_memory_android_hardware_buffer)
ACID_CONTEXT_EXPORT extern PFN_vkGetAndroidHardwareBufferPropertiesANDROID vkGetAndroidHardwareBufferPropertiesANDROID;
ACID_CONTEXT_EXPORT extern PFN_vkGetMemoryAndroidHardwareBufferANDROID vkGetMemoryAndroidHardwareBufferANDROID;
#endif /* defined(VK_ANDROID_external_memory_android_hardware_buffer) */
#if defined(VK_EXT_acquire_drm_display)
ACID_CONTEXT_EXPORT extern PFN_vkAcquireDrmDisplayEXT vkAcquireDrmDisplayEXT;
ACID_CONTEXT_EXPORT extern PFN_vkGetDrmDisplayEXT vkGetDrmDisplayEXT;
#endif /* defined(VK_EXT_acquire_drm_display) */
#if defined(VK_EXT_acquire_xlib_display)
ACID_CONTEXT_EXPORT extern PFN_vkAcquireXlibDisplayEXT vkAcquireXlibDisplayEXT;
ACID_CONTEXT_EXPORT extern PFN_vkGetRandROutputDisplayEXT vkGetRandROutputDisplayEXT;
#endif /* defined(VK_EXT_acquire_xlib_display) */
#if defined(VK_EXT_buffer_device_address)
ACID_CONTEXT_EXPORT extern PFN_vkGetBufferDeviceAddressEXT vkGetBufferDeviceAddressEXT;
#endif /* defined(VK_EXT_buffer_device_address) */
#if defined(VK_EXT_calibrated_timestamps)
ACID_CONTEXT_EXPORT extern PFN_vkGetCalibratedTimestampsEXT vkGetCalibratedTimestampsEXT;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT vkGetPhysicalDeviceCalibrateableTimeDomainsEXT;
#endif /* defined(VK_EXT_calibrated_timestamps) */
#if defined(VK_EXT_color_write_enable)
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetColorWriteEnableEXT vkCmdSetColorWriteEnableEXT;
#endif /* defined(VK_EXT_color_write_enable) */
#if defined(VK_EXT_conditional_rendering)
ACID_CONTEXT_EXPORT extern PFN_vkCmdBeginConditionalRenderingEXT vkCmdBeginConditionalRenderingEXT;
ACID_CONTEXT_EXPORT extern PFN_vkCmdEndConditionalRenderingEXT vkCmdEndConditionalRenderingEXT;
#endif /* defined(VK_EXT_conditional_rendering) */
#if defined(VK_EXT_debug_marker)
ACID_CONTEXT_EXPORT extern PFN_vkCmdDebugMarkerBeginEXT vkCmdDebugMarkerBeginEXT;
ACID_CONTEXT_EXPORT extern PFN_vkCmdDebugMarkerEndEXT vkCmdDebugMarkerEndEXT;
ACID_CONTEXT_EXPORT extern PFN_vkCmdDebugMarkerInsertEXT vkCmdDebugMarkerInsertEXT;
ACID_CONTEXT_EXPORT extern PFN_vkDebugMarkerSetObjectNameEXT vkDebugMarkerSetObjectNameEXT;
ACID_CONTEXT_EXPORT extern PFN_vkDebugMarkerSetObjectTagEXT vkDebugMarkerSetObjectTagEXT;
#endif /* defined(VK_EXT_debug_marker) */
#if defined(VK_EXT_debug_report)
ACID_CONTEXT_EXPORT extern PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT;
ACID_CONTEXT_EXPORT extern PFN_vkDebugReportMessageEXT vkDebugReportMessageEXT;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT;
#endif /* defined(VK_EXT_debug_report) */
#if defined(VK_EXT_debug_utils)
ACID_CONTEXT_EXPORT extern PFN_vkCmdBeginDebugUtilsLabelEXT vkCmdBeginDebugUtilsLabelEXT;
ACID_CONTEXT_EXPORT extern PFN_vkCmdEndDebugUtilsLabelEXT vkCmdEndDebugUtilsLabelEXT;
ACID_CONTEXT_EXPORT extern PFN_vkCmdInsertDebugUtilsLabelEXT vkCmdInsertDebugUtilsLabelEXT;
ACID_CONTEXT_EXPORT extern PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;
ACID_CONTEXT_EXPORT extern PFN_vkQueueBeginDebugUtilsLabelEXT vkQueueBeginDebugUtilsLabelEXT;
ACID_CONTEXT_EXPORT extern PFN_vkQueueEndDebugUtilsLabelEXT vkQueueEndDebugUtilsLabelEXT;
ACID_CONTEXT_EXPORT extern PFN_vkQueueInsertDebugUtilsLabelEXT vkQueueInsertDebugUtilsLabelEXT;
ACID_CONTEXT_EXPORT extern PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT;
ACID_CONTEXT_EXPORT extern PFN_vkSetDebugUtilsObjectTagEXT vkSetDebugUtilsObjectTagEXT;
ACID_CONTEXT_EXPORT extern PFN_vkSubmitDebugUtilsMessageEXT vkSubmitDebugUtilsMessageEXT;
#endif /* defined(VK_EXT_debug_utils) */
#if defined(VK_EXT_direct_mode_display)
ACID_CONTEXT_EXPORT extern PFN_vkReleaseDisplayEXT vkReleaseDisplayEXT;
#endif /* defined(VK_EXT_direct_mode_display) */
#if defined(VK_EXT_directfb_surface)
ACID_CONTEXT_EXPORT extern PFN_vkCreateDirectFBSurfaceEXT vkCreateDirectFBSurfaceEXT;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceDirectFBPresentationSupportEXT vkGetPhysicalDeviceDirectFBPresentationSupportEXT;
#endif /* defined(VK_EXT_directfb_surface) */
#if defined(VK_EXT_discard_rectangles)
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetDiscardRectangleEXT vkCmdSetDiscardRectangleEXT;
#endif /* defined(VK_EXT_discard_rectangles) */
#if defined(VK_EXT_display_control)
ACID_CONTEXT_EXPORT extern PFN_vkDisplayPowerControlEXT vkDisplayPowerControlEXT;
ACID_CONTEXT_EXPORT extern PFN_vkGetSwapchainCounterEXT vkGetSwapchainCounterEXT;
ACID_CONTEXT_EXPORT extern PFN_vkRegisterDeviceEventEXT vkRegisterDeviceEventEXT;
ACID_CONTEXT_EXPORT extern PFN_vkRegisterDisplayEventEXT vkRegisterDisplayEventEXT;
#endif /* defined(VK_EXT_display_control) */
#if defined(VK_EXT_display_surface_counter)
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT vkGetPhysicalDeviceSurfaceCapabilities2EXT;
#endif /* defined(VK_EXT_display_surface_counter) */
#if defined(VK_EXT_extended_dynamic_state)
ACID_CONTEXT_EXPORT extern PFN_vkCmdBindVertexBuffers2EXT vkCmdBindVertexBuffers2EXT;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetCullModeEXT vkCmdSetCullModeEXT;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetDepthBoundsTestEnableEXT vkCmdSetDepthBoundsTestEnableEXT;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetDepthCompareOpEXT vkCmdSetDepthCompareOpEXT;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetDepthTestEnableEXT vkCmdSetDepthTestEnableEXT;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetDepthWriteEnableEXT vkCmdSetDepthWriteEnableEXT;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetFrontFaceEXT vkCmdSetFrontFaceEXT;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetPrimitiveTopologyEXT vkCmdSetPrimitiveTopologyEXT;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetScissorWithCountEXT vkCmdSetScissorWithCountEXT;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetStencilOpEXT vkCmdSetStencilOpEXT;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetStencilTestEnableEXT vkCmdSetStencilTestEnableEXT;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetViewportWithCountEXT vkCmdSetViewportWithCountEXT;
#endif /* defined(VK_EXT_extended_dynamic_state) */
#if defined(VK_EXT_extended_dynamic_state2)
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetDepthBiasEnableEXT vkCmdSetDepthBiasEnableEXT;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetLogicOpEXT vkCmdSetLogicOpEXT;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetPatchControlPointsEXT vkCmdSetPatchControlPointsEXT;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetPrimitiveRestartEnableEXT vkCmdSetPrimitiveRestartEnableEXT;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetRasterizerDiscardEnableEXT vkCmdSetRasterizerDiscardEnableEXT;
#endif /* defined(VK_EXT_extended_dynamic_state2) */
#if defined(VK_EXT_external_memory_host)
ACID_CONTEXT_EXPORT extern PFN_vkGetMemoryHostPointerPropertiesEXT vkGetMemoryHostPointerPropertiesEXT;
#endif /* defined(VK_EXT_external_memory_host) */
#if defined(VK_EXT_full_screen_exclusive)
ACID_CONTEXT_EXPORT extern PFN_vkAcquireFullScreenExclusiveModeEXT vkAcquireFullScreenExclusiveModeEXT;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceSurfacePresentModes2EXT vkGetPhysicalDeviceSurfacePresentModes2EXT;
ACID_CONTEXT_EXPORT extern PFN_vkReleaseFullScreenExclusiveModeEXT vkReleaseFullScreenExclusiveModeEXT;
#endif /* defined(VK_EXT_full_screen_exclusive) */
#if defined(VK_EXT_hdr_metadata)
ACID_CONTEXT_EXPORT extern PFN_vkSetHdrMetadataEXT vkSetHdrMetadataEXT;
#endif /* defined(VK_EXT_hdr_metadata) */
#if defined(VK_EXT_headless_surface)
ACID_CONTEXT_EXPORT extern PFN_vkCreateHeadlessSurfaceEXT vkCreateHeadlessSurfaceEXT;
#endif /* defined(VK_EXT_headless_surface) */
#if defined(VK_EXT_host_query_reset)
ACID_CONTEXT_EXPORT extern PFN_vkResetQueryPoolEXT vkResetQueryPoolEXT;
#endif /* defined(VK_EXT_host_query_reset) */
#if defined(VK_EXT_image_drm_format_modifier)
ACID_CONTEXT_EXPORT extern PFN_vkGetImageDrmFormatModifierPropertiesEXT vkGetImageDrmFormatModifierPropertiesEXT;
#endif /* defined(VK_EXT_image_drm_format_modifier) */
#if defined(VK_EXT_line_rasterization)
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetLineStippleEXT vkCmdSetLineStippleEXT;
#endif /* defined(VK_EXT_line_rasterization) */
#if defined(VK_EXT_metal_surface)
ACID_CONTEXT_EXPORT extern PFN_vkCreateMetalSurfaceEXT vkCreateMetalSurfaceEXT;
#endif /* defined(VK_EXT_metal_surface) */
#if defined(VK_EXT_multi_draw)
ACID_CONTEXT_EXPORT extern PFN_vkCmdDrawMultiEXT vkCmdDrawMultiEXT;
ACID_CONTEXT_EXPORT extern PFN_vkCmdDrawMultiIndexedEXT vkCmdDrawMultiIndexedEXT;
#endif /* defined(VK_EXT_multi_draw) */
#if defined(VK_EXT_pageable_device_local_memory)
ACID_CONTEXT_EXPORT extern PFN_vkSetDeviceMemoryPriorityEXT vkSetDeviceMemoryPriorityEXT;
#endif /* defined(VK_EXT_pageable_device_local_memory) */
#if defined(VK_EXT_private_data)
ACID_CONTEXT_EXPORT extern PFN_vkCreatePrivateDataSlotEXT vkCreatePrivateDataSlotEXT;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyPrivateDataSlotEXT vkDestroyPrivateDataSlotEXT;
ACID_CONTEXT_EXPORT extern PFN_vkGetPrivateDataEXT vkGetPrivateDataEXT;
ACID_CONTEXT_EXPORT extern PFN_vkSetPrivateDataEXT vkSetPrivateDataEXT;
#endif /* defined(VK_EXT_private_data) */
#if defined(VK_EXT_sample_locations)
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetSampleLocationsEXT vkCmdSetSampleLocationsEXT;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT vkGetPhysicalDeviceMultisamplePropertiesEXT;
#endif /* defined(VK_EXT_sample_locations) */
#if defined(VK_EXT_tooling_info)
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceToolPropertiesEXT vkGetPhysicalDeviceToolPropertiesEXT;
#endif /* defined(VK_EXT_tooling_info) */
#if defined(VK_EXT_transform_feedback)
ACID_CONTEXT_EXPORT extern PFN_vkCmdBeginQueryIndexedEXT vkCmdBeginQueryIndexedEXT;
ACID_CONTEXT_EXPORT extern PFN_vkCmdBeginTransformFeedbackEXT vkCmdBeginTransformFeedbackEXT;
ACID_CONTEXT_EXPORT extern PFN_vkCmdBindTransformFeedbackBuffersEXT vkCmdBindTransformFeedbackBuffersEXT;
ACID_CONTEXT_EXPORT extern PFN_vkCmdDrawIndirectByteCountEXT vkCmdDrawIndirectByteCountEXT;
ACID_CONTEXT_EXPORT extern PFN_vkCmdEndQueryIndexedEXT vkCmdEndQueryIndexedEXT;
ACID_CONTEXT_EXPORT extern PFN_vkCmdEndTransformFeedbackEXT vkCmdEndTransformFeedbackEXT;
#endif /* defined(VK_EXT_transform_feedback) */
#if defined(VK_EXT_validation_cache)
ACID_CONTEXT_EXPORT extern PFN_vkCreateValidationCacheEXT vkCreateValidationCacheEXT;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyValidationCacheEXT vkDestroyValidationCacheEXT;
ACID_CONTEXT_EXPORT extern PFN_vkGetValidationCacheDataEXT vkGetValidationCacheDataEXT;
ACID_CONTEXT_EXPORT extern PFN_vkMergeValidationCachesEXT vkMergeValidationCachesEXT;
#endif /* defined(VK_EXT_validation_cache) */
#if defined(VK_EXT_vertex_input_dynamic_state)
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetVertexInputEXT vkCmdSetVertexInputEXT;
#endif /* defined(VK_EXT_vertex_input_dynamic_state) */
#if defined(VK_FUCHSIA_buffer_collection)
ACID_CONTEXT_EXPORT extern PFN_vkCreateBufferCollectionFUCHSIA vkCreateBufferCollectionFUCHSIA;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyBufferCollectionFUCHSIA vkDestroyBufferCollectionFUCHSIA;
ACID_CONTEXT_EXPORT extern PFN_vkGetBufferCollectionPropertiesFUCHSIA vkGetBufferCollectionPropertiesFUCHSIA;
ACID_CONTEXT_EXPORT extern PFN_vkSetBufferCollectionBufferConstraintsFUCHSIA vkSetBufferCollectionBufferConstraintsFUCHSIA;
ACID_CONTEXT_EXPORT extern PFN_vkSetBufferCollectionImageConstraintsFUCHSIA vkSetBufferCollectionImageConstraintsFUCHSIA;
#endif /* defined(VK_FUCHSIA_buffer_collection) */
#if defined(VK_FUCHSIA_external_memory)
ACID_CONTEXT_EXPORT extern PFN_vkGetMemoryZirconHandleFUCHSIA vkGetMemoryZirconHandleFUCHSIA;
ACID_CONTEXT_EXPORT extern PFN_vkGetMemoryZirconHandlePropertiesFUCHSIA vkGetMemoryZirconHandlePropertiesFUCHSIA;
#endif /* defined(VK_FUCHSIA_external_memory) */
#if defined(VK_FUCHSIA_external_semaphore)
ACID_CONTEXT_EXPORT extern PFN_vkGetSemaphoreZirconHandleFUCHSIA vkGetSemaphoreZirconHandleFUCHSIA;
ACID_CONTEXT_EXPORT extern PFN_vkImportSemaphoreZirconHandleFUCHSIA vkImportSemaphoreZirconHandleFUCHSIA;
#endif /* defined(VK_FUCHSIA_external_semaphore) */
#if defined(VK_FUCHSIA_imagepipe_surface)
ACID_CONTEXT_EXPORT extern PFN_vkCreateImagePipeSurfaceFUCHSIA vkCreateImagePipeSurfaceFUCHSIA;
#endif /* defined(VK_FUCHSIA_imagepipe_surface) */
#if defined(VK_GGP_stream_descriptor_surface)
ACID_CONTEXT_EXPORT extern PFN_vkCreateStreamDescriptorSurfaceGGP vkCreateStreamDescriptorSurfaceGGP;
#endif /* defined(VK_GGP_stream_descriptor_surface) */
#if defined(VK_GOOGLE_display_timing)
ACID_CONTEXT_EXPORT extern PFN_vkGetPastPresentationTimingGOOGLE vkGetPastPresentationTimingGOOGLE;
ACID_CONTEXT_EXPORT extern PFN_vkGetRefreshCycleDurationGOOGLE vkGetRefreshCycleDurationGOOGLE;
#endif /* defined(VK_GOOGLE_display_timing) */
#if defined(VK_HUAWEI_invocation_mask)
ACID_CONTEXT_EXPORT extern PFN_vkCmdBindInvocationMaskHUAWEI vkCmdBindInvocationMaskHUAWEI;
#endif /* defined(VK_HUAWEI_invocation_mask) */
#if defined(VK_HUAWEI_subpass_shading)
ACID_CONTEXT_EXPORT extern PFN_vkCmdSubpassShadingHUAWEI vkCmdSubpassShadingHUAWEI;
ACID_CONTEXT_EXPORT extern PFN_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI;
#endif /* defined(VK_HUAWEI_subpass_shading) */
#if defined(VK_INTEL_performance_query)
ACID_CONTEXT_EXPORT extern PFN_vkAcquirePerformanceConfigurationINTEL vkAcquirePerformanceConfigurationINTEL;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetPerformanceMarkerINTEL vkCmdSetPerformanceMarkerINTEL;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetPerformanceOverrideINTEL vkCmdSetPerformanceOverrideINTEL;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetPerformanceStreamMarkerINTEL vkCmdSetPerformanceStreamMarkerINTEL;
ACID_CONTEXT_EXPORT extern PFN_vkGetPerformanceParameterINTEL vkGetPerformanceParameterINTEL;
ACID_CONTEXT_EXPORT extern PFN_vkInitializePerformanceApiINTEL vkInitializePerformanceApiINTEL;
ACID_CONTEXT_EXPORT extern PFN_vkQueueSetPerformanceConfigurationINTEL vkQueueSetPerformanceConfigurationINTEL;
ACID_CONTEXT_EXPORT extern PFN_vkReleasePerformanceConfigurationINTEL vkReleasePerformanceConfigurationINTEL;
ACID_CONTEXT_EXPORT extern PFN_vkUninitializePerformanceApiINTEL vkUninitializePerformanceApiINTEL;
#endif /* defined(VK_INTEL_performance_query) */
#if defined(VK_KHR_acceleration_structure)
ACID_CONTEXT_EXPORT extern PFN_vkBuildAccelerationStructuresKHR vkBuildAccelerationStructuresKHR;
ACID_CONTEXT_EXPORT extern PFN_vkCmdBuildAccelerationStructuresIndirectKHR vkCmdBuildAccelerationStructuresIndirectKHR;
ACID_CONTEXT_EXPORT extern PFN_vkCmdBuildAccelerationStructuresKHR vkCmdBuildAccelerationStructuresKHR;
ACID_CONTEXT_EXPORT extern PFN_vkCmdCopyAccelerationStructureKHR vkCmdCopyAccelerationStructureKHR;
ACID_CONTEXT_EXPORT extern PFN_vkCmdCopyAccelerationStructureToMemoryKHR vkCmdCopyAccelerationStructureToMemoryKHR;
ACID_CONTEXT_EXPORT extern PFN_vkCmdCopyMemoryToAccelerationStructureKHR vkCmdCopyMemoryToAccelerationStructureKHR;
ACID_CONTEXT_EXPORT extern PFN_vkCmdWriteAccelerationStructuresPropertiesKHR vkCmdWriteAccelerationStructuresPropertiesKHR;
ACID_CONTEXT_EXPORT extern PFN_vkCopyAccelerationStructureKHR vkCopyAccelerationStructureKHR;
ACID_CONTEXT_EXPORT extern PFN_vkCopyAccelerationStructureToMemoryKHR vkCopyAccelerationStructureToMemoryKHR;
ACID_CONTEXT_EXPORT extern PFN_vkCopyMemoryToAccelerationStructureKHR vkCopyMemoryToAccelerationStructureKHR;
ACID_CONTEXT_EXPORT extern PFN_vkCreateAccelerationStructureKHR vkCreateAccelerationStructureKHR;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyAccelerationStructureKHR vkDestroyAccelerationStructureKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizesKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetAccelerationStructureDeviceAddressKHR vkGetAccelerationStructureDeviceAddressKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetDeviceAccelerationStructureCompatibilityKHR vkGetDeviceAccelerationStructureCompatibilityKHR;
ACID_CONTEXT_EXPORT extern PFN_vkWriteAccelerationStructuresPropertiesKHR vkWriteAccelerationStructuresPropertiesKHR;
#endif /* defined(VK_KHR_acceleration_structure) */
#if defined(VK_KHR_android_surface)
ACID_CONTEXT_EXPORT extern PFN_vkCreateAndroidSurfaceKHR vkCreateAndroidSurfaceKHR;
#endif /* defined(VK_KHR_android_surface) */
#if defined(VK_KHR_bind_memory2)
ACID_CONTEXT_EXPORT extern PFN_vkBindBufferMemory2KHR vkBindBufferMemory2KHR;
ACID_CONTEXT_EXPORT extern PFN_vkBindImageMemory2KHR vkBindImageMemory2KHR;
#endif /* defined(VK_KHR_bind_memory2) */
#if defined(VK_KHR_buffer_device_address)
ACID_CONTEXT_EXPORT extern PFN_vkGetBufferDeviceAddressKHR vkGetBufferDeviceAddressKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetBufferOpaqueCaptureAddressKHR vkGetBufferOpaqueCaptureAddressKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetDeviceMemoryOpaqueCaptureAddressKHR vkGetDeviceMemoryOpaqueCaptureAddressKHR;
#endif /* defined(VK_KHR_buffer_device_address) */
#if defined(VK_KHR_copy_commands2)
ACID_CONTEXT_EXPORT extern PFN_vkCmdBlitImage2KHR vkCmdBlitImage2KHR;
ACID_CONTEXT_EXPORT extern PFN_vkCmdCopyBuffer2KHR vkCmdCopyBuffer2KHR;
ACID_CONTEXT_EXPORT extern PFN_vkCmdCopyBufferToImage2KHR vkCmdCopyBufferToImage2KHR;
ACID_CONTEXT_EXPORT extern PFN_vkCmdCopyImage2KHR vkCmdCopyImage2KHR;
ACID_CONTEXT_EXPORT extern PFN_vkCmdCopyImageToBuffer2KHR vkCmdCopyImageToBuffer2KHR;
ACID_CONTEXT_EXPORT extern PFN_vkCmdResolveImage2KHR vkCmdResolveImage2KHR;
#endif /* defined(VK_KHR_copy_commands2) */
#if defined(VK_KHR_create_renderpass2)
ACID_CONTEXT_EXPORT extern PFN_vkCmdBeginRenderPass2KHR vkCmdBeginRenderPass2KHR;
ACID_CONTEXT_EXPORT extern PFN_vkCmdEndRenderPass2KHR vkCmdEndRenderPass2KHR;
ACID_CONTEXT_EXPORT extern PFN_vkCmdNextSubpass2KHR vkCmdNextSubpass2KHR;
ACID_CONTEXT_EXPORT extern PFN_vkCreateRenderPass2KHR vkCreateRenderPass2KHR;
#endif /* defined(VK_KHR_create_renderpass2) */
#if defined(VK_KHR_deferred_host_operations)
ACID_CONTEXT_EXPORT extern PFN_vkCreateDeferredOperationKHR vkCreateDeferredOperationKHR;
ACID_CONTEXT_EXPORT extern PFN_vkDeferredOperationJoinKHR vkDeferredOperationJoinKHR;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyDeferredOperationKHR vkDestroyDeferredOperationKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetDeferredOperationMaxConcurrencyKHR vkGetDeferredOperationMaxConcurrencyKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetDeferredOperationResultKHR vkGetDeferredOperationResultKHR;
#endif /* defined(VK_KHR_deferred_host_operations) */
#if defined(VK_KHR_descriptor_update_template)
ACID_CONTEXT_EXPORT extern PFN_vkCreateDescriptorUpdateTemplateKHR vkCreateDescriptorUpdateTemplateKHR;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyDescriptorUpdateTemplateKHR vkDestroyDescriptorUpdateTemplateKHR;
ACID_CONTEXT_EXPORT extern PFN_vkUpdateDescriptorSetWithTemplateKHR vkUpdateDescriptorSetWithTemplateKHR;
#endif /* defined(VK_KHR_descriptor_update_template) */
#if defined(VK_KHR_device_group)
ACID_CONTEXT_EXPORT extern PFN_vkCmdDispatchBaseKHR vkCmdDispatchBaseKHR;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetDeviceMaskKHR vkCmdSetDeviceMaskKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR vkGetDeviceGroupPeerMemoryFeaturesKHR;
#endif /* defined(VK_KHR_device_group) */
#if defined(VK_KHR_device_group_creation)
ACID_CONTEXT_EXPORT extern PFN_vkEnumeratePhysicalDeviceGroupsKHR vkEnumeratePhysicalDeviceGroupsKHR;
#endif /* defined(VK_KHR_device_group_creation) */
#if defined(VK_KHR_display)
ACID_CONTEXT_EXPORT extern PFN_vkCreateDisplayModeKHR vkCreateDisplayModeKHR;
ACID_CONTEXT_EXPORT extern PFN_vkCreateDisplayPlaneSurfaceKHR vkCreateDisplayPlaneSurfaceKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetDisplayModePropertiesKHR vkGetDisplayModePropertiesKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetDisplayPlaneCapabilitiesKHR vkGetDisplayPlaneCapabilitiesKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetDisplayPlaneSupportedDisplaysKHR vkGetDisplayPlaneSupportedDisplaysKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR vkGetPhysicalDeviceDisplayPlanePropertiesKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceDisplayPropertiesKHR vkGetPhysicalDeviceDisplayPropertiesKHR;
#endif /* defined(VK_KHR_display) */
#if defined(VK_KHR_display_swapchain)
ACID_CONTEXT_EXPORT extern PFN_vkCreateSharedSwapchainsKHR vkCreateSharedSwapchainsKHR;
#endif /* defined(VK_KHR_display_swapchain) */
#if defined(VK_KHR_draw_indirect_count)
ACID_CONTEXT_EXPORT extern PFN_vkCmdDrawIndexedIndirectCountKHR vkCmdDrawIndexedIndirectCountKHR;
ACID_CONTEXT_EXPORT extern PFN_vkCmdDrawIndirectCountKHR vkCmdDrawIndirectCountKHR;
#endif /* defined(VK_KHR_draw_indirect_count) */
#if defined(VK_KHR_dynamic_rendering)
ACID_CONTEXT_EXPORT extern PFN_vkCmdBeginRenderingKHR vkCmdBeginRenderingKHR;
ACID_CONTEXT_EXPORT extern PFN_vkCmdEndRenderingKHR vkCmdEndRenderingKHR;
#endif /* defined(VK_KHR_dynamic_rendering) */
#if defined(VK_KHR_external_fence_capabilities)
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceExternalFencePropertiesKHR vkGetPhysicalDeviceExternalFencePropertiesKHR;
#endif /* defined(VK_KHR_external_fence_capabilities) */
#if defined(VK_KHR_external_fence_fd)
ACID_CONTEXT_EXPORT extern PFN_vkGetFenceFdKHR vkGetFenceFdKHR;
ACID_CONTEXT_EXPORT extern PFN_vkImportFenceFdKHR vkImportFenceFdKHR;
#endif /* defined(VK_KHR_external_fence_fd) */
#if defined(VK_KHR_external_fence_win32)
ACID_CONTEXT_EXPORT extern PFN_vkGetFenceWin32HandleKHR vkGetFenceWin32HandleKHR;
ACID_CONTEXT_EXPORT extern PFN_vkImportFenceWin32HandleKHR vkImportFenceWin32HandleKHR;
#endif /* defined(VK_KHR_external_fence_win32) */
#if defined(VK_KHR_external_memory_capabilities)
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceExternalBufferPropertiesKHR vkGetPhysicalDeviceExternalBufferPropertiesKHR;
#endif /* defined(VK_KHR_external_memory_capabilities) */
#if defined(VK_KHR_external_memory_fd)
ACID_CONTEXT_EXPORT extern PFN_vkGetMemoryFdKHR vkGetMemoryFdKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetMemoryFdPropertiesKHR vkGetMemoryFdPropertiesKHR;
#endif /* defined(VK_KHR_external_memory_fd) */
#if defined(VK_KHR_external_memory_win32)
ACID_CONTEXT_EXPORT extern PFN_vkGetMemoryWin32HandleKHR vkGetMemoryWin32HandleKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetMemoryWin32HandlePropertiesKHR vkGetMemoryWin32HandlePropertiesKHR;
#endif /* defined(VK_KHR_external_memory_win32) */
#if defined(VK_KHR_external_semaphore_capabilities)
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR vkGetPhysicalDeviceExternalSemaphorePropertiesKHR;
#endif /* defined(VK_KHR_external_semaphore_capabilities) */
#if defined(VK_KHR_external_semaphore_fd)
ACID_CONTEXT_EXPORT extern PFN_vkGetSemaphoreFdKHR vkGetSemaphoreFdKHR;
ACID_CONTEXT_EXPORT extern PFN_vkImportSemaphoreFdKHR vkImportSemaphoreFdKHR;
#endif /* defined(VK_KHR_external_semaphore_fd) */
#if defined(VK_KHR_external_semaphore_win32)
ACID_CONTEXT_EXPORT extern PFN_vkGetSemaphoreWin32HandleKHR vkGetSemaphoreWin32HandleKHR;
ACID_CONTEXT_EXPORT extern PFN_vkImportSemaphoreWin32HandleKHR vkImportSemaphoreWin32HandleKHR;
#endif /* defined(VK_KHR_external_semaphore_win32) */
#if defined(VK_KHR_fragment_shading_rate)
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetFragmentShadingRateKHR vkCmdSetFragmentShadingRateKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceFragmentShadingRatesKHR vkGetPhysicalDeviceFragmentShadingRatesKHR;
#endif /* defined(VK_KHR_fragment_shading_rate) */
#if defined(VK_KHR_get_display_properties2)
ACID_CONTEXT_EXPORT extern PFN_vkGetDisplayModeProperties2KHR vkGetDisplayModeProperties2KHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetDisplayPlaneCapabilities2KHR vkGetDisplayPlaneCapabilities2KHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceDisplayPlaneProperties2KHR vkGetPhysicalDeviceDisplayPlaneProperties2KHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceDisplayProperties2KHR vkGetPhysicalDeviceDisplayProperties2KHR;
#endif /* defined(VK_KHR_get_display_properties2) */
#if defined(VK_KHR_get_memory_requirements2)
ACID_CONTEXT_EXPORT extern PFN_vkGetBufferMemoryRequirements2KHR vkGetBufferMemoryRequirements2KHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetImageMemoryRequirements2KHR vkGetImageMemoryRequirements2KHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetImageSparseMemoryRequirements2KHR vkGetImageSparseMemoryRequirements2KHR;
#endif /* defined(VK_KHR_get_memory_requirements2) */
#if defined(VK_KHR_get_physical_device_properties2)
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceFeatures2KHR vkGetPhysicalDeviceFeatures2KHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceFormatProperties2KHR vkGetPhysicalDeviceFormatProperties2KHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceImageFormatProperties2KHR vkGetPhysicalDeviceImageFormatProperties2KHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceMemoryProperties2KHR vkGetPhysicalDeviceMemoryProperties2KHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceProperties2KHR vkGetPhysicalDeviceProperties2KHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR vkGetPhysicalDeviceQueueFamilyProperties2KHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceSparseImageFormatProperties2KHR vkGetPhysicalDeviceSparseImageFormatProperties2KHR;
#endif /* defined(VK_KHR_get_physical_device_properties2) */
#if defined(VK_KHR_get_surface_capabilities2)
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR vkGetPhysicalDeviceSurfaceCapabilities2KHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceSurfaceFormats2KHR vkGetPhysicalDeviceSurfaceFormats2KHR;
#endif /* defined(VK_KHR_get_surface_capabilities2) */
#if defined(VK_KHR_maintenance1)
ACID_CONTEXT_EXPORT extern PFN_vkTrimCommandPoolKHR vkTrimCommandPoolKHR;
#endif /* defined(VK_KHR_maintenance1) */
#if defined(VK_KHR_maintenance3)
ACID_CONTEXT_EXPORT extern PFN_vkGetDescriptorSetLayoutSupportKHR vkGetDescriptorSetLayoutSupportKHR;
#endif /* defined(VK_KHR_maintenance3) */
#if defined(VK_KHR_maintenance4)
ACID_CONTEXT_EXPORT extern PFN_vkGetDeviceBufferMemoryRequirementsKHR vkGetDeviceBufferMemoryRequirementsKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetDeviceImageMemoryRequirementsKHR vkGetDeviceImageMemoryRequirementsKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetDeviceImageSparseMemoryRequirementsKHR vkGetDeviceImageSparseMemoryRequirementsKHR;
#endif /* defined(VK_KHR_maintenance4) */
#if defined(VK_KHR_performance_query)
ACID_CONTEXT_EXPORT extern PFN_vkAcquireProfilingLockKHR vkAcquireProfilingLockKHR;
ACID_CONTEXT_EXPORT extern PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR;
ACID_CONTEXT_EXPORT extern PFN_vkReleaseProfilingLockKHR vkReleaseProfilingLockKHR;
#endif /* defined(VK_KHR_performance_query) */
#if defined(VK_KHR_pipeline_executable_properties)
ACID_CONTEXT_EXPORT extern PFN_vkGetPipelineExecutableInternalRepresentationsKHR vkGetPipelineExecutableInternalRepresentationsKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetPipelineExecutablePropertiesKHR vkGetPipelineExecutablePropertiesKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetPipelineExecutableStatisticsKHR vkGetPipelineExecutableStatisticsKHR;
#endif /* defined(VK_KHR_pipeline_executable_properties) */
#if defined(VK_KHR_present_wait)
ACID_CONTEXT_EXPORT extern PFN_vkWaitForPresentKHR vkWaitForPresentKHR;
#endif /* defined(VK_KHR_present_wait) */
#if defined(VK_KHR_push_descriptor)
ACID_CONTEXT_EXPORT extern PFN_vkCmdPushDescriptorSetKHR vkCmdPushDescriptorSetKHR;
#endif /* defined(VK_KHR_push_descriptor) */
#if defined(VK_KHR_ray_tracing_pipeline)
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetRayTracingPipelineStackSizeKHR vkCmdSetRayTracingPipelineStackSizeKHR;
ACID_CONTEXT_EXPORT extern PFN_vkCmdTraceRaysIndirectKHR vkCmdTraceRaysIndirectKHR;
ACID_CONTEXT_EXPORT extern PFN_vkCmdTraceRaysKHR vkCmdTraceRaysKHR;
ACID_CONTEXT_EXPORT extern PFN_vkCreateRayTracingPipelinesKHR vkCreateRayTracingPipelinesKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR vkGetRayTracingCaptureReplayShaderGroupHandlesKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetRayTracingShaderGroupHandlesKHR vkGetRayTracingShaderGroupHandlesKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetRayTracingShaderGroupStackSizeKHR vkGetRayTracingShaderGroupStackSizeKHR;
#endif /* defined(VK_KHR_ray_tracing_pipeline) */
#if defined(VK_KHR_sampler_ycbcr_conversion)
ACID_CONTEXT_EXPORT extern PFN_vkCreateSamplerYcbcrConversionKHR vkCreateSamplerYcbcrConversionKHR;
ACID_CONTEXT_EXPORT extern PFN_vkDestroySamplerYcbcrConversionKHR vkDestroySamplerYcbcrConversionKHR;
#endif /* defined(VK_KHR_sampler_ycbcr_conversion) */
#if defined(VK_KHR_shared_presentable_image)
ACID_CONTEXT_EXPORT extern PFN_vkGetSwapchainStatusKHR vkGetSwapchainStatusKHR;
#endif /* defined(VK_KHR_shared_presentable_image) */
#if defined(VK_KHR_surface)
ACID_CONTEXT_EXPORT extern PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR;
#endif /* defined(VK_KHR_surface) */
#if defined(VK_KHR_swapchain)
ACID_CONTEXT_EXPORT extern PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR;
ACID_CONTEXT_EXPORT extern PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
ACID_CONTEXT_EXPORT extern PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
ACID_CONTEXT_EXPORT extern PFN_vkQueuePresentKHR vkQueuePresentKHR;
#endif /* defined(VK_KHR_swapchain) */
#if defined(VK_KHR_synchronization2)
ACID_CONTEXT_EXPORT extern PFN_vkCmdPipelineBarrier2KHR vkCmdPipelineBarrier2KHR;
ACID_CONTEXT_EXPORT extern PFN_vkCmdResetEvent2KHR vkCmdResetEvent2KHR;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetEvent2KHR vkCmdSetEvent2KHR;
ACID_CONTEXT_EXPORT extern PFN_vkCmdWaitEvents2KHR vkCmdWaitEvents2KHR;
ACID_CONTEXT_EXPORT extern PFN_vkCmdWriteTimestamp2KHR vkCmdWriteTimestamp2KHR;
ACID_CONTEXT_EXPORT extern PFN_vkQueueSubmit2KHR vkQueueSubmit2KHR;
#endif /* defined(VK_KHR_synchronization2) */
#if defined(VK_KHR_synchronization2) && defined(VK_AMD_buffer_marker)
ACID_CONTEXT_EXPORT extern PFN_vkCmdWriteBufferMarker2AMD vkCmdWriteBufferMarker2AMD;
#endif /* defined(VK_KHR_synchronization2) && defined(VK_AMD_buffer_marker) */
#if defined(VK_KHR_synchronization2) && defined(VK_NV_device_diagnostic_checkpoints)
ACID_CONTEXT_EXPORT extern PFN_vkGetQueueCheckpointData2NV vkGetQueueCheckpointData2NV;
#endif /* defined(VK_KHR_synchronization2) && defined(VK_NV_device_diagnostic_checkpoints) */
#if defined(VK_KHR_timeline_semaphore)
ACID_CONTEXT_EXPORT extern PFN_vkGetSemaphoreCounterValueKHR vkGetSemaphoreCounterValueKHR;
ACID_CONTEXT_EXPORT extern PFN_vkSignalSemaphoreKHR vkSignalSemaphoreKHR;
ACID_CONTEXT_EXPORT extern PFN_vkWaitSemaphoresKHR vkWaitSemaphoresKHR;
#endif /* defined(VK_KHR_timeline_semaphore) */
#if defined(VK_KHR_video_decode_queue)
ACID_CONTEXT_EXPORT extern PFN_vkCmdDecodeVideoKHR vkCmdDecodeVideoKHR;
#endif /* defined(VK_KHR_video_decode_queue) */
#if defined(VK_KHR_video_encode_queue)
ACID_CONTEXT_EXPORT extern PFN_vkCmdEncodeVideoKHR vkCmdEncodeVideoKHR;
#endif /* defined(VK_KHR_video_encode_queue) */
#if defined(VK_KHR_video_queue)
ACID_CONTEXT_EXPORT extern PFN_vkBindVideoSessionMemoryKHR vkBindVideoSessionMemoryKHR;
ACID_CONTEXT_EXPORT extern PFN_vkCmdBeginVideoCodingKHR vkCmdBeginVideoCodingKHR;
ACID_CONTEXT_EXPORT extern PFN_vkCmdControlVideoCodingKHR vkCmdControlVideoCodingKHR;
ACID_CONTEXT_EXPORT extern PFN_vkCmdEndVideoCodingKHR vkCmdEndVideoCodingKHR;
ACID_CONTEXT_EXPORT extern PFN_vkCreateVideoSessionKHR vkCreateVideoSessionKHR;
ACID_CONTEXT_EXPORT extern PFN_vkCreateVideoSessionParametersKHR vkCreateVideoSessionParametersKHR;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyVideoSessionKHR vkDestroyVideoSessionKHR;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyVideoSessionParametersKHR vkDestroyVideoSessionParametersKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceVideoCapabilitiesKHR vkGetPhysicalDeviceVideoCapabilitiesKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceVideoFormatPropertiesKHR vkGetPhysicalDeviceVideoFormatPropertiesKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetVideoSessionMemoryRequirementsKHR vkGetVideoSessionMemoryRequirementsKHR;
ACID_CONTEXT_EXPORT extern PFN_vkUpdateVideoSessionParametersKHR vkUpdateVideoSessionParametersKHR;
#endif /* defined(VK_KHR_video_queue) */
#if defined(VK_KHR_wayland_surface)
ACID_CONTEXT_EXPORT extern PFN_vkCreateWaylandSurfaceKHR vkCreateWaylandSurfaceKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR vkGetPhysicalDeviceWaylandPresentationSupportKHR;
#endif /* defined(VK_KHR_wayland_surface) */
#if defined(VK_KHR_win32_surface)
ACID_CONTEXT_EXPORT extern PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR vkGetPhysicalDeviceWin32PresentationSupportKHR;
#endif /* defined(VK_KHR_win32_surface) */
#if defined(VK_KHR_xcb_surface)
ACID_CONTEXT_EXPORT extern PFN_vkCreateXcbSurfaceKHR vkCreateXcbSurfaceKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR vkGetPhysicalDeviceXcbPresentationSupportKHR;
#endif /* defined(VK_KHR_xcb_surface) */
#if defined(VK_KHR_xlib_surface)
ACID_CONTEXT_EXPORT extern PFN_vkCreateXlibSurfaceKHR vkCreateXlibSurfaceKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR vkGetPhysicalDeviceXlibPresentationSupportKHR;
#endif /* defined(VK_KHR_xlib_surface) */
#if defined(VK_MVK_ios_surface)
ACID_CONTEXT_EXPORT extern PFN_vkCreateIOSSurfaceMVK vkCreateIOSSurfaceMVK;
#endif /* defined(VK_MVK_ios_surface) */
#if defined(VK_MVK_macos_surface)
ACID_CONTEXT_EXPORT extern PFN_vkCreateMacOSSurfaceMVK vkCreateMacOSSurfaceMVK;
#endif /* defined(VK_MVK_macos_surface) */
#if defined(VK_NN_vi_surface)
ACID_CONTEXT_EXPORT extern PFN_vkCreateViSurfaceNN vkCreateViSurfaceNN;
#endif /* defined(VK_NN_vi_surface) */
#if defined(VK_NVX_binary_import)
ACID_CONTEXT_EXPORT extern PFN_vkCmdCuLaunchKernelNVX vkCmdCuLaunchKernelNVX;
ACID_CONTEXT_EXPORT extern PFN_vkCreateCuFunctionNVX vkCreateCuFunctionNVX;
ACID_CONTEXT_EXPORT extern PFN_vkCreateCuModuleNVX vkCreateCuModuleNVX;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyCuFunctionNVX vkDestroyCuFunctionNVX;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyCuModuleNVX vkDestroyCuModuleNVX;
#endif /* defined(VK_NVX_binary_import) */
#if defined(VK_NVX_image_view_handle)
ACID_CONTEXT_EXPORT extern PFN_vkGetImageViewAddressNVX vkGetImageViewAddressNVX;
ACID_CONTEXT_EXPORT extern PFN_vkGetImageViewHandleNVX vkGetImageViewHandleNVX;
#endif /* defined(VK_NVX_image_view_handle) */
#if defined(VK_NV_acquire_winrt_display)
ACID_CONTEXT_EXPORT extern PFN_vkAcquireWinrtDisplayNV vkAcquireWinrtDisplayNV;
ACID_CONTEXT_EXPORT extern PFN_vkGetWinrtDisplayNV vkGetWinrtDisplayNV;
#endif /* defined(VK_NV_acquire_winrt_display) */
#if defined(VK_NV_clip_space_w_scaling)
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetViewportWScalingNV vkCmdSetViewportWScalingNV;
#endif /* defined(VK_NV_clip_space_w_scaling) */
#if defined(VK_NV_cooperative_matrix)
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV vkGetPhysicalDeviceCooperativeMatrixPropertiesNV;
#endif /* defined(VK_NV_cooperative_matrix) */
#if defined(VK_NV_coverage_reduction_mode)
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV;
#endif /* defined(VK_NV_coverage_reduction_mode) */
#if defined(VK_NV_device_diagnostic_checkpoints)
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetCheckpointNV vkCmdSetCheckpointNV;
ACID_CONTEXT_EXPORT extern PFN_vkGetQueueCheckpointDataNV vkGetQueueCheckpointDataNV;
#endif /* defined(VK_NV_device_diagnostic_checkpoints) */
#if defined(VK_NV_device_generated_commands)
ACID_CONTEXT_EXPORT extern PFN_vkCmdBindPipelineShaderGroupNV vkCmdBindPipelineShaderGroupNV;
ACID_CONTEXT_EXPORT extern PFN_vkCmdExecuteGeneratedCommandsNV vkCmdExecuteGeneratedCommandsNV;
ACID_CONTEXT_EXPORT extern PFN_vkCmdPreprocessGeneratedCommandsNV vkCmdPreprocessGeneratedCommandsNV;
ACID_CONTEXT_EXPORT extern PFN_vkCreateIndirectCommandsLayoutNV vkCreateIndirectCommandsLayoutNV;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyIndirectCommandsLayoutNV vkDestroyIndirectCommandsLayoutNV;
ACID_CONTEXT_EXPORT extern PFN_vkGetGeneratedCommandsMemoryRequirementsNV vkGetGeneratedCommandsMemoryRequirementsNV;
#endif /* defined(VK_NV_device_generated_commands) */
#if defined(VK_NV_external_memory_capabilities)
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceExternalImageFormatPropertiesNV vkGetPhysicalDeviceExternalImageFormatPropertiesNV;
#endif /* defined(VK_NV_external_memory_capabilities) */
#if defined(VK_NV_external_memory_rdma)
ACID_CONTEXT_EXPORT extern PFN_vkGetMemoryRemoteAddressNV vkGetMemoryRemoteAddressNV;
#endif /* defined(VK_NV_external_memory_rdma) */
#if defined(VK_NV_external_memory_win32)
ACID_CONTEXT_EXPORT extern PFN_vkGetMemoryWin32HandleNV vkGetMemoryWin32HandleNV;
#endif /* defined(VK_NV_external_memory_win32) */
#if defined(VK_NV_fragment_shading_rate_enums)
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetFragmentShadingRateEnumNV vkCmdSetFragmentShadingRateEnumNV;
#endif /* defined(VK_NV_fragment_shading_rate_enums) */
#if defined(VK_NV_mesh_shader)
ACID_CONTEXT_EXPORT extern PFN_vkCmdDrawMeshTasksIndirectCountNV vkCmdDrawMeshTasksIndirectCountNV;
ACID_CONTEXT_EXPORT extern PFN_vkCmdDrawMeshTasksIndirectNV vkCmdDrawMeshTasksIndirectNV;
ACID_CONTEXT_EXPORT extern PFN_vkCmdDrawMeshTasksNV vkCmdDrawMeshTasksNV;
#endif /* defined(VK_NV_mesh_shader) */
#if defined(VK_NV_ray_tracing)
ACID_CONTEXT_EXPORT extern PFN_vkBindAccelerationStructureMemoryNV vkBindAccelerationStructureMemoryNV;
ACID_CONTEXT_EXPORT extern PFN_vkCmdBuildAccelerationStructureNV vkCmdBuildAccelerationStructureNV;
ACID_CONTEXT_EXPORT extern PFN_vkCmdCopyAccelerationStructureNV vkCmdCopyAccelerationStructureNV;
ACID_CONTEXT_EXPORT extern PFN_vkCmdTraceRaysNV vkCmdTraceRaysNV;
ACID_CONTEXT_EXPORT extern PFN_vkCmdWriteAccelerationStructuresPropertiesNV vkCmdWriteAccelerationStructuresPropertiesNV;
ACID_CONTEXT_EXPORT extern PFN_vkCompileDeferredNV vkCompileDeferredNV;
ACID_CONTEXT_EXPORT extern PFN_vkCreateAccelerationStructureNV vkCreateAccelerationStructureNV;
ACID_CONTEXT_EXPORT extern PFN_vkCreateRayTracingPipelinesNV vkCreateRayTracingPipelinesNV;
ACID_CONTEXT_EXPORT extern PFN_vkDestroyAccelerationStructureNV vkDestroyAccelerationStructureNV;
ACID_CONTEXT_EXPORT extern PFN_vkGetAccelerationStructureHandleNV vkGetAccelerationStructureHandleNV;
ACID_CONTEXT_EXPORT extern PFN_vkGetAccelerationStructureMemoryRequirementsNV vkGetAccelerationStructureMemoryRequirementsNV;
ACID_CONTEXT_EXPORT extern PFN_vkGetRayTracingShaderGroupHandlesNV vkGetRayTracingShaderGroupHandlesNV;
#endif /* defined(VK_NV_ray_tracing) */
#if defined(VK_NV_scissor_exclusive)
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetExclusiveScissorNV vkCmdSetExclusiveScissorNV;
#endif /* defined(VK_NV_scissor_exclusive) */
#if defined(VK_NV_shading_rate_image)
ACID_CONTEXT_EXPORT extern PFN_vkCmdBindShadingRateImageNV vkCmdBindShadingRateImageNV;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetCoarseSampleOrderNV vkCmdSetCoarseSampleOrderNV;
ACID_CONTEXT_EXPORT extern PFN_vkCmdSetViewportShadingRatePaletteNV vkCmdSetViewportShadingRatePaletteNV;
#endif /* defined(VK_NV_shading_rate_image) */
#if defined(VK_QNX_screen_surface)
ACID_CONTEXT_EXPORT extern PFN_vkCreateScreenSurfaceQNX vkCreateScreenSurfaceQNX;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDeviceScreenPresentationSupportQNX vkGetPhysicalDeviceScreenPresentationSupportQNX;
#endif /* defined(VK_QNX_screen_surface) */
#if defined(VK_VALVE_descriptor_set_host_mapping)
ACID_CONTEXT_EXPORT extern PFN_vkGetDescriptorSetHostMappingVALVE vkGetDescriptorSetHostMappingVALVE;
ACID_CONTEXT_EXPORT extern PFN_vkGetDescriptorSetLayoutHostMappingInfoVALVE vkGetDescriptorSetLayoutHostMappingInfoVALVE;
#endif /* defined(VK_VALVE_descriptor_set_host_mapping) */
#if (defined(VK_EXT_full_screen_exclusive) && defined(VK_KHR_device_group)) || (defined(VK_EXT_full_screen_exclusive) && defined(VK_VERSION_1_1))
ACID_CONTEXT_EXPORT extern PFN_vkGetDeviceGroupSurfacePresentModes2EXT vkGetDeviceGroupSurfacePresentModes2EXT;
#endif /* (defined(VK_EXT_full_screen_exclusive) && defined(VK_KHR_device_group)) || (defined(VK_EXT_full_screen_exclusive) && defined(VK_VERSION_1_1)) */
#if (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor)) || (defined(VK_KHR_push_descriptor) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_push_descriptor) && defined(VK_KHR_descriptor_update_template))
ACID_CONTEXT_EXPORT extern PFN_vkCmdPushDescriptorSetWithTemplateKHR vkCmdPushDescriptorSetWithTemplateKHR;
#endif /* (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor)) || (defined(VK_KHR_push_descriptor) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_push_descriptor) && defined(VK_KHR_descriptor_update_template)) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
ACID_CONTEXT_EXPORT extern PFN_vkGetDeviceGroupPresentCapabilitiesKHR vkGetDeviceGroupPresentCapabilitiesKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetDeviceGroupSurfacePresentModesKHR vkGetDeviceGroupSurfacePresentModesKHR;
ACID_CONTEXT_EXPORT extern PFN_vkGetPhysicalDevicePresentRectanglesKHR vkGetPhysicalDevicePresentRectanglesKHR;
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
ACID_CONTEXT_EXPORT extern PFN_vkAcquireNextImage2KHR vkAcquireNextImage2KHR;
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
/* VOLK_GENERATE_PROTOTYPES_H */

#ifdef __cplusplus
}
#endif

#endif

#ifdef VOLK_IMPLEMENTATION
#undef VOLK_IMPLEMENTATION
// Prevent tools like dependency checkers that don't evaluate
// macros from detecting a cyclic dependency.
#define VOLK_SOURCE "volk.c"
#include VOLK_SOURCE
#endif

/**
 * Copyright (c) 2018-2019 Arseny Kapoulkine
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/
/* clang-format on */
