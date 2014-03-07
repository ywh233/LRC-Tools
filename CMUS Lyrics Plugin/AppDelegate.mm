//
//  AppDelegate.m
//  CMUS Lyrics Plugin
//
//  Created by Yuwei Huang on 14-3-5.
//  Copyright (c) 2014年 Yuwei Huang. All rights reserved.
//

#define DEFAULT_CMUS_REMOTE_PATH @"/usr/local/bin/cmus-remote"
#define DEFAULT_LYRICS_PATH @"/"
#define DEFAULT_PERIOD 0.5f

#import "AppDelegate.h"

#include <stdlib.h>

@implementation AppDelegate

@synthesize tfMain,tfn1,tfn2,tfp1,tfp2,window,settingWindow,cmusPathField,lyricsPathField,periodField,topmostCheckbox;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	cmusRemotePath = [[NSUserDefaults standardUserDefaults] stringForKey:@"cmus-remote-path"];
	lyricsPath = [[NSUserDefaults standardUserDefaults] stringForKey:@"lyrics-path"];
	period = [[NSUserDefaults standardUserDefaults] floatForKey:@"period"];
	
	int windowX = (int)[[NSUserDefaults standardUserDefaults] integerForKey:@"posx"];
	int windowY = (int)[[NSUserDefaults standardUserDefaults] integerForKey:@"posy"];
	
	topMostFlag = [[NSUserDefaults standardUserDefaults] boolForKey:@"topmost"];
	
	if (cmusRemotePath == nil)
		cmusRemotePath = DEFAULT_CMUS_REMOTE_PATH;
	
	if (period < 0.001)
		period = DEFAULT_PERIOD;
		
	if (windowX != 0 || windowY != 0)
		[window setFrameOrigin:NSMakePoint(windowX, windowY)];
	
	if (lyricsPath == nil) {
		lyricsPath = DEFAULT_LYRICS_PATH;
		
		topMostFlag = true;
		
		[self openPrefWindow:nil];
	}
	
	[self updateTopmostMode];
	
	lrc = new Lyrics();
	status = new CmusStatus(cmusRemotePath.UTF8String);
	
	thread = [[NSThread alloc] initWithTarget:self selector:@selector(backgroundWork) object:nil];
	[thread start];
}

- (BOOL)windowWillClose:(NSNotification*)sender{
	if ([sender object] == window) {
		
		[[NSUserDefaults standardUserDefaults] setInteger:window.frame.origin.x forKey:@"posx"];
		[[NSUserDefaults standardUserDefaults] setInteger:window.frame.origin.y forKey:@"posy"];
		[[NSUserDefaults standardUserDefaults] synchronize];
		
		[thread cancel];
		
		delete lrc;
		delete status;
		
		exit(0);
	}
	return true;
}

- (void)backgroundWork {
	while(true) {
		if (!status->refreshStatus()) {
			[self fadeOutWindow:window];
			[NSThread sleepForTimeInterval:period];
			continue;
		}
		
		NSString *statusStr = [NSString stringWithUTF8String:status->getItem("status").c_str()];
		
		if (statusStr!=nil && ![statusStr isEqualToString:@"playing"]) {
			[self fadeOutWindow:window];
			[NSThread sleepForTimeInterval:period];
			continue;
		}
		
		NSString* filename = [[[[NSString stringWithUTF8String:status->getItem("file").c_str()]
			lastPathComponent] stringByDeletingPathExtension] stringByAppendingString:@".lrc"];
		filename = [[lyricsPath stringByAppendingString:@"/"] stringByAppendingString:filename];
		
		if (([lrcFilename compare:filename] == NSOrderedSame && notFoundFlag) // do one less test
		 || ![[NSFileManager defaultManager] fileExistsAtPath:filename isDirectory:nil]) {
			[self fadeOutWindow:window];
			[NSThread sleepForTimeInterval:period];
			lrcFilename = filename;
			notFoundFlag = true;
			continue;
		}
		
		[self fadeInWindow:window];
		
		if (lrcFilename== nil || [lrcFilename compare:filename]!=NSOrderedSame) {
			NSString *lrcText = [NSString stringWithContentsOfFile:filename
				encoding:NSUTF8StringEncoding error:nil];
			lrc->reset(lrcText.UTF8String);
			lrcFilename = filename;
			notFoundFlag = false;
			
			// change window title
			NSString* title = [NSString stringWithUTF8String: status->getItem("tag title").c_str()];
			NSString* artist = [NSString stringWithUTF8String: status->getItem("tag artist").c_str()];
		
			window.title = [NSString stringWithFormat:@"%@ - %@", title, artist];
		}
		
		
		int sec = atoi(status->getItem("position").c_str());
		int index = lrc->getIndexOfLyrics(sec*1000);
		
		if (index == -1) { // no lyrics at this time
			tfMain.stringValue = @"";
			tfn1.stringValue = @"";
			tfn2.stringValue = @"";
			tfp1.stringValue = @"";
			tfp2.stringValue = @"";
		} else {
		
			tfMain.stringValue = [NSString stringWithUTF8String:lrc->getLyricsAtIndex(index).c_str()];
			tfn1.stringValue = [NSString stringWithUTF8String:lrc->getLyricsAtIndex(index+1).c_str()];
			tfn2.stringValue = [NSString stringWithUTF8String:lrc->getLyricsAtIndex(index+2).c_str()];
			tfp1.stringValue = [NSString stringWithUTF8String:lrc->getLyricsAtIndex(index-1).c_str()];
			tfp2.stringValue = [NSString stringWithUTF8String:lrc->getLyricsAtIndex(index-2).c_str()];
		}
		
		[NSThread sleepForTimeInterval:period];
	}
}

