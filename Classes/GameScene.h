#ifndef __GAMESCENE_SCENE_H__
#define __GAMESCENE_SCENE_H__

#include "cocos2d.h"
#include "Hugo.h"

USING_NS_CC;

const int POD_STEP_MOVE = 75;

class GameScene : public cocos2d::Layer {
public:
	
	Hugo * _playerSprite;
	CCTMXTiledMap *map;
	Sprite* _backgroundGameScene;
	std::string arch;
	float cx, cy;

	void goToPauseScene(Ref *pSender);
	void goToGameOverScene(Ref *pSender);
	void update(float dt);
	void spawnAsteroid(float dt);
	void asteroidDone(Node *pSender);
	void backgroundDone(Node *pSender);
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event);
	bool comprobarTileAcc(float x, float y);
	bool comprobarTileMov(float x, float y);
	bool pulsadoE; 
	int coordToTileX(float x);
	int coordToTileY(float y);

	void setPhysicsWorld(PhysicsWorld *world);
	bool onContactBegin(PhysicsContact &contact);

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(const std::string& File, float x, float y);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
	//CREATE_FUNC(GameScene);

	static GameScene* create(const std::string& File, float x, float y);


private:

	EventKeyboard::KeyCode _pressedKey;
	Vec2 _podVector;
	bool _isMoving;
	PhysicsWorld *mWorld;
	CCTMXLayer *obs;
	
};

#endif // __GAME_SCENE_H__
