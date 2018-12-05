#pragma once
#ifndef STDAFX_H
#define STDAFX_H

#include"cocos2d.h"
#include<vector>
#include"ui\CocosGUI.h"
#include"math.h"
#include<random>
#include<map>

//#pragma execution_character_set("utf-8")


USING_NS_CC;
using namespace std;
using namespace ui;

#define PI 3.1415926
#define BUFFER_SIZE 8192

typedef std::mt19937 RngT;


enum GameSceneObjType{
	MONSTER,
	PLAYER,
	MAINPLAYER
};



inline float getRandomFloat(RngT& rng, const float min, const float max) {
	return std::uniform_real_distribution<float>(min, max)(rng);
}

inline int getMyRandomInt(RngT& rng, const int min, const int max) {
	return std::uniform_int_distribution<int>(min, max)(rng);
}

inline Vec2 getRandomVec2Int(RngT& rng, const int min, const int max) {
	return Vec2(getMyRandomInt(rng, min, max), getMyRandomInt(rng, min, max));
}

inline float randomClamped(RngT& rng) {
	return std::uniform_real_distribution<float>(-1.0f, 1.0f)(rng);
}


//-------------------
#endif STDAFX_H