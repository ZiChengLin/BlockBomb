#ifndef __STARMATRIX_SCENE_H__
#define __STARMATRIX_SCENE_H__

#include "cocos2d.h"
#include "Star.h"
#include "GameLayer.h"
#include <deque>              //类似于vector 不过允许两端插入和删除元素

USING_NS_CC;
using namespace std;

class GameLayer;
class StarMatrix : public Node {

public:
	static StarMatrix* create(GameLayer* layer);  //传入layer是为了以后通知layer进行更新操作用
	virtual bool init(GameLayer* layer);

	void updateStar(float delta);
	void onTouch(const Point& p);
	void setNeedClear(bool b);
	void initMatrix();

private:
	Point getPositionByIndex(int i, int j);  //可以由矩阵位置i和j得到一个Point
	Star* getStarByTouch(const Point& p);    //得到GameLayer点击传入的touch

	void genSelectedList(Star* s);           
	void deleteSelectedList();
	void adjustMatrix();
	void refreshScore();
	bool isEnded();
	void clearMatrixOneByOne();
	int getLeftStarNum();

public:
	const static int ROW_NUM = 10;
	const static int COL_NUM = 10;

private:
	Star* stars[ROW_NUM][COL_NUM];
	GameLayer *m_layer;
	deque<Star*> selectedList;      // 定义一个Star的数组列表
	bool needClear;
	static float ONE_CLEAR_TIME;    // 每颗星星消除的时间
	float clearSumTime;
	Star *star;
};

#endif