- (void)fadeOutWindow:(NSWindow*)wd{
	if (wd.isVisible)
		[wd orderOut:nil];
/*
	if (!wd.isVisible || wd.alphaValue < 0.1)
		return;
		
	float alpha = 1.0;
	[wd setAlphaValue:alpha];
	//[wd makeKeyAndOrderFront:self];
	for (int x = 0; x < 10; x++) {
		alpha -= 0.1;
		[wd setAlphaValue:alpha];
		[NSThread sleepForTimeInterval:0.020];
	}
	[wd orderOut:nil];
	*/
}

- (void)fadeInWindow:(NSWindow*)wd{
	if (!wd.isVisible)
		[wd makeKeyAndOrderFront:nil];
/*
	if (!wd.isVisible)
		[wd makeKeyAndOrderFront:nil];
	if (wd.alphaValue > 0.9)
		return;
	float alpha = 0.0;
	[wd setAlphaValue:alpha];
	[wd makeKeyAndOrderFront:self];
	for (int x = 0; x < 10; x++) {
		alpha += 0.1;
		[wd setAlphaValue:alpha];
		[NSThread sleepForTimeInterval:0.020];
	}
	*/
}

-(IBAction) openPrefWindow:(id)sender {
	cmusPathField.stringValue = cmusRemotePath;
	lyricsPathField.stringValue = lyricsPath;
	periodField.floatValue = period;
	topmostCheckbox.state = topMostFlag?NSOnState:NSOffState;
	[settingWindow makeKeyAndOrderFront:nil];
}

-(IBAction) closePrefWindow:(id)sender {
	[settingWindow orderOut:nil];
}

-(IBAction)closeAndSavePrefWindow:(id)sender {
	cmusRemotePath = cmusPathField.stringValue;
	lyricsPath = lyricsPathField.stringValue;
	period = periodField.floatValue;
	topMostFlag = (topmostCheckbox.state == NSOnState)?true:false;
	[[NSUserDefaults standardUserDefaults] setObject:cmusRemotePath forKey:@"cmus-remote-path"];
	[[NSUserDefaults standardUserDefaults] setObject:lyricsPath forKey:@"lyrics-path"];
	[[NSUserDefaults standardUserDefaults] setFloat:period forKey:@"period"];
	[[NSUserDefaults standardUserDefaults] setBool:topMostFlag forKey:@"topmost"];
	
	[[NSUserDefaults standardUserDefaults] synchronize];
	
	[self updateTopmostMode];
	[self closePrefWindow:nil];
}

-(void)updateTopmostMode {
	if (topMostFlag)
		[window setLevel: NSStatusWindowLevel];
	else
		[window setLevel:NSNormalWindowLevel];
}

-(void) toggleTopmostMode {
	topMostFlag = !topMostFlag;
	[[NSUserDefaults standardUserDefaults] setBool:topMostFlag forKey:@"topmost"];
	[self updateTopmostMode];
}

@end