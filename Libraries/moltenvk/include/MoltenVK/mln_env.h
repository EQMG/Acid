/*
 * mln_env.h
 *
 * Copyright (c) 2014-2017 The Brenwill Workshop Ltd. All rights reserved.
 * http://www.brenwill.com
 *
 * Use of this document is governed by the Molten License Agreement, as included
 * in the Molten distribution package. CAREFULLY READ THAT LICENSE AGREEMENT BEFORE
 * READING AND USING THIS DOCUMENT. BY READING OR OTHERWISE USING THIS DOCUMENT,
 * YOU ACCEPT AND AGREE TO BE BOUND BY THE TERMS AND CONDITIONS OF THAT LICENSE
 * AGREEMENT. IF YOU DO NOT ACCEPT THE TERMS AND CONDITIONS OF THAT LICENSE AGREEMENT,
 * DO NOT READ OR USE THIS DOCUMENT.
 */


#ifndef __mlnEnv_h_
#define __mlnEnv_h_ 1

#ifdef __cplusplus
extern "C" {
#endif	//  __cplusplus


#include <Availability.h>

/** Constructs the Molten version number from its components. */
#define MLN_MAKE_VERSION(major, minor, patch, build)	((major << 24) | (minor << 16) | (patch << 8) | build)

/**
 * The version of Molten, derived from the version format, where each of the major, minor,
 * patch, and build components of the version is allocated a single byte in the four-byte 
 * version value, in the format MjMnPtBd.
 *
 * The following examples show each of the four bytes of the version value displyed in hex format:
 *   - 0x00070001	(version 0.7.0 build 1)
 *   - 0x03010402	(version 3.1.4 build 2)
 *   - 0x040C0F17	(version 4.12.15 build 23)
 *
 * The build component is not considered part of the formal version number, but can be used
 * to differentate between several possible alpha, beta, release candidate, and final release 
 * builds for the same nominal version.
 */
#define MLN_VERSION_MAJOR   0
#define MLN_VERSION_MINOR   19
#define MLN_VERSION_PATCH   0
#define MLN_VERSION_BUILD   1
#define MLN_VERSION         MLN_MAKE_VERSION(MLN_VERSION_MAJOR, MLN_VERSION_MINOR, MLN_VERSION_PATCH, MLN_VERSION_BUILD)

/** Building for iOS. Use ifdef instead of defined() operator to allow MLN_IOS to be used in expansions */
#ifndef MLN_IOS
#	ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
#		define MLN_IOS			1
#	else
#		define MLN_IOS			0
#	endif
#endif

/** Building for macOS. Use ifdef instead of defined() operator to allow MLN_MACOS to be used in expansions */
#ifndef MLN_MACOS
#	ifdef __MAC_OS_X_VERSION_MAX_ALLOWED
#		define MLN_MACOS		1
#	else
#		define MLN_MACOS		0
#	endif
#endif


/**
 * Compiler build setting that indicates whether the architecture currently being built for supports Metal.
 *
 * By default, all 64-bit mobile architectures are compatible with Metal. You can override this
 * build setting if necessary when compiling for any 64-bit architectures that do not support Metal.
 */
#ifndef MLN_ARCH_SUPPORTS_METAL
#	if defined(__LP64__) && __LP64__
#		define MLN_ARCH_SUPPORTS_METAL	1
#	else
#		define MLN_ARCH_SUPPORTS_METAL	0
#	endif	// __LP64__
#endif		// MLN_ARCH_SUPPORTS_METAL


/** Directive to identify public symbols. */
#define MLN_PUBLIC_SYMBOL		__attribute__((visibility("default")))


#ifdef __cplusplus
}
#endif	//  __cplusplus

#endif

