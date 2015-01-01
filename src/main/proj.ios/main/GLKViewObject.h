#import "core/ios/GLKViewObjectInterface.h"

// An Objective-C class that needs to be accessed from C++
@interface GLKViewObject : NSObject
{
    
}

// The Objective-C member function you want to call from C++
- (void) bindFrameBuffer:(void *) param;
@end