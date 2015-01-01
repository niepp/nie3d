#pragma once
#ifndef __GLKVIEW_OBJECT_INTERFACE_H__
#define __GLKVIEW_OBJECT_INTERFACE_H__

// This is the C "trampoline" function that will be used
// to invoke a specific Objective-C method FROM C++
void bindFrameBuffer(void *parameter);
#endif // __GLKVIEW_OBJECT_INTERFACE_H__