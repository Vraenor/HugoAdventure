#include "GameRoom.h"
#include "GameScene.h"
#include "GameHub.h"

USING_NS_CC;

Scene* GameRoom::createScene()
{

    // 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();
    
    // 'layer' is an autorelease object
	auto layer = GameRoom::create();
	layer->setPhysicsWorld(scene->getPhysicsWorld());

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void GameRoom::goToGameScene(Ref *pSender) {

	auto scene = GameScene::createScene();
	Director::getInstance()->pushScene(TransitionFade::create(1.0, scene));
}

void GameRoom::goToGameHub(Ref *pSender) {

	auto scene = GameHub::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene));
}

void GameRoom::update(float dt) {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	// Scrolls background
	for (int i = 0; i < 2; i++) {
		if (_backgroundSpriteArray[i]->getPosition().y < (visibleSize.height* -0.5))
			_backgroundSpriteArray[i]->setPosition(Point(visibleSize.width / 2,visibleSize.height * 1.5f));
		else
			_backgroundSpriteArray[i]->setPosition(Point(_backgroundSpriteArray[i]->getPosition().x,
			_backgroundSpriteArray[i]->getPosition().y -(BACKGROUND_SPEED * visibleSize.height * dt)));
	}

	if (_isMoving || _isMovingByMouse) {
		Vec2 newPos = Vec2(_playerSprite->getPosition().x + _podVector.x,
			_playerSprite->getPosition().y + _podVector.y);
		if (newPos.x >= _playerSprite->getBoundingBox().size.width / 2 && newPos.x <= (visibleSize.width - _playerSprite->getBoundingBox().size.width / 2) &&
			newPos.y >= _playerSprite->getBoundingBox().size.height / 2 && newPos.y <= (visibleSize.height - _playerSprite->getBoundingBox().size.height / 2))
		{
			_playerSprite->setPosition(newPos);
		}
		_isMovingByMouse = false;
	}

}

void GameRoom::asteroidDone(Node *pSender) {
	pSender->stopAllActions();
	_asteroids.remove(pSender);
	removeChild(pSender);
}

void GameRoom::spawnAsteroid(float dt) {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	int asteroidIndex = (std::rand() % 3 + 1);
	__String *filename = __String::createWithFormat("images/GameScreen/Asteroid_%i.png",asteroidIndex);
	
	Sprite *tempAsteroid = Sprite::create(filename->getCString());

	auto body = PhysicsBody::createCircle(tempAsteroid->getBoundingBox().size.width / 2);
	body->setContactTestBitmask(true);
	body->setDynamic(true);
	tempAsteroid->setPhysicsBody(body);
	
	int xRandomPosition = (std::rand() % (int)(visibleSize.width - tempAsteroid->getContentSize().width)) 
		+ tempAsteroid->getContentSize().width / 2;
	
	tempAsteroid->setPosition(Point(xRandomPosition, visibleSize.height + tempAsteroid->getContentSize().height));
	
	auto *move = (FiniteTimeAction *)MoveBy::create(2 / METEOR_SPEED, Point(0, - visibleSize.height - tempAsteroid->getBoundingBox().size.height));
	move->retain();
	
	auto *seq = Sequence::create(move, CallFuncN::create(CC_CALLBACK_1(GameRoom::asteroidDone, this)),NULL);
	seq->retain();
	_asteroids.push_back(tempAsteroid);
	tempAsteroid->runAction(seq);
	addChild(tempAsteroid, 2);
}

void GameRoom::onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event){
	_pressedKey = keyCode;
	switch (_pressedKey) {
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		_podVector = Vec2(0, POD_STEP_MOVE);
		_isMoving = true;
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		_podVector = Vec2(0, -POD_STEP_MOVE);
		_isMoving = true;
		break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		_podVector = Vec2(-POD_STEP_MOVE, 0);
		_isMoving = true;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		_podVector = Vec2(POD_STEP_MOVE, 0);
		_isMoving = true;
		break;
	}
} void GameRoom::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *
	event){
	if (_pressedKey == keyCode) {
		_pressedKey = EventKeyboard::KeyCode::KEY_NONE;
		_isMoving = false;
		_podVector = Vec2::ZERO;
	}
}

void GameRoom::onMouseMove(Event *event) {
	static Vec2 *oldPosition;
	auto *e = dynamic_cast<EventMouse *>(event);
	if (oldPosition == NULL) {
		oldPosition = new Vec2(e->getCursorX(), e->getCursorY());
	}
	else {
		_podVector = Vec2(e->getCursorX() - oldPosition->x,
			e->getCursorY() - oldPosition->y);
		if (!_isMovingByMouse) {
			_isMovingByMouse = true;
			oldPosition->x = e->getCursorX();
			oldPosition->y = e->getCursorY();
		}
	}
}

void GameRoom::setPhysicsWorld(PhysicsWorld *world) {
	mWorld = world;
	mWorld->setGravity(Vec2::ZERO);
}

bool GameRoom::onContactBegin(PhysicsContact &contact) {
	goToGameHub(this);
	return true;
}

// on "init" you need to initialize your instance
bool GameRoom::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();

	auto pauseItem = MenuItemImage::create("images/GameScreen/Pause_Button.png", "images/GameScreen/Pause_Button(Click).png", 
		CC_CALLBACK_1(GameRoom::goToGameScene, this));
	addChild(pauseItem, 1);

	// Setting and binding mouse callbacks
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseMove = CC_CALLBACK_1(GameRoom::onMouseMove, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, this);

	//Setting and binding keyboard callbacks
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(GameRoom::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(GameRoom::onKeyReleased, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	_pressedKey = EventKeyboard::KeyCode::KEY_NONE;
	_podVector = Vec2::ZERO;
	_isMoving = false;

	//Loading scrollable background
	for (int i = 0; i < 2; i++) {
		_backgroundSpriteArray[i]
			= Sprite::create("images/GameScreen/Game_Screen_Background.png");
		_backgroundSpriteArray[i]->setPosition(Point(visibleSize.width / 2, visibleSize.height * (i + 0.5f)));
		addChild(_backgroundSpriteArray[i], 0);
	}
	// Loading player sprite
	_playerSprite = Sprite::create("images/GameScreen/Space_Pod.png");
	_playerSprite->setPosition(Point(visibleSize.width / 2,
		_playerSprite->getContentSize().height * 0.75));
	addChild(_playerSprite, 1);

	auto body = PhysicsBody::createCircle(_playerSprite->getBoundingBox().size.width / 2);
	body->setContactTestBitmask(true);
	body->setDynamic(true);
	_playerSprite->setPhysicsBody(body);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameRoom::onContactBegin, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	this->scheduleUpdate();
	this->schedule(schedule_selector(GameRoom::spawnAsteroid), 1.0);
    return true;
}