#include "GameScene.h"
#include "PauseScene.h"
#include "GameOverScene.h"
#include "Hugo.h"


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

bool GameScene::comprobarTileAcc(float x, float y) {

	int a = coordToTileX(x);
	int b = coordToTileY(y);
	
	int tileGID = obs->tileGIDAt(Vec2(coordToTileX(x), coordToTileY(y)));
	if (tileGID != 0){

		ValueMap mapProperties = map->propertiesForGID(tileGID).asValueMap();
		
		bool value = mapProperties.at("accesible").asBool(); // Si es false (no es accesible), devuelve false

		if (value == true) 
			
			return true; // Se movera

	}

	return false; // No se movera
}

bool GameScene::comprobarTileMov(float x, float y) {

	int a = coordToTileX(x);
	int b = coordToTileY(y);

	int tileGID = obs->tileGIDAt(Vec2(coordToTileX(x), coordToTileY(y)));
	if (tileGID != 0){

		ValueMap mapProperties = map->propertiesForGID(tileGID).asValueMap();

		bool value = mapProperties.at("movible").asBool(); // Si es false (no es movible), devuelve false

		if (value == true)

			return true; // Se movera

	}

	return false; // No se movera
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event){
	_pressedKey = keyCode;
	bool accesible, accesible2, movible, movible2;
	switch (_pressedKey) { //Llamar a la función de Hugo para cambiar el sprite de la animación
	case EventKeyboard::KeyCode::KEY_UP_ARROW:

		accesible = comprobarTileAcc(_playerSprite->getPosition().x, _playerSprite->getPosition().y + 75);
		movible = comprobarTileMov(_playerSprite->getPosition().x, _playerSprite->getPosition().y + 75);

		if (accesible == false && movible == true) { // Es objeto movible

			accesible2 = comprobarTileAcc(_playerSprite->getPosition().x, _playerSprite->getPosition().y + 150); //Es accesible la siguiente tile?

			if (accesible2 == true && pulsadoE == true) {

				_playerSprite-> empujando=true;
				int gid1 = obs->tileGIDAt(Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y + 150)));
				int gid2 = obs->tileGIDAt(Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y + 75)));
				obs->removeTileAt(Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y + 75)));
				obs->removeTileAt(Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y + 150)));
				obs->setTileGID(gid1, Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y + 75)));
				obs->setTileGID(gid2, Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y + 150)));

				_playerSprite->animatePlayer(keyCode);
				_podVector = Vec2(0, POD_STEP_MOVE);
				_isMoving = true;
				break;
			}
			break;
			
			/*else
			{
				_isMoving = false;
			}*/
		}
		else if (accesible == true && movible == false) {

			_playerSprite->animatePlayer(keyCode);
			_podVector = Vec2(0, POD_STEP_MOVE);
			_isMoving = true;
			break;
		}
		else {

			_playerSprite->animatePlayer(keyCode);
			_isMoving = false;
			break;
		}

	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:

		accesible = comprobarTileAcc(_playerSprite->getPosition().x, _playerSprite->getPosition().y - 75);
		movible = comprobarTileMov(_playerSprite->getPosition().x, _playerSprite->getPosition().y - 75);

		if (accesible == false && movible == true) {

			accesible2 = comprobarTileAcc(_playerSprite->getPosition().x, _playerSprite->getPosition().y - 150);

			if (accesible2 == true && pulsadoE == true) {

				_playerSprite-> empujando=true;
				int gid1 = obs->tileGIDAt(Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y - 150)));
				int gid2 = obs->tileGIDAt(Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y - 75)));
				obs->removeTileAt(Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y - 75)));
				obs->removeTileAt(Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y - 150)));
				obs->setTileGID(gid1, Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y - 75)));
				obs->setTileGID(gid2, Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y - 150)));

				_playerSprite->animatePlayer(keyCode);
				_podVector = Vec2(0, -POD_STEP_MOVE);
				_isMoving = true;
				break;
			}
			break;
		}
		else if (accesible == true && movible == false) {

			_playerSprite->animatePlayer(keyCode);
			_podVector = Vec2(0, -POD_STEP_MOVE);
			_isMoving = true;
			break;
		}
		else {

			_playerSprite->animatePlayer(keyCode);
			_isMoving = false;
			break;
		}

	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:

		accesible = comprobarTileAcc(_playerSprite->getPosition().x - 75, _playerSprite->getPosition().y);
		movible = comprobarTileMov(_playerSprite->getPosition().x - 75, _playerSprite->getPosition().y);

		if (accesible == false && movible == true) {

			accesible2 = comprobarTileAcc(_playerSprite->getPosition().x - 150, _playerSprite->getPosition().y);

			if (accesible2 == true && pulsadoE == true) {

				_playerSprite-> empujando=true;
				int gid1 = obs->tileGIDAt(Vec2(coordToTileX(_playerSprite->getPosition().x - 150), coordToTileY(_playerSprite->getPosition().y)));
				int gid2 = obs->tileGIDAt(Vec2(coordToTileX(_playerSprite->getPosition().x - 75), coordToTileY(_playerSprite->getPosition().y)));
				obs->removeTileAt(Vec2(coordToTileX(_playerSprite->getPosition().x - 75), coordToTileY(_playerSprite->getPosition().y)));
				obs->removeTileAt(Vec2(coordToTileX(_playerSprite->getPosition().x - 150), coordToTileY(_playerSprite->getPosition().y)));
				obs->setTileGID(gid1, Vec2(coordToTileX(_playerSprite->getPosition().x - 75), coordToTileY(_playerSprite->getPosition().y)));
				obs->setTileGID(gid2, Vec2(coordToTileX(_playerSprite->getPosition().x - 150), coordToTileY(_playerSprite->getPosition().y)));

				_playerSprite->animatePlayer(keyCode);
				_podVector = Vec2(-POD_STEP_MOVE, 0);
				_isMoving = true;
				break;
			}
			break;

		}
		else if (accesible == true && movible == false) {

			_playerSprite->animatePlayer(keyCode);
			_podVector = Vec2(-POD_STEP_MOVE, 0);
			_isMoving = true;
			break;
		}
		else {

			_playerSprite->animatePlayer(keyCode);
			_isMoving = false;
			break;
		}

	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		
		accesible = comprobarTileAcc(_playerSprite->getPosition().x + 75, _playerSprite->getPosition().y);
		movible = comprobarTileMov(_playerSprite->getPosition().x + 75, _playerSprite->getPosition().y);

		if (accesible == false && movible == true) {

			accesible2 = comprobarTileAcc(_playerSprite->getPosition().x + 150, _playerSprite->getPosition().y);

			if (accesible2 == true && pulsadoE == true) {
				
				_playerSprite-> empujando=true;
				int gid1 = obs->tileGIDAt(Vec2(coordToTileX(_playerSprite->getPosition().x + 150), coordToTileY(_playerSprite->getPosition().y)));
				int gid2 = obs->tileGIDAt(Vec2(coordToTileX(_playerSprite->getPosition().x + 75), coordToTileY(_playerSprite->getPosition().y)));
				obs->removeTileAt(Vec2(coordToTileX(_playerSprite->getPosition().x + 75), coordToTileY(_playerSprite->getPosition().y)));
				obs->removeTileAt(Vec2(coordToTileX(_playerSprite->getPosition().x + 150), coordToTileY(_playerSprite->getPosition().y)));
				obs->setTileGID(gid1, Vec2(coordToTileX(_playerSprite->getPosition().x + 75), coordToTileY(_playerSprite->getPosition().y)));
				obs->setTileGID(gid2, Vec2(coordToTileX(_playerSprite->getPosition().x + 150), coordToTileY(_playerSprite->getPosition().y)));

				_playerSprite->animatePlayer(keyCode);
				_podVector = Vec2(POD_STEP_MOVE, 0);
				_isMoving = true;
				break;
			}
			break;

		}
		else if (accesible == true && movible == false) {

			_playerSprite->animatePlayer(keyCode);
			_podVector = Vec2(POD_STEP_MOVE, 0);
			_isMoving = true;
			break;
		}
		else {

			_playerSprite->animatePlayer(keyCode);
			_isMoving = false;
			break;
		}

	case EventKeyboard::KeyCode::KEY_E:
		if(pulsadoE==false)
			pulsadoE=true;
		else 
			pulsadoE=false;

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
	pulsadoE=false;

	//Loading map http://www.cocos2d-x.org/wiki/TileMap
	
	map = TMXTiledMap::create("images/mapaI.tmx");
	map->setPosition(Point(0, 0));
	addChild(map, 0);

	obs = map->layerNamed("Obstaculos");

	_playerSprite = new Hugo();
	_playerSprite->setPosition(1237.5, 112.5); //Tile(18,11)
	addChild(_playerSprite, 1);

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