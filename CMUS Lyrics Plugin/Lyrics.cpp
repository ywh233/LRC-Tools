//
//  Lyrics.cpp
//  CMUS Lyrics Plugin
//
//  Created by Yuwei Huang on 14-3-5.
//  Copyright (c) 2014年 Yuwei Huang. All rights reserved.
//

#include "Lyrics.h"
#include <string>
#include <regex>

// extends former lyrics index and decrement.
// [0, 0, 1, 0, 0, 2, 3, 0, 4] => [-1, -1, 0, 0, 0, 1, 2, 2, 3]
void prefixExtension(int* arr, int length);

void emptyMap(int* map, int length);

Lyrics::Lyrics() {
	title = "";
	artist = "";
	album = "";
	offset = 0;
	
	timeMap = (int*)calloc(TIME_MAP_SIZE, sizeof(int));
	lyricsMap.reserve(LRC_MAP_SIZE);
}

Lyrics::Lyrics(std::string lyrics) {
	timeMap = (int*)calloc(TIME_MAP_SIZE, sizeof(int));
	lyricsMap.reserve(LRC_MAP_SIZE);
	reset(lyrics);
}

Lyrics::~Lyrics() {
	free(timeMap);
	lyricsMap.clear();
}

void Lyrics::reset(std::string lyrics) {
	emptyMap(timeMap, lastTmIndex + 1); // empty case: timeMap[0] = 0
	
	lastTmIndex = 0;
	
	lyricsMap.clear();
	
	title = "";
	artist = "";
	album = "";
	offset = 0;
	constructArrayFromLrc(lyrics);
}

void Lyrics::constructArrayFromLrc(std::string lyrics) {

	std::smatch match;
	std::regex exp("(\\[.+\\])+([^\\[\\n]*)(\\n|$)"); // match a line

	std::regex timeExp("\\[([^:]+):([^\\]]+)\\]"); // match [(xxx):(xxx)]
	std::regex numExp("[0-9]+");

	int lmIndex = 0;
	
	while(std::regex_search(lyrics, match, exp)) { // find lines
		
		
		std::string times = match[1]; // [...][...][...]
		std::string other = match[2]; // blah blah blah
		
		std::smatch timeMatch;
		
		bool isLyrics = false;
		
		while (std::regex_search(times, timeMatch, timeExp)) {
			std::string left = timeMatch[1]; //[(...):...]
			std::string right = timeMatch[2]; //[...:(...)]
			if (std::regex_match(left, numExp)) { // is a time, have lyrics
				isLyrics = true;
				int centiTime = atoi(left.c_str()) * 6000;
				centiTime += (int) (atof(right.c_str()) * 100.f);
				
				timeMap[centiTime] = lmIndex + 1; // +1, escape 0 index
				lastTmIndex = (lastTmIndex>centiTime)?lastTmIndex:centiTime;
			} else {
				setMetaInfo(left, right);
			}
			
			times = timeMatch.suffix().str();
		}
		
		if (isLyrics) {
			lyricsMap.push_back(other);
			lmIndex++;
		}

		lyrics = match.suffix().str();
	}
	
	prefixExtension(timeMap, lastTmIndex);
}

void Lyrics::setMetaInfo(std::string field, std::string data) {
	if (field.compare("ti") == 0)
		title = data;
	else if (field.compare("ar") == 0)
		artist = data;
	else if (field.compare("al") == 0)
		album = data;
	else if (field.compare("offset") == 0)
		offset = atoi(data.c_str());
}

std::string Lyrics::getLyrics(int millisecond) {
	if (lyricsMap.empty())
		return "";
	return getLyricsAtIndex(getIndexOfLyrics(millisecond));
}

std::string Lyrics::getLyricsAtIndex(int index) {
	if (lyricsMap.empty() || index < 0 || index >= lyricsMap.size())
		return "";
	return lyricsMap[index];
}

int Lyrics::getIndexOfLyrics(int millisecond) {
	if (timeMap == nullptr)
		return -1;
		
	int centisecond = (millisecond - offset) / 10;
	if (centisecond<0 || centisecond > lastTmIndex)
		return -1;
	
	return timeMap[centisecond];
	
}

std::string Lyrics::getTitle() {
	return title;
}
std::string Lyrics::getArtist() {
	return artist;
}
std::string Lyrics::getAlbum() {
	return album;
}
int Lyrics::getOffset() {
	return offset;
}

void prefixExtension(int* arr, int length) {
	for (int i = 1; i < length; i++) {
		if (arr[i] == 0)
			arr[i] = arr[i-1];
		else
			arr[i]--;
	}
}

void emptyMap(int* map, int length) {
	for (int i = 0; i < length; i++)
		map[i] = 0;
}