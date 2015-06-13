#ifndef __STARMATRIX_SCENE_H__
#define __STARMATRIX_SCENE_H__

#include "cocos2d.h"
#include "Star.h"
#include "GameLayer.h"
#include <deque>              //������vector �����������˲����ɾ��Ԫ��

USING_NS_CC;
using namespace std;

class GameLayer;
class StarMatrix : public Node {

public:
	static StarMatrix* create(GameLayer* layer);  //����layer��Ϊ���Ժ�֪ͨlayer���и��²�����
	virtual bool init(GameLayer* layer);

	void updateStar(float delta);
	void onTouch(const Point& p);
	void setNeedClear(bool b);
	void initMatrix();

private:
	Point getPositionByIndex(int i, int j);  //�����ɾ���λ��i��j�õ�һ��Point
	Star* getStarByTouch(const Point& p);    //�õ�GameLayer��������touch

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
	deque<Star*> selectedList;      // ����һ��Star�������б�
	bool needClear;
	static float ONE_CLEAR_TIME;    // ÿ������������ʱ��
	float clearSumTime;
	Star *star;
};

#endif