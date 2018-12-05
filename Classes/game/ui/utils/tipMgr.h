#pragma once

#include"stdafx.h"
#include"ui/UIText.h"
#include"game/scene/control/scene_layer_mgr.h"
using namespace ui;

struct TipText {
	TipText() 
	{
		textLabel = Text::create();
		textLabel->retain();
		isShow = false;
	}

	~TipText() {
		textLabel->release();
	}

	Text* textLabel;
	bool isShow;
};

class TipMgr {
public:
	static TipMgr* getInstance() 
	{
		static TipMgr* tipMgr = new TipMgr();
		return tipMgr;
	}

	void ShowNotic(const std::string& text);

private:
	TipMgr()
		:tipList(new TipText*[m_MaxLength]),
		m_MaxLength(50)

	{
	}

	TipText* getAtip() {
		for (int i = 0; i < m_MaxLength; i++) {
			if (!tipList[i]) {
				tipList[i] = new TipText();
				tipList[i]->textLabel->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2, Director::getInstance()->getWinSize().height / 2));
			}
			if(!tipList[i]->textLabel->getParent())
				SceneLayerMgr::getInstance()->addTop(tipList[i]->textLabel);
			if (tipList[i]->isShow == false)
				return tipList[i];
		}
	}

	TipText** tipList;
	int m_MaxLength;
};