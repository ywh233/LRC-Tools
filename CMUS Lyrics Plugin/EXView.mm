//
//  EXView.m
//  CMUS Lyrics Plugin
//
//  Created by Yuwei Huang on 14-3-6.
//  Copyright (c) 2014年 Yuwei Huang. All rights reserved.
//

#import "EXView.h"

@implementation EXView

@synthesize appDelegate;

- (void)rightMouseUp:(NSEvent *)theEvent {
	[appDelegate toggleTopmostMode];
}
@end
