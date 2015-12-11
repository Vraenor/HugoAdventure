#ifndef __ENEMIGO_H__
#define __ENEMIGO_H__

#include "cocos2d.h"

USING_NS_CC;

class Enemigo : public cocos2d::CCSprite {

public:

	CCPoint pos;
	float posX, posY;
	String facing;
	std::string map;
	//Sprite * eSprite;

	Enemigo(void);
	~Enemigo();
	static Enemigo* createEnemigo(std::string mapa);
	bool initEnemigo(float posX, float posY);
	void update();
	void buscarPersonaje();
	void meterMiedo();//gira al enemigo en todas las direcciones, 
	//si el jugador se encuentra en una de ellas a una distancia de 2 tiles o menos
	// le aumenta el miedo
	Point coorEnemigo(std::string mapa);
	


private:



};
#endif