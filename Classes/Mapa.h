#ifndef __MAPA_H__
#define __MAPA_H__

#include "cocos2d.h"
#include "Hugo.h"

USING_NS_CC;
using std::string;

class Mapa : public cocos2d::Layer {

public:

	cocos2d::CCTMXTiledMap * map;
	cocos2d::CCTMXLayer * obs;
	Hugo * pj;

	CCTMXTiledMap createMapa(std::string mapa);


private:

	//Vector<nivel> * niveles;




};
#endif