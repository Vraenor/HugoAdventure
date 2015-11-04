#ifndef __OPTIONS_SCENE_H__
#define __OPTIONS_SCENE_H__

#include "cocos2d.h"

class OptionsScene : public cocos2d::Layer
{
public:

	void goToMainMenuScene(Ref *pSender);
	void resumePauseScene(Ref *pSender);

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(OptionsScene);
};

#endif // __OPTIONS_SCENE_H__