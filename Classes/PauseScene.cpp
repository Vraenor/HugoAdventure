#include "PauseScene.h"
#include "MainMenuScene.h"
#include "GameScene.h"

USING_NS_CC;

Scene* PauseScene::createScene(const std::string& File, float x, float y)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = PauseScene::create(File, x, y);

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

PauseScene* PauseScene::create(const std::string& File, float x, float y) {

	PauseScene *pRet = new PauseScene();

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

void PauseScene::resumeGameScene(Ref *pSender) {
	Director::getInstance()->popScene();
}
void PauseScene::goToMainMenu(Ref *pSender) {
	auto scene = MainMenuScene::createScene();
	Director::getInstance()->popScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene));
}
void PauseScene::retryGameScene(Ref *pSender) {
	auto scene = GameScene::createScene(curMap, curX, curY); //(curMap, curX, curY)
	Director::getInstance()->popScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene));
}

// on "init" you need to initialize your instance
bool PauseScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
   
	auto resumeItem = MenuItemImage::create("images/PauseScreen/Resume_Button.png", "images/PauseScreen/Resume_Button(Click).png", 
		CC_CALLBACK_1(PauseScene::resumeGameScene, this));

	auto replayItem = MenuItemImage::create("images/PauseScreen/Retry_Button.png", "images/PauseScreen/Retry_Button(Click).png",
		CC_CALLBACK_1(PauseScene::retryGameScene, this));

	auto menuItem = MenuItemImage::create("images/PauseScreen/Menu_Button.png", "images/PauseScreen/Menu_Button(Click).png",
		CC_CALLBACK_1(PauseScene::goToMainMenu, this));

	auto menu = Menu::create(resumeItem, replayItem, menuItem, NULL);
	menu->alignItemsVerticallyWithPadding(visibleSize.height / 10);
	addChild(menu, 1);

	auto background = Sprite::create("images/PauseScreen/Pause_Background.png");
	background->setPosition(visibleSize.height / 2, visibleSize.width / 2);
	addChild(background, 0);

    return true;
}