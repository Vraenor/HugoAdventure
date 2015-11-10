#include "Hugo.h"

USING_NS_CC;

Hugo::Hugo() {

	playerSprite = Sprite::create("images/HugoUp_1.png");
	this->addChild(playerSprite);
}

void Hugo::animatePlayer(EventKeyboard::KeyCode keyCode) {

	/*SpriteBatchNode* spritebatch = SpriteBatchNode::create("Hugo.tps");
	SpriteFrameCache* cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("Hugo.plist");
	auto Sprite1 = Sprite::createWithSpriteFrameName("HugoUp_1.png");
	spritebatch->addChild(Sprite1);
	addChild(spritebatch);*/

	SpriteBatchNode* spritebatch = SpriteBatchNode::create("images/Hugo.png");
	SpriteFrameCache* cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("images/Hugo.plist");

	auto Sprite1 = Sprite::createWithSpriteFrameName("HugoUp_1.png");
	spritebatch->addChild(Sprite1);
	addChild(spritebatch);

	SpriteFrame* frame;
	Animation* animation;

	Vector<SpriteFrame*> animFrames(3);
	char str[100] = { 0 };
	animFrames.clear();

	switch (keyCode)
	{

		case cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW:

			for (int i = 1; i <= 3; i++)
			{
				sprintf(str, "HugoLe_%01d.png", i);
				frame = cache->getSpriteFrameByName(str);
				animFrames.pushBack(frame);
			}
			animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
			Sprite1->runAction(Animate::create(animation));
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:

			for (int i = 1; i <= 3; i++)
			{
				sprintf(str, "HugoRi_%01d.png", i);
				frame = cache->getSpriteFrameByName(str);
				animFrames.pushBack(frame);
			}
			animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
			Sprite1->runAction(Animate::create(animation));
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW:

			for (int i = 1; i <= 3; i++)
			{
				sprintf(str, "HugoUp_%01d.png", i);
				frame = cache->getSpriteFrameByName(str);
				animFrames.pushBack(frame);
			}
			animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
			Sprite1->runAction(Animate::create(animation));
			break;
		case cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW:

			for (int i = 1; i <= 3; i++)
			{
				sprintf(str, "HugoDo_%01d.png", i);
				frame = cache->getSpriteFrameByName(str);
				animFrames.pushBack(frame);
			}
			animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
			Sprite1->runAction(Animate::create(animation));
			break;
		}

	//Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
	//Sprite1->runAction(Animate::create(animation));
	//animFrames.popBack();
	
}