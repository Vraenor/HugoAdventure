#include "GameOverScene.h"
#include "GameScene.h"
#include "MainMenuScene.h"

USING_NS_CC;

Scene* GameOverScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = GameOverScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void GameOverScene::goToMainMenuScene(Ref *pSender) {

	auto scene = MainMenuScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene));
}

void GameOverScene::retryGameScene(Ref *pSender) {

	auto scene = GameScene::createScene("images/mapaBed.tmx", 637.5, 562.5);
	Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene));
}

// on "init" you need to initialize your instance
bool GameOverScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
   
	auto retryItem = MenuItemImage::create("images/GameOverScreen/Retry_Button.png", "images/GameOverScreen/Retry_Button(Click).png",
		CC_CALLBACK_1(GameOverScene::retryGameScene, this));

	auto menuItem = MenuItemImage::create("images/GameOverScreen/Menu_Button.png", "images/GameOverScreen/Menu_Button(Click).png",
		CC_CALLBACK_1(GameOverScene::goToMainMenuScene, this));

	auto menuTitle = MenuItemImage::create("images/GameOverScreen/Game_Over.png", "images/GameOverScreen/Game_Over.png");

	auto menu = Menu::create(menuTitle, menuItem, retryItem, NULL);
	menu->alignItemsVerticallyWithPadding(visibleSize.height / 6);
	addChild(menu, 1);

	auto background = Sprite::create("images/GameOverScreen/Game_Over_Screen_Background.png");
	background->setPosition(Point((visibleSize.width / 2), (visibleSize.height / 2)));
	addChild(background, 0);

    return true;
}