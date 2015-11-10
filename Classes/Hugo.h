#ifndef __HUGO_H__
#define __HUGO_H__

#include "cocos2d.h"

USING_NS_CC;

class Hugo : public cocos2d::Layer
{
	
public:

	cocos2d::Sprite * playerSprite;
	Point * pos;
	Vector<SpriteFrame*> animFrames(int);
	Hugo();
	void animatePlayer(EventKeyboard::KeyCode keyCode);

private:



};
#endif