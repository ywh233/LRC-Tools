//
//  LyricsClassTest.m
//  CMUS Lyrics Plugin
//
//  Created by Yuwei Huang on 14-3-7.
//  Copyright (c) 2014年 Yuwei Huang. All rights reserved.
//

#import <XCTest/XCTest.h>
#include "Lyrics.h"

@interface LyricsClassTest : XCTestCase

@end

@implementation LyricsClassTest

Lyrics* lrc;

- (void)setUp
{
    [super setUp];
    lrc = new Lyrics();
	// Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown
{
	delete lrc;
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testTimeAccuracy
{
	lrc->reset("[02:44.22]谁能够厮守永远\n[03:29.05]你总会活於老片断");
	std::string line = lrc->getLyrics(164220);
	
	if (line.compare("谁能够厮守永远")!=0)
		XCTFail(@"Error, it returns: %s .",line.c_str());
	
	line = lrc->getLyrics(165500);
	if (line.compare("谁能够厮守永远")!=0)
		XCTFail(@"Error, it returns: %s .",line.c_str());
		
	line = lrc->getLyrics(209050);
	if (line.compare("你总会活於老片断")!=0)
		XCTFail(@"Error, it returns: %s .",line.c_str());
	
	if (lrc->getLyrics(841648648).compare("")!=0)
		XCTFail(@"Error");
}

- (void)testSeveralTimeTags
{
	lrc->reset("[00:12.00][01:18.24][02:31.44][03:33.29]陈奕迅-白玫瑰\n[01:06.67]一撮玫瑰无疑心的丧礼");
	std::string l1 = lrc->getLyrics(12000);
	std::string l2 = lrc->getLyrics(151440);
	std::string l3 = lrc->getLyrics(213290);
	
	if (l1.compare(l2)!=0 || l2.compare(l3)!=0)
		XCTFail(@"Error");
}

- (void)testFileInfo
{
	lrc->reset("[ti:オリオンの夢]\n[ar:GUMI]\n[al:nicovideo sm22013760]");
	if (lrc->getTitle().compare("オリオンの夢")!=0)
		XCTFail(@"Returned wrong title");
	
	if (lrc->getArtist().compare("GUMI")!=0)
		XCTFail(@"Returned wrong artist");
	
	if (lrc->getAlbum().compare("nicovideo sm22013760")!=0)
		XCTFail(@"Returned wrong album");
}

- (void)testExpansion
{
	lrc->reset("[00:01.12]A\n[09:00.00]B\n[30:00.00]C");
	if (lrc->getLyrics(60000).compare("A")!=0)
		XCTFail("Error");
		
	if (lrc->getLyrics(540000).compare("B")!=0)
		XCTFail("Error");
	
	if (lrc->getLyrics(1800000).compare("C")!=0)
		XCTFail("Error");
}

- (void)testIncorrectInput
{
	lrc->reset("aweoghres;kt'ewrmfgresahw'atjgiryhngbf v \naweg\naewtiohjpnjj rea\naewirth94o3uebrhfvb4 38t\n");
	lrc->getLyrics(1565);
}

- (void)testLinearize
{
	lrc->reset("[03:10.23][03:25.24]AA\n[03:20.96]BB\n[03:28.53]CC", true);
	
	if (lrc->getIndexOfLyrics(190230)!=0)
		XCTFail("Returned wrong index: %d", lrc->getIndexOfLyrics(190230));
	
	if (lrc->getIndexOfLyrics(200960)!=1)
		XCTFail("Returned wrong index: %d", lrc->getIndexOfLyrics(200960));
		
	if (lrc->getIndexOfLyrics(205240)!=2)
		XCTFail("Returned wrong index: %d", lrc->getIndexOfLyrics(205240));
		
	if (lrc->getIndexOfLyrics(208530)!=3)
		XCTFail("Returned wrong index: %d", lrc->getIndexOfLyrics(208530));
}

@end
