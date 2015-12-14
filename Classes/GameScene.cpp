#include "GameScene.h"
#include "PauseScene.h"
#include "GameOverScene.h"
#include "Hugo.h"


USING_NS_CC;
using std::string;

Scene* GameScene::createScene(const std::string& File, float x , float y)
{
	
    // 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();
    
    // 'layer' is an autorelease object
	auto layer = GameScene::create(File, x, y);
	layer->setPhysicsWorld(scene->getPhysicsWorld());

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

//#define CREATE_FUNC(GameScene)
GameScene* GameScene::create(const std::string& File, float x, float y) {

	GameScene *pRet = new GameScene();

	pRet->arch = File;
	pRet->cx = x;
	pRet->cy = y;

	if (pRet && pRet->init())
	{

		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}

}

void GameScene::goToPauseScene(Ref *pSender) {

	cStr = arch;
	cuX = cx;
	cuY = cy;

	auto scene = PauseScene::createScene(cStr, cx, cy);
	Director::getInstance()->pushScene(TransitionFade::create(1.0, scene));
}

void GameScene::goToGameOverScene(Ref *pSender) {

	cStr = arch;
	cuX = cx;
	cuY = cy;

	auto scene = GameOverScene::createScene(cStr, cx, cy);
	Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene));
}

void GameScene::goToNewScene(Ref *pSender, const std::string& File, float newx, float newy) {

	cStr = File;
	cuX = newx;
	cuY = newy;

	auto scene = GameScene::createScene(cStr, cuX, cuY);
	Director::getInstance()->replaceScene(TransitionFade::create(3.0, scene));
}

