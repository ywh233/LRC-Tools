//
//  AppDelegate.h
//  CMUS Lyrics Plugin
//
//  Created by Yuwei Huang on 14-3-5.
//  Copyright (c) 2014年 Yuwei Huang. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "Lyrics.h"
#import "CmusStatus.h"

@interface AppDelegate : NSObject <NSApplicationDelegate> {
	
	Lyrics* lrc;
	CmusStatus* status;
	NSString* lrcFilename;
	NSUserDefaults* defaults;
	
	NSString* cmusRemotePath;
	NSString* lyricsPath;
	
	NSThread* thread;
	
	float period;
	
	BOOL notFoundFlag;
	BOOL topMostFlag;
}

@property (assign) IBOutlet NSWindow *window;
@property (assign) IBOutlet NSWindow *settingWindow;

@property (assign) IBOutlet NSTextField* tfp2;
@property (assign) IBOutlet NSTextField* tfp1;
@property (assign) IBOutlet NSTextField* tfMain;
@property (assign) IBOutlet NSTextField* tfn1;
@property (assign) IBOutlet NSTextField* tfn2;

@property (assign) IBOutlet NSTextField* cmusPathField;
@property (assign) IBOutlet NSTextField* lyricsPathField;
@property (assign) IBOutlet NSTextField* periodField;

@property (assign) IBOutlet NSButton* topmostCheckbox;

-(IBAction) openPrefWindow:(id)sender;
-(IBAction) closePrefWindow:(id)sender;
-(IBAction) closeAndSavePrefWindow:(id)sender;

-(void) toggleTopmostMode;
@end
