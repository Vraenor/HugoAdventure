#include "Hugo.h"


USING_NS_CC;

Hugo::Hugo() {
	pulsadoE=false;
	spritebatch=nullptr;
	playerSprite = Sprite::create("images/Hugo/HugoUp_1.png");
	this->addChild(playerSprite);
}

void Hugo::animatePlayer(EventKeyboard::KeyCode keyCode) {

	if (spritebatch != nullptr)
	{
		spritebatch->removeFromParent();
	}
	spritebatch = SpriteBatchNode::create("images/Hugo/Hugo.png");
	cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("images/Hugo/Hugo.plist");

	playerSprite->setVisible(false);

	Sprite1 = Sprite::createWithSpriteFrameName("HugoUp_0.png");
	spritebatch->addChild(Sprite1);

	addChild(spritebatch);

	Vector<SpriteFrame*> animFrames(4);
	char str[15] = { 0 };

	switch (keyCode)
	{

	case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:

		for (int i = 0; i <= 3; i++)
		{
			if(empujando==true&&tirando==false) sprintf(str, "HugoEmLe_%01d.png", i);
			else if (tirando==true&& pulsadoE==true) sprintf(str, "HugoTiLe_%01d.png",i);
			else sprintf (str, "HugoLe_%01d.png", i);
			frame = cache->getSpriteFrameByName(str);
			animFrames.pushBack(frame);
			
		}
		empujando=false;
		break;

	case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:

		for (int i = 0; i <= 3; i++)
		{
			if(empujando==true&&tirando==false) sprintf(str, "HugoEmRi_%01d.png", i);
			else if (tirando==true&& pulsadoE==true) sprintf(str, "HugoTiRi_%01d.png",i);
			else sprintf (str, "HugoRi_%01d.png", i);
			frame = cache->getSpriteFrameByName(str);
			animFrames.pushBack(frame);
		}
		empujando=false;
		break;

	case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:

		for (int i = 0; i <= 3; i++)
		{
			if(empujando==true&&tirando==false) sprintf(str, "HugoEmUp_%01d.png", i);
			else if (tirando==true&& pulsadoE==true) sprintf(str, "HugoTiUp_%01d.png",i);
			else sprintf (str, "HugoUp_%01d.png", i);
			frame = cache->getSpriteFrameByName(str);
			animFrames.pushBack(frame);
		}
		empujando=false;
		break;

	case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:

		for (int i = 0; i <= 3; i++)
		{
			if(empujando==true&&tirando==false) sprintf(str, "HugoEmDo_%01d.png", i);
			else if (tirando==true&& pulsadoE==true) sprintf(str, "HugoTiDo_%01d.png",i);
			else sprintf (str, "HugoDo_%01d.png", i);
			frame = cache->getSpriteFrameByName(str);
			animFrames.pushBack(frame);
		}
		empujando=false;
		break;
	}
	animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
	Sprite1->runAction(Animate::create(animation));
}