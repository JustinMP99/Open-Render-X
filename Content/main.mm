
//#include "window_test.mm"

#include <AppKit/AppKit.h>


@interface MainWindowDelegate : NSObject <NSWindowDelegate>
{
    @public
        bool *m_running;
}

- (instancetype)initWithRunning:(bool *)running;
@end

@implementation MainWindowDelegate

- (instancetype)initWithRunning:(bool *)running
{
    self = [super init];
    m_running = running;
    return self;
}

- (bool)windowShouldClose:(NSWindow *)window
{
    *m_running = false;
    return true;
}

@end

@interface baseWindow : NSWindow

@end

@implementation baseWindow

- (bool)WindowCanBecomeMainWindow
{
    return true;
}

- (bool)WindowCanBecomeKeyWindow
{
    return true;
}

@end

baseWindow *CreateMainWindow(bool *running)
{
    NSRect frame = NSMakeRect(0, 0, 800, 600);
    NSUInteger style = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable;
    baseWindow *window = [[baseWindow alloc] initWithContentRect:frame styleMask:style backing:NSBackingStoreBuffered defer:NO];
    MainWindowDelegate *delegate = [[MainWindowDelegate alloc] initWithRunning:running];
    [window setTitle:@"Basic MacOS Window"];
    [window setDelegate:delegate];
    [window makeKeyAndOrderFront:nil];
    return window;
}


// Instructions on how to build & run:
// 1. Open Terminal and navigate to the project directory.
// 2. Run the build script: ./build.sh
// 3. After the build completes, run the generated executable: ./Build/window

int main()
{

    bool running = true;

    NSApplication *app = [NSApplication sharedApplication];
    [app setActivationPolicy:NSApplicationActivationPolicyRegular];
    [app setPresentationOptions:NSApplicationPresentationAutoHideDock | NSApplicationPresentationAutoHideMenuBar];
    [app finishLaunching];
    
    baseWindow *mainWindow = CreateMainWindow(&running);
    
    NSLog(@"Hello, World!"); // This will print to the console when the application is launched. Equivalent to printf in C/C++.

    while (running) 
    {
        @autoreleasepool 
        {
            NSEvent *event = [app nextEventMatchingMask:NSEventMaskAny untilDate:[NSDate distantFuture] inMode:NSDefaultRunLoopMode dequeue:YES];
            if (event)
            {
                [app sendEvent:event];
            }
        }
    }

    return 0;
}


