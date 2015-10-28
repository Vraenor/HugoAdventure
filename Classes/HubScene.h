#ifndef __HUBSCENE_SCENE_H__
#define __HUBSCENE_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class HubScene : public cocos2d::Layer
{
public:

	/*Sprite *_backgroundSpriteArray[2];
	std::list<cocos2d::Node *> _asteroids;
	Sprite *_playerSprite;

	void goToPauseScene(Ref *pSender);
	void goToGameScene(Ref *pSender);
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
	CREATE_FUNC(HubScene);*/

private:

	/*EventKeyboard::KeyCode _pressedKey;
	Vec2 _podVector;
	bool _isMoving;
	bool _isMovingByMouse;
	PhysicsWorld *mWorld;*/
};

#endif // __HUB_SCENE_H__
