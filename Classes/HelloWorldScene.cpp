#include "HelloWorldScene.h"
#include"game\scene\control\scene_layer_mgr.h"
#include"game\scene\control\scene_object_mgr.h"
#include"net/MsgMgr.h"


#include"bean/test.pb.h"


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

	SceneLayerMgr::getInstance()->Init(this);

	unsigned short part = 8888;

	//MsgMgr::getInstance()->connect("192.168.51.171", part, [=](void) {
	//	float layerWidth = 868;
	//	float layerHeight = 680;
	//	auto layer = DrawLayer::create(Color4B(255, 255, 255, 255), layerWidth, layerHeight);
	//	layer->setPosition(origin.x, origin.y);
	//	//this->addChild(layer);
	//	SceneLayerMgr::getInstance()->addLayerMap(layer);
	//});

	float layerWidth = 868;
	float layerHeight = 680;
	auto layer = DrawLayer::create(Color4B(255, 255, 255, 255), layerWidth, layerHeight);
	layer->setPosition(origin.x, origin.y);
	//this->addChild(layer);
	SceneLayerMgr::getInstance()->addLayerMap(layer);



	scheduleUpdate();
    return true;
}

void HelloWorld::update(float delta){
	SceneLayerMgr::getInstance()->update(delta);
	SceneObjectMgr::getInstance()->update(delta);
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
}
