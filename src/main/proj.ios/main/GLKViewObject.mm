#import "glkit/glkview.h"
#import "GLKViewObject.H"

@implementation GLKViewObject

// C "trampoline" function to invoke Objective-C method
void bindFrameBuffer(void *param)
{
    // Call the Objective-C method using Objective-C syntax
    [(__bridge GLKView*)param bindDrawable];
}

@end