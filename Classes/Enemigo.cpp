#include "Enemigo.h"
#include "Hugo.h"
#include "GameScene.h"

USING_NS_CC;


Enemigo::Enemigo(void) {


}

Enemigo::~Enemigo(void){


}

Enemigo* Enemigo::createEnemigo(std::string mapa) {


	switch (mapa)
	{
	default:
		break;
	}
	Enemigo* ob = new Enemigo();
	if (ob && ob->initEnemigo(posX, posY)) {

		ob->autorelease();
		return ob;
	}

	return NULL;

}

bool Enemigo::initEnemigo(float posX, float posY) {

	this->initWithFile("enemigo.png");
	pos = ccp(eneX, eneY)

}

	/*eSprite = Sprite::create("images/enemy1.png");
	addChild(eSprite);*/

void buscarPersonaje() {

	//if (GameScene::_playerSprite->get



}