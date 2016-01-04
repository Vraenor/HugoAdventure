#include "Instructions.h"
#include "GameScene.h"
#include "MainMenuScene.h"

USING_NS_CC;

Scene* Instructions::createScene(const std::string& File, float x, float y)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = Instructions::create(File, x, y);

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

Instructions* Instructions::create(const std::string& File, float x, float y) {

	Instructions *pRet = new Instructions();

	pRet->curMap = File;
	pRet->curX = x;
	pRet->curY = y;

	if (pRet && pRet->init())
	{

		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}

}

void Instructions::retryGameScene(Ref *pSender) {

	auto scene = GameScene::createScene(curMap, curX, curY); //(curMap, curX, curY)
	Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene));
}

// on "init" you need to initialize your instance
bool Instructions::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
   
	auto retryItem = MenuItemImage::create("images/Instructions/Retry_Button.png", "images/Instructions/Retry_Button(Click).png",
		CC_CALLBACK_1(Instructions::retryGameScene, this));

	auto menuTitle = MenuItemImage::create("images/Instructions/Instructions.png", "images/Instructions/Instructions.png");

	auto menu = Menu::create(menuTitle, retryItem, NULL);
	menu->alignItemsVerticallyWithPadding(visibleSize.height / 6);
	addChild(menu, 1);

	auto background = Sprite::create("images/Instructions/Instructions_Background.png");
	background->setPosition(Point((visibleSize.width / 2), (visibleSize.height / 2)));
	addChild(background, 0);

    return true;
}