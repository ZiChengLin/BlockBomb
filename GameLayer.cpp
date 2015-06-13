#include "GameLayer.h"
#include "StarMatrix.h"

bool GameLayer::init() {

	if (!Layer::init()) {

		return false;
	}
	matrix = nullptr;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Sprite* background = Sprite::create("game_bg.png");
	background->setPosition(visibleSize.width / 2,visibleSize.height / 2);
	this->addChild(background,-1);

	this->scheduleUpdate();
	showStarMatrix();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void GameLayer::update(float delta) {

	if (matrix) {
		//调用内置星星矩阵matrix的updateStar方法
		matrix->updateStar(delta);
	}
}

void GameLayer::showStarMatrix() {
	// 参数this是把本layer传过去
	matrix = StarMatrix::create(this);
	this->addChild(matrix);
}

bool GameLayer::onTouchBegan(Touch* touch, Event* event) {
	// 获取点在视图中的坐标（左上角为原点）
	Point p = touch->getLocationInView();
	// 把点的坐标转化为OpenGL坐标（左下角为原点）
	p = Director::getInstance()->convertToGL( p );
	//log("x = %f y = %f", p.x, p.y);
	// 如果矩阵存在 则把触摸点传过去
	if (matrix) {  
		matrix->onTouch( p );
	}

	return true;
}