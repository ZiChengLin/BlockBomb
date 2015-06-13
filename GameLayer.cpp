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
		//�����������Ǿ���matrix��updateStar����
		matrix->updateStar(delta);
	}
}

void GameLayer::showStarMatrix() {
	// ����this�ǰѱ�layer����ȥ
	matrix = StarMatrix::create(this);
	this->addChild(matrix);
}

bool GameLayer::onTouchBegan(Touch* touch, Event* event) {
	// ��ȡ������ͼ�е����꣨���Ͻ�Ϊԭ�㣩
	Point p = touch->getLocationInView();
	// �ѵ������ת��ΪOpenGL���꣨���½�Ϊԭ�㣩
	p = Director::getInstance()->convertToGL( p );
	//log("x = %f y = %f", p.x, p.y);
	// ���������� ��Ѵ����㴫��ȥ
	if (matrix) {  
		matrix->onTouch( p );
	}

	return true;
}