void GameScene::update(float dt) {

	Size visibleSize = Director::getInstance()->getVisibleSize();

	switch (_playerSprite->miedo){
	case 4: {
		goToNewScene(this, "images/mapaPas.tmx", 862.5, 562.5);
		//_playerSprite->miedo=1;
		break;
			}
	case 3: {
	    _mascara1->setOpacity(0);
		_mascara->setOpacity(0);
		_mascara3->setOpacity(999);
		break;
		}
	case 2:  {
		_mascara1->setOpacity(0);
		_mascara->setOpacity(999);
		_mascara3->setOpacity(0);
		break;
			 }
	case 1:	{
		_mascara1->setOpacity(999);
		_mascara->setOpacity(0);
		_mascara3->setOpacity(0);
		break;
			}
	}
	if (_isMoving) {
		Vec2 newPos = Vec2(_playerSprite->getPosition().x + _podVector.x, _playerSprite->getPosition().y + _podVector.y);
		if (newPos.x >= _playerSprite->getBoundingBox().size.width / 2 && newPos.x <= (visibleSize.width - _playerSprite->getBoundingBox().size.width / 2) &&
			newPos.y >= _playerSprite->getBoundingBox().size.height / 2 && newPos.y <= (visibleSize.height - _playerSprite->getBoundingBox().size.height / 2))
		{
			_playerSprite->setPosition(newPos);
			_mascara->setPosition(newPos);
			_mascara1->setPosition(newPos);
			_mascara3->setPosition(newPos);
			_podVector = Vec2::ZERO;

			compEnemigo();
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

bool GameScene::comprobarTilePuerta(float x, float y) {

	int a = coordToTileX(x);
	int b = coordToTileY(y);

	int tileGID = obs->tileGIDAt(Vec2(coordToTileX(x), coordToTileY(y)));
	if (tileGID != 0){

		ValueMap mapProperties = map->propertiesForGID(tileGID).asValueMap(); 

		bool value = mapProperties.at("puerta").asBool(); // Si es false (no es movible), devuelve false

		if (value == true)

			return true; // Se movera

	}
	return false; // No se movera
}

bool GameScene::comprobarTileEne(float x, float y) {

	int a = coordToTileX(x);
	int b = coordToTileY(y);

	int tileGID = obs->tileGIDAt(Vec2(coordToTileX(x), coordToTileY(y)));
	if (tileGID != 0){

		ValueMap mapProperties = map->propertiesForGID(tileGID).asValueMap();

		bool value = mapProperties.at("enemigo").asBool(); // Si es false (no es movible), devuelve false

		if (value == true)

			return true; // Se movera

	}
	return false; // No se movera
}

void GameScene::compEnemigo() {

	bool e1 = false, e2 = false, e3 = false, e4 = false, e5 = false, e6 = false, e7 = false, e8 = false;

	e1 = comprobarTileEne(_playerSprite->getPosition().x, _playerSprite->getPosition().y + 75);
	//e2 = comprobarTileEne(_playerSprite->getPosition().x, _playerSprite->getPosition().y + 150);
	e3 = comprobarTileEne(_playerSprite->getPosition().x, _playerSprite->getPosition().y - 75);
	//e4 = comprobarTileEne(_playerSprite->getPosition().x, _playerSprite->getPosition().y - 150);
	e5 = comprobarTileEne(_playerSprite->getPosition().x + 75, _playerSprite->getPosition().y);
	//e6 = comprobarTileEne(_playerSprite->getPosition().x + 150, _playerSprite->getPosition().y);
	e7 = comprobarTileEne(_playerSprite->getPosition().x - 75, _playerSprite->getPosition().y);
	//e8 = comprobarTileEne(_playerSprite->getPosition().x - 150, _playerSprite->getPosition().y);

	if ((e1 == true || /*e2 == true ||*/ e3 == true || /*e4 == true ||*/ e5 == true || /*e6 == true ||*/ e7 == true /*|| e8 == true*/) && EneOn==true)
	{
		_playerSprite->miedo++;
		EneOn=false;

	}

}

void GameScene::cambiarEscena(float x, float y) {

	int a = coordToTileX(x);
	int b = coordToTileY(y);

	int tileGID = obs->tileGIDAt(Vec2(coordToTileX(x), coordToTileY(y)));
	if (tileGID != 0){

		ValueMap mapProperties = map->propertiesForGID(tileGID).asValueMap();

		int value = mapProperties.at("valorPuerta").asInt(); // Si es false (no es movible), devuelve false

		switch (value) {

		case 0:
			
			goToNewScene(this, "images/mapaBed.tmx", 637.5, 562.5);
			break;

		case 1:
			
			goToNewScene(this, "images/mapaPas.tmx", 412.5, 412.5);
			break;

		case 2:
			
			goToNewScene(this, "images/mapaI.tmx", 1237.5, 112.5);
			break;

		case 3:
			
			goToNewScene(this, "images/mapaII.tmx", 787.5, 112.5);
			break;

		case 4:
			
			goToNewScene(this, "images/mapaIII.tmx", 487.5, 112.5);
			break;

		}
	}
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event){
	_pressedKey = keyCode;
	bool accesible, accesible2, movible, puerta, accesibleant, enemigo1, enemigo2;
	switch (_pressedKey) { //Llamar a la función de Hugo para cambiar el sprite de la animación
	case EventKeyboard::KeyCode::KEY_UP_ARROW:

		//compEnemigo();

		/*enemigo1 = comprobarTileEne(_playerSprite->getPosition().x, _playerSprite->getPosition().y + 75);
		enemigo2 = comprobarTileEne(_playerSprite->getPosition().x, _playerSprite->getPosition().y + 150);

		if (enemigo1 == true || enemigo2 == true) {

			_playerSprite->miedo = 3;
		}*/

		accesible = comprobarTileAcc(_playerSprite->getPosition().x, _playerSprite->getPosition().y + 75);
		movible = comprobarTileMov(_playerSprite->getPosition().x, _playerSprite->getPosition().y + 75);
		puerta = comprobarTilePuerta(_playerSprite->getPosition().x, _playerSprite->getPosition().y + 75);
		_playerSprite->tirando = comprobarTileMov(_playerSprite->getPosition().x, _playerSprite->getPosition().y - 75);


		if (puerta == true) {

			cambiarEscena(_playerSprite->getPosition().x, _playerSprite->getPosition().y + 75);
		}

		else if ((accesible == false || _playerSprite-> tirando == true) && (movible == true || _playerSprite->tirando == true) && puerta == false) { // Es objeto movible

			accesible2 = comprobarTileAcc(_playerSprite->getPosition().x, _playerSprite->getPosition().y + 150); //Es accesible la siguiente tile?
			accesibleant = comprobarTileAcc(_playerSprite->getPosition().x, _playerSprite->getPosition().y + 75);

			if (accesible2 == true && _playerSprite->pulsadoE == true && _playerSprite->tirando == false) {

				LeOn = false;
				RiOn = false;

				_playerSprite-> empujando=true;
				int gid1 = obs->tileGIDAt(Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y + 150)));
				int gid2 = obs->tileGIDAt(Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y + 75)));
				obs->setTileGID(gid1, Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y + 75)));
				obs->setTileGID(gid2, Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y + 150)));

				_playerSprite->animatePlayer(keyCode);
				_podVector = Vec2(0, POD_STEP_MOVE);
				EneOn=true;
				_isMoving = true;
				break;
			}

			else if (accesibleant == true && _playerSprite->pulsadoE == true) {
				LeOn = false;
				RiOn = false;
				_playerSprite->empujando = true;
				int gid1 = obs->tileGIDAt(Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y - 75)));
				int gid2 = obs->tileGIDAt(Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y)));
				obs->setTileGID(gid1, Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y)));
				obs->setTileGID(gid2, Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y - 75)));

				_playerSprite->animatePlayer(keyCode);
				_podVector = Vec2(0, POD_STEP_MOVE);
				EneOn=true;
				_isMoving = true;
				break;
			}
			else if (accesible == true && movible == false && UpOn == true) {

				_playerSprite->animatePlayer(keyCode);
				_podVector = Vec2(0, POD_STEP_MOVE);
				EneOn=true;
				_isMoving = true;
				break;
			}
			break;
		}
		else if (accesible == true && movible == false && UpOn == true) {

			_playerSprite->animatePlayer(keyCode);
			_podVector = Vec2(0, POD_STEP_MOVE);
			EneOn=true;
			_isMoving = true;
			break;
		}
		else {
			if (UpOn == true){

				_playerSprite->animatePlayer(keyCode);
				_isMoving = false;
			}
			break;
		}

	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:

		//compEnemigo();

		/*enemigo1 = comprobarTileEne(_playerSprite->getPosition().x, _playerSprite->getPosition().y - 75);
		enemigo2 = comprobarTileEne(_playerSprite->getPosition().x, _playerSprite->getPosition().y - 150);

		if (enemigo1 == true || enemigo2 == true) {

			_playerSprite->miedo = 3;
		}*/

		accesible = comprobarTileAcc(_playerSprite->getPosition().x, _playerSprite->getPosition().y - 75);
		movible = comprobarTileMov(_playerSprite->getPosition().x, _playerSprite->getPosition().y - 75);
		puerta = comprobarTilePuerta(_playerSprite->getPosition().x, _playerSprite->getPosition().y - 75);
		_playerSprite-> tirando = comprobarTileMov(_playerSprite->getPosition().x, _playerSprite->getPosition().y + 75);

		if (accesible == false && movible == false && puerta == true) {

			cambiarEscena(_playerSprite->getPosition().x, _playerSprite->getPosition().y - 75);
		}

		if ((accesible == false || _playerSprite-> tirando == true) && (movible == true || _playerSprite-> tirando == true) && (_playerSprite->getPosition().y - 150>0) && puerta == false) {

			accesible2 = comprobarTileAcc(_playerSprite->getPosition().x, _playerSprite->getPosition().y - 150);
			accesibleant = comprobarTileAcc(_playerSprite->getPosition().x, _playerSprite->getPosition().y - 75);

			if (accesible2 == true && _playerSprite->pulsadoE == true && _playerSprite-> tirando == false) {

				LeOn = false;
				RiOn = false;

				_playerSprite-> empujando=true;
				int gid1 = obs->tileGIDAt(Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y - 150)));
				int gid2 = obs->tileGIDAt(Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y - 75)));
				obs->setTileGID(gid1, Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y - 75)));
				obs->setTileGID(gid2, Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y - 150)));

				_playerSprite->animatePlayer(keyCode);
				_podVector = Vec2(0, -POD_STEP_MOVE);
				EneOn=true;
				_isMoving = true;
				break;
			}

			else if (accesibleant == true && _playerSprite->pulsadoE == true) {
				LeOn = false;
				RiOn = false;
				_playerSprite->empujando = true;
				int gid1 = obs->tileGIDAt(Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y + 75)));
				int gid2 = obs->tileGIDAt(Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y)));
				obs->setTileGID(gid1, Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y)));
				obs->setTileGID(gid2, Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y + 75)));

				_playerSprite->animatePlayer(keyCode);
				_podVector = Vec2(0, -POD_STEP_MOVE);
				EneOn=true;
				_isMoving = true;
				break;
			}
			else if (accesible == true && movible == false && DoOn == true) {

				_playerSprite->animatePlayer(keyCode);
				_podVector = Vec2(0, -POD_STEP_MOVE);
				EneOn=true;
				_isMoving = true;
				break;
			}
			break;
		}
		else if (accesible == true && movible == false && DoOn == true) {

			_playerSprite->animatePlayer(keyCode);
			_podVector = Vec2(0, -POD_STEP_MOVE);
			EneOn=true;
			_isMoving = true;
			break;
		}
		else {

			if (DoOn == true){
				_playerSprite->animatePlayer(keyCode);
				_isMoving = false;
			}
			break;
		}

	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:

		//compEnemigo();

		/*enemigo1 = comprobarTileEne(_playerSprite->getPosition().x - 75, _playerSprite->getPosition().y);
		enemigo2 = comprobarTileEne(_playerSprite->getPosition().x - 150, _playerSprite->getPosition().y);

		if (enemigo1 == true || enemigo2 == true) {

			_playerSprite->miedo = 3;
		}*/

		accesible = comprobarTileAcc(_playerSprite->getPosition().x - 75, _playerSprite->getPosition().y);
		movible = comprobarTileMov(_playerSprite->getPosition().x - 75, _playerSprite->getPosition().y);
		puerta = comprobarTilePuerta(_playerSprite->getPosition().x - 75, _playerSprite->getPosition().y);
		_playerSprite-> tirando = comprobarTileMov(_playerSprite->getPosition().x + 75, _playerSprite->getPosition().y);

		if (accesible == false && movible == false && puerta == true) {

			cambiarEscena(_playerSprite->getPosition().x - 75, _playerSprite->getPosition().y);
		}

		if ((accesible == false || _playerSprite-> tirando == true) && (movible == true || _playerSprite-> tirando == true) && puerta == false) {

			accesible2 = comprobarTileAcc(_playerSprite->getPosition().x - 150, _playerSprite->getPosition().y);
			accesibleant = comprobarTileAcc(_playerSprite->getPosition().x - 75, _playerSprite->getPosition().y);

			if (accesible2 == true && _playerSprite->pulsadoE == true && _playerSprite-> tirando == false) {
				UpOn = false;
				DoOn = false;

				_playerSprite-> empujando=true;
				int gid1 = obs->tileGIDAt(Vec2(coordToTileX(_playerSprite->getPosition().x - 150), coordToTileY(_playerSprite->getPosition().y)));
				int gid2 = obs->tileGIDAt(Vec2(coordToTileX(_playerSprite->getPosition().x - 75), coordToTileY(_playerSprite->getPosition().y)));
				obs->setTileGID(gid1, Vec2(coordToTileX(_playerSprite->getPosition().x - 75), coordToTileY(_playerSprite->getPosition().y)));
				obs->setTileGID(gid2, Vec2(coordToTileX(_playerSprite->getPosition().x - 150), coordToTileY(_playerSprite->getPosition().y)));

				_playerSprite->animatePlayer(keyCode);
				_podVector = Vec2(-POD_STEP_MOVE, 0);
				EneOn=true;
				_isMoving = true;
				break;
			}
			else if (accesibleant == true && _playerSprite->pulsadoE == true) {
				UpOn = false;
				DoOn = false;
				_playerSprite->empujando = true;
				int gid1 = obs->tileGIDAt(Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y)));
				int gid2 = obs->tileGIDAt(Vec2(coordToTileX(_playerSprite->getPosition().x + 75), coordToTileY(_playerSprite->getPosition().y)));
				obs->setTileGID(gid1, Vec2(coordToTileX(_playerSprite->getPosition().x + 75), coordToTileY(_playerSprite->getPosition().y)));
				obs->setTileGID(gid2, Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y)));

				_playerSprite->animatePlayer(keyCode);
				_podVector = Vec2(-POD_STEP_MOVE, 0);
				EneOn=true;
				_isMoving = true;
				break;
			}
			else if (accesible == true && movible == false && LeOn == true) {

				_playerSprite->animatePlayer(keyCode);
				_podVector = Vec2(-POD_STEP_MOVE, 0);
				EneOn=true;
				_isMoving = true;
				break;
			}

			break;

		}
		else if (accesible == true && movible == false && LeOn == true) {

			_playerSprite->animatePlayer(keyCode);
			_podVector = Vec2(-POD_STEP_MOVE, 0);
			EneOn=true;
			_isMoving = true;
			break;
		}
		else {
			
			if (LeOn == true){
				_playerSprite->animatePlayer(keyCode);
				_isMoving = false;
			}
			break;
		}

	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		
		//compEnemigo();

		/*enemigo1 = comprobarTileEne(_playerSprite->getPosition().x + 75, _playerSprite->getPosition().y);
		enemigo2 = comprobarTileEne(_playerSprite->getPosition().x + 150, _playerSprite->getPosition().y);

		if (enemigo1 == true || enemigo2 == true) {

			_playerSprite->miedo = 3;
		}*/

		accesible = comprobarTileAcc(_playerSprite->getPosition().x + 75, _playerSprite->getPosition().y);
		movible = comprobarTileMov(_playerSprite->getPosition().x + 75, _playerSprite->getPosition().y);
		puerta = comprobarTilePuerta(_playerSprite->getPosition().x + 75, _playerSprite->getPosition().y);
		_playerSprite->tirando = comprobarTileMov(_playerSprite->getPosition().x - 75, _playerSprite->getPosition().y);

		if (accesible == false && movible == false && puerta == true) {

			cambiarEscena(_playerSprite->getPosition().x + 75, _playerSprite->getPosition().y);
		}

		if ((accesible == false ||_playerSprite-> tirando == true) && (movible == true ||_playerSprite-> tirando == true) && (_playerSprite->getPosition().x + 150<1500) && puerta == false) {

			accesible2 = comprobarTileAcc(_playerSprite->getPosition().x + 150, _playerSprite->getPosition().y);
			accesibleant = comprobarTileAcc(_playerSprite->getPosition().x + 75, _playerSprite->getPosition().y);

			if (accesible2 == true && _playerSprite-> pulsadoE == true && _playerSprite->tirando == false) {
				
				UpOn = false;
				DoOn = false;
				_playerSprite-> empujando=true;
				int gid1 = obs->tileGIDAt(Vec2(coordToTileX(_playerSprite->getPosition().x + 150), coordToTileY(_playerSprite->getPosition().y)));
				int gid2 = obs->tileGIDAt(Vec2(coordToTileX(_playerSprite->getPosition().x + 75), coordToTileY(_playerSprite->getPosition().y)));
				obs->setTileGID(gid1, Vec2(coordToTileX(_playerSprite->getPosition().x + 75), coordToTileY(_playerSprite->getPosition().y)));
				obs->setTileGID(gid2, Vec2(coordToTileX(_playerSprite->getPosition().x + 150), coordToTileY(_playerSprite->getPosition().y)));

				_playerSprite->animatePlayer(keyCode);
				_podVector = Vec2(POD_STEP_MOVE, 0);
				EneOn=true;
				_isMoving = true;
				break;
			}
			else if (accesibleant == true && _playerSprite->pulsadoE == true) {
				UpOn = false;
				DoOn = false;
				_playerSprite->empujando = true;
				int gid1 = obs->tileGIDAt(Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y)));
				int gid2 = obs->tileGIDAt(Vec2(coordToTileX(_playerSprite->getPosition().x - 75), coordToTileY(_playerSprite->getPosition().y)));
				obs->setTileGID(gid1, Vec2(coordToTileX(_playerSprite->getPosition().x - 75), coordToTileY(_playerSprite->getPosition().y)));
				obs->setTileGID(gid2, Vec2(coordToTileX(_playerSprite->getPosition().x), coordToTileY(_playerSprite->getPosition().y)));

				_playerSprite->animatePlayer(keyCode);
				_podVector = Vec2(POD_STEP_MOVE, 0);
				EneOn=true;
				_isMoving = true;
				break;
			}
			else if (accesible == true && movible == false && RiOn == true) {

				_playerSprite->animatePlayer(keyCode);
				_podVector = Vec2(POD_STEP_MOVE, 0);
				EneOn=true;
				_isMoving = true;
				break;
			}

			break;

		}
		else if (accesible == true && movible == false && RiOn == true) {

			_playerSprite->animatePlayer(keyCode);
			_podVector = Vec2(POD_STEP_MOVE, 0);
			EneOn=true;
			_isMoving = true;
			break;
		}
		else {

			if (RiOn == true){
				_playerSprite->animatePlayer(keyCode);
				
				_isMoving = false;
			}
			break;
		}

	case EventKeyboard::KeyCode::KEY_E: //Si se pulsa E con un bloque movible al lado, este se pega al jugador, ocupando su ultima posicion hasta
		if (UpOn == false){
			UpOn = true;
			DoOn = true;
		}
		if (LeOn == false){
			LeOn = true;
			RiOn = true;
		}
		if(_playerSprite->pulsadoE==false) // que vuelva a pulsar E, para soltarlo
			_playerSprite->pulsadoE=true;
		else 
			_playerSprite->pulsadoE=false;
		
		break;

	case EventKeyboard::KeyCode::KEY_P:

		goToPauseScene(this);

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
	
	UpOn = true;
	RiOn = true;
	LeOn = true;
	DoOn = true;

	//Loading map http://www.cocos2d-x.org/wiki/TileMap
	_mascara = Sprite::create("images/mascara2.png");
	_mascara->setOpacity(999);
	_mascara->setPosition(cx, cy);
	addChild(_mascara, 2);
	_mascara3 = Sprite::create("images/mascara3.png");
	_mascara3->setOpacity(0);
    _mascara3->setPosition(cx, cy);
    addChild(_mascara3, 2);
	_mascara1 = Sprite::create("images/mascara1.png");
	_mascara1->setOpacity(0);
    _mascara1->setPosition(cx, cy);
    addChild(_mascara1, 2);

	map = TMXTiledMap::create(arch); //"images/mapaI.tmx"
	map->setPosition(Point(0, 0));
	addChild(map, 0);

	obs = map->layerNamed("Obstaculos");

	_playerSprite = new Hugo();
	_playerSprite->setPosition(cx, cy); //Tile(mapaI 1237.5,112.5) (mapaBed 637.5,562.5) (mapaPas 412.5, 337.5)
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