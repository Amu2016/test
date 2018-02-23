#include "HelloWorldScene.h"

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
							origin.y + visibleSize.height - closeItem->getContentSize().height / 2));

    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);


	float layerWidth = 868;
	float layerHeight = 680;

	auto layer = DrawLayer::create(Color4B(255, 255, 255, 255), layerWidth, layerHeight);
	layer->setPosition(origin.x, origin.y);
	this->addChild(layer, 1);
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
}
