/*
 * mvk_vulkan.h
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


/** 
 * This is a convenience header file that loads vulkan.h with the appropriate MoltenVK
 * Vulkan platform surface extension automatically enabled for iOS or macOS.
 *
 * When building for iOS, this header automatically enables the VK_MVK_ios_surface Vulkan extension.
 * When building for macOS, this header automatically enables the VK_MVK_macos_surface Vulkan extension.
 *
 * Use this header file in place of the vulkan.h file, where access to the MoltenVK
 * surface extensions are required.
 *
 * Use the following form when including this header file:
 *
 *     #include <MoltenVK/mvk_vulkan.h>
 */

#ifndef __mvk_vulkan_h_
#define __mvk_vulkan_h_ 1

#include "mln_env.h"

#if MLN_IOS
#	define VK_USE_PLATFORM_IOS_MVK				1
#endif

#if MLN_MACOS
#	define VK_USE_PLATFORM_MACOS_MVK			1
#endif

#include <MoltenVK/vulkan/vulkan.h>

#endif
