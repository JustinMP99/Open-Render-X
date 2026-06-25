//#pragma once
//#ifndef WINDOW_TEST_H
//#define WINDOW_TEST_H
//#include <AppKit/AppKit.h>
//#endif
//
//
//@interface MainWindowDelegate : NSObject <NSWindowDelegate>
//{
//    @public
//        bool *m_running;
//}
//
//- (instancetype)initWithRunning:(bool *)running;
//@end
//
//@implementation MainWindowDelegate
//
//- (instancetype)initWithRunning:(bool *)running
//{
//    self = [super init];
//    m_running = running;
//    return self;
//}
//
//- (bool)windowShouldClose:(NSWindow *)window
//{
//    *m_running = false;
//    return true;
//}
//
//@end
//
//@interface baseWindow : NSWindow
//
//@end
//
//@implementation baseWindow
//
//- (bool)WindowCanBecomeMainWindow
//{
//    return true;
//}
//
//- (bool)WindowCanBecomeKeyWindow
//{
//    return true;
//}
//
//@end
//
//baseWindow *CreateMainWindow(bool *running)
//{
//    NSRect frame = NSMakeRect(0, 0, 800, 600);
//    NSUInteger style = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable;
//    baseWindow *window = [[baseWindow alloc] initWithContentRect:frame styleMask:style backing:NSBackingStoreBuffered defer:NO];
//    MainWindowDelegate *delegate = [[MainWindowDelegate alloc] initWithRunning:running];
//    [window setTitle:@"Basic MacOS Window"];
//    [window setDelegate:delegate];
//    [window makeKeyAndOrderFront:nil];
//    return window;
//}
