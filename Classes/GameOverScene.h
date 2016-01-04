#ifndef __GAMEOVER_SCENE_H__
#define __GAMEOVER_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class GameOverScene : public cocos2d::Layer
{
public:

	void goToMainMenuScene(Ref *pSender);
	void retryGameScene(Ref *pSender);
	std::string curMap;
	float curX, curY;

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene(const std::string& File, float x, float y);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
	//CREATE_FUNC(GameOverScene);

	static GameOverScene* create(const std::string& File, float x, float y);
};

#endif // __GAMEOVER_SCENE_H__
