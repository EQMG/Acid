/* -*- Mode: c++; indent-tabs-mode: nil; tab-width: 40; c-basic-offset: 4 -*- */

#ifndef __emscripten_vr_h__
#define __emscripten_vr_h__

#include <stdint.h>
#include <stdbool.h>

/*
 * This file provides some basic interfaces for interacting with WebVR from Emscripten.
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef int32_t WebVRDeviceId;
typedef int32_t WebVRHardwareUnitId;

typedef enum {
    WebVREyeLeft = 0,
    WebVREyeRight = 1
} WebVREye;

typedef enum {
    WebVRUnknownDevice = 0,
    WebVRHMDDevice = 1,
    WebVRPositionSensorDevice = 2
} WebVRDeviceType;

typedef struct WebVRFieldOfView {
    double upDegrees;
    double rightDegrees;
    double downDegrees;
    double leftDegrees;
} WebVRFieldOfView;

typedef struct WebVRPoint {
    double x, y, z, w;
} WebVRPoint;

typedef struct WebVRIntRect {
    int32_t x, y;
    uint32_t width, height;
} WebVRIntRect;

typedef struct WebVRPositionState {
    double timeStamp;

    int hasPosition;
    WebVRPoint position;
    WebVRPoint linearVelocity;
    WebVRPoint linearAcceleration;

    int hasOrientation;
    WebVRPoint orientation;
    WebVRPoint angularVelocity;
    WebVRPoint angularAcceleration;
} WebVRPositionState;

typedef struct WebVREyeParameters {
    WebVRFieldOfView minimumFieldOfView;
    WebVRFieldOfView maximumFieldOfView;
    WebVRFieldOfView recommendedFieldOfView;
    WebVRPoint eyeTranslation;

    WebVRFieldOfView currentFieldOfView;
    WebVRIntRect renderRect;
} WebVREyeParameters;

extern void emscripten_vr_init();
extern int emscripten_vr_ready();

extern int emscripten_vr_count_devices();
extern WebVRDeviceId emscripten_vr_get_device_id(int deviceIndex);
extern WebVRDeviceType emscripten_vr_get_device_type(WebVRDeviceId deviceId);
extern WebVRHardwareUnitId emscripten_vr_get_device_hwid(WebVRDeviceId deviceId);
extern char *emscripten_vr_get_device_name(WebVRDeviceId deviceId);

// select the given device as the fullscreen HMD device
extern int emscripten_vr_select_hmd_device(WebVRDeviceId deviceId);
extern WebVRDeviceId emscripten_vr_get_selected_hmd_device();

// For HMD devices.
// All of these return 1 on success, <= 0 on failure.
extern int emscripten_vr_hmd_get_eye_parameters(WebVRDeviceId deviceId, WebVREye whichEye, WebVREyeParameters *eyeParams);
extern int emscripten_vr_hmd_set_fov(WebVRDeviceId deviceId,
                                     const WebVRFieldOfView *leftFov,
                                     const WebVRFieldOfView *rightFov,
                                     double zNear, double zFar);


// for Position devices
// All of these return 1 on success, <= 0 on failure.
extern int emscripten_vr_sensor_get_state(WebVRDeviceId deviceId, bool immediate, WebVRPositionState *state);
extern int emscripten_vr_sensor_zero(WebVRDeviceId deviceId);

#ifdef __cplusplus
} // ~extern "C"
#endif

#endif
