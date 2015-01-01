#include "core/core.h"

#include "oglrenderer/ogl.h"
#include "oglrenderer/ogldefine.h"
#include "oglrenderer/ogldevice.h"
#include "oglrenderer/oglgeobuffer.h"
#include "oglrenderer/oglinputlayout.h"
#include "oglrenderer/ogltexture.h"
#include "oglrenderer/oglmaterialtemplate.h"
#include "oglrenderer/oglwindowtarget.h"
#include "oglrenderer/oglrendertarget.h"
#include "oglrenderer/oglrenderer.h"

#include "engine/engine.h"
#include "engine/resource.h"
#include "engine/texture.h"
#include "engine/material.h"
#include "engine/meshresource.h"
#include "engine/rendercontext.h"
#include "engine/utilityrenderer.h"
#include "engine/scenenode.h"
#include "engine/cameranode.h"
#include "engine/lightnode.h"
#include "engine/rendernode.h"
#include "engine/postprocess.h"
#include "engine/postprocessdof.h"
#include "engine/scene.h"
#include "engine/cloth.h"
#include "engine/gridnode.h"
#include "engine/meshnode.h"

#include "iosapp.h"

#import "Foundation/NSString.h"
#import "Foundation/NSBundle.h"
#import "ViewController.h"

@interface ViewController () {
 
}

@property (strong, nonatomic) EAGLContext *context;
- (void)setupGL;
- (void)tearDownGL;
@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    //view.drawableMultisample = GLKViewDrawableMultisample4X;
    
    [self setupGL];
}

- (void)dealloc
{    
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];

    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;
        
        [self tearDownGL];
        
        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }

    // Dispose of any resources that can be recreated.
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];

    NSString *resPath = [[NSBundle mainBundle] resourcePath];
    const char *resCPath = [resPath UTF8String];
    char *rootPath = new char[512];
    strcpy(rootPath, resCPath);
    strcat(rootPath, "/res/");

    FileUtils::Instance()->SetResourceRoot(rootPath);
    int width = self.view.bounds.size.width*2;
    int height = self.view.bounds.size.height*2;
    Application::GetInstance().InitRenderer(width, height, (__bridge void*)self.view);
    Application::GetInstance().InitScene();

}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
    Application::GetInstance().UpdateFrame();
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
}

@end
