#ifndef __GAMESCENE_SCENE_H__
#define __GAMESCENE_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

const float METEOR_SPEED = 0.5f;
const float BACKGROUND_SPEED = 0.3f;
const int POD_STEP_MOVE = 10;

class GameScene : public cocos2d::Layer
{
public:

	Sprite *_backgroundSpriteArray[2];
	std::list<cocos2d::Node *> _asteroids;
	Sprite *_playerSprite;

	void goToPauseScene(Ref *pSender);
	void goToGameOverScene(Ref *pSender);
	void update(float dt);
	void spawnAsteroid(float dt);
	void asteroidDone(Node *pSender);
	void backgroundDone(Node *pSender);
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event);
	void onMouseMove(Event *event);

	void setPhysicsWorld(PhysicsWorld *world);
	bool onContactBegin(PhysicsContact &contact);

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
	CREATE_FUNC(GameScene);

private:

	EventKeyboard::KeyCode _pressedKey;
	Vec2 _podVector;
	bool _isMoving;
	bool _isMovingByMouse;
	PhysicsWorld *mWorld;
};

#endif // __GAME_SCENE_H__
