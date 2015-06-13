#ifndef __GAMELAYER_SCENE_H__
#define __GAMELAYER_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class StarMatrix;
class GameLayer : public cocos2d::Layer {

public:

	virtual bool init();
	CREATE_FUNC(GameLayer);

	void showStarMatrix();          //œ‘ æ–«–«æÿ’Û
	virtual void update(float delta);


	virtual bool onTouchBegan(Touch* touch, Event* event);


private:
	StarMatrix *matrix;



};

#endif