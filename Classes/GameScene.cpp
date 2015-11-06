#include "GameScene.h"
#include "PauseScene.h"
#include "GameOverScene.h"

USING_NS_CC;

Scene* GameScene::createScene()
{

    // 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();
    
    // 'layer' is an autorelease object
	auto layer = GameScene::create();
	layer->setPhysicsWorld(scene->getPhysicsWorld());

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void GameScene::goToPauseScene(Ref *pSender) {

	auto scene = PauseScene::createScene();
	Director::getInstance()->pushScene(TransitionFade::create(1.0, scene));
}

void GameScene::goToGameOverScene(Ref *pSender) {

	auto scene = GameOverScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene));
}

void GameScene::update(float dt) {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	

	if (_isMoving) {
		Vec2 newPos = Vec2(_playerSprite->getPosition().x + _podVector.x, _playerSprite->getPosition().y + _podVector.y);
		if (newPos.x >= _playerSprite->getBoundingBox().size.width / 2 && newPos.x <= (visibleSize.width - _playerSprite->getBoundingBox().size.width / 2) &&
			newPos.y >= _playerSprite->getBoundingBox().size.height / 2 && newPos.y <= (visibleSize.height - _playerSprite->getBoundingBox().size.height / 2))
		{
			_playerSprite->setPosition(newPos);
			_podVector = Vec2::ZERO;
		}
	}

}

/*void GameScene::asteroidDone(Node *pSender) {
	pSender->stopAllActions();
	_asteroids.remove(pSender);
	removeChild(pSender);
}*/

/*void GameScene::spawnAsteroid(float dt) {
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
	
	/*auto *seq = Sequence::create(move, CallFuncN::create(CC_CALLBACK_1(GameScene::asteroidDone, this)),NULL);
	seq->retain();
	_asteroids.push_back(tempAsteroid);
	tempAsteroid->runAction(seq);
	addChild(tempAsteroid, 2);
}*/

int GameScene::coordToTileX(float x) {
	// Tile (1,1) 112.5, 862.5
	// Tile (18,11) 1387.5, 112.5
	
	int tileX = 0;
	tileX = int(x / 75);

	return tileX;

}

int GameScene::coordToTileY(float y) {
	// Tile (1,1) 112.5, 862.5
	// Tile (18,11) 1387.5, 112.5

	int tileY = 0;
	tileY = int((975-y)/75);

	return tileY;

}


bool GameScene::comprobarTile(float x, float y) {

	//llamada funcion pasar coordenadas a tile
	//auto map = TMXTiledMap::create("mapa.tmx");
	//auto bloques = map->layerNamed("Bloques");
	//unsigned int gid = bloques->tileGIDAt(Vec2(x, y + 75));
	int a = coordToTileX(x);
	int b = coordToTileY(y);
	
	//TODO Vector en els noms de les layer en cadena, bucle for
	
	int tileGID = obs->tileGIDAt(Vec2(coordToTileX(x), coordToTileY(y)));
	//int tileGID = map->layerNamed("Movibles")->tileGIDAt(Vec2(coordToTileX(x), coordToTileY(y)));
	if (tileGID != 0){

		ValueMap mapProperties = map->propertiesForGID(tileGID).asValueMap();
		//Value valueName = map->getProperty("Nombre");
		
		bool valueProp = mapProperties.at("accesible").asBool(); // Si es false (no es accesible), devuelve false

		if (valueProp == true) 
			
			return true; // Se movera

	}
	/*else
	{
		tileGID = obj->tileGIDAt(Vec2(coordToTileX(x), coordToTileY(y)));
		if (tileGID != 0)
		{
			ValueMap mapProperties = map->propertiesForGID(tileGID).asValueMap();
			//Value valueName = map->getProperty("Nombre");

			bool valueProp = mapProperties.at("accesible").asBool(); // Si es false (no es accesible), devuelve false

			if (valueProp == true)

				return true; // Se movera
		}
	}*/
	return false; // No se movera
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event){
	_pressedKey = keyCode;
	bool accesible;
	switch (_pressedKey) {
	case EventKeyboard::KeyCode::KEY_UP_ARROW:

		accesible = comprobarTile(_playerSprite->getPosition().x , _playerSprite->getPosition().y + 75);
		if (accesible == true) {
			_podVector = Vec2(0, POD_STEP_MOVE);
			_isMoving = true;
			break;
		}
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		accesible = comprobarTile(_playerSprite->getPosition().x, _playerSprite->getPosition().y - 75);
		if (accesible == true) {
			_podVector = Vec2(0, -POD_STEP_MOVE);
			_isMoving = true;
			break;
		}
		break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		accesible = comprobarTile(_playerSprite->getPosition().x - 75, _playerSprite->getPosition().y);
		if (accesible == true) {
			_podVector = Vec2(-POD_STEP_MOVE, 0);
			_isMoving = true;
			break;
		}
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		accesible = comprobarTile(_playerSprite->getPosition().x + 75, _playerSprite->getPosition().y);
		if (accesible == true) {
			_podVector = Vec2(POD_STEP_MOVE, 0);
			_isMoving = true;
			break;
		}
		break;
	}
} 

void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *
	event){
	if (_pressedKey == keyCode) {
		_pressedKey = EventKeyboard::KeyCode::KEY_NONE;
		_isMoving = false;
		_podVector = Vec2::ZERO;
	}
}

void GameScene::setPhysicsWorld(PhysicsWorld *world) {
	mWorld = world;
	mWorld->setGravity(Vec2::ZERO);
}

bool GameScene::onContactBegin(PhysicsContact &contact) {
	goToGameOverScene(this);
	return true;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();

	//Setting and binding keyboard callbacks
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	_pressedKey = EventKeyboard::KeyCode::KEY_NONE;
	_podVector = Vec2::ZERO;
	_isMoving = false;

	//Loading map http://www.cocos2d-x.org/wiki/TileMap
	
	map = TMXTiledMap::create("images/mapaI.tmx");
	map->setPosition(Point(0, 0));
	addChild(map, 0);

	obs = map->layerNamed("Obstaculos");

	//for (const auto& child : map->getChildren()) static_cast<SpriteBatchNode*>(child)->getTexture()->setAntiAliasTexParameters();

	//_backgroundGameScene->setPosition(Point(visibleSize.width / 2, visibleSize.height /2));

	// Loading player sprite
	_playerSprite = Sprite::create("images/HugoUp_1.png");
	_playerSprite->setPosition(Point(1237.5, 112.5)); //Tile(18,11)
	addChild(_playerSprite, 1);

	/*SpriteBatchNode* spritebatch = SpriteBatchNode::create("Hugo.tps");
	SpriteFrameCache* cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("Hugo.plist");
	auto Sprite1 = Sprite::createWithSpriteFrameName("HugoUp_1.png");
	spritebatch->addChild(Sprite1);
	addChild(spritebatch);

	Vector<SpriteFrame*> animFrames(2);

	char str[100] = { 0 };
	for (int i = 0; i < 2; i++)
	{
		sprintf(str, "HugoUp_%02d.png", i);
		SpriteFrame* frame = cache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}

	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.3f);
	Sprite1->runAction(Animate::create(animation));*/


	auto body = PhysicsBody::createCircle(_playerSprite->getBoundingBox().size.width / 2);
	body->setContactTestBitmask(true);
	body->setDynamic(true);
	_playerSprite->setPhysicsBody(body);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	this->scheduleUpdate();
	//this->schedule(schedule_selector(GameScene::spawnAsteroid), 1.0);
    return true;
}