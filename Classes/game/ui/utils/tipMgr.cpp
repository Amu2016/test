#include"tipMgr.h"


void TipMgr::ShowNotic(const std::string& text) {
	auto tip = getAtip();
	tip->textLabel->setPosition(Vec2(Director::getInstance()->getWinSize().width / 2, Director::getInstance()->getWinSize().height / 2));

	auto moveUp = MoveBy::create(1, Vec2(0, 200));
	auto action = Sequence::create(moveUp, [=](void) {
		tip->isShow = false;
		tip->textLabel->setVisible(false);
		tip->textLabel->removeFromParent();
	});

	tip->textLabel->setText(text);
	tip->textLabel->runAction(action);
	tip->isShow = true;
	tip->textLabel->setVisible(true);
}