#ifndef __STAR_SCENE_H__
#define __STAR_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

class Star : public Sprite {
public:
	enum color {
		GREEN,
		RED,
		YELLOW,
		PURPLE,
		BLUE
	};
	static Star* create(int color);
	bool isSelected();
	// 将简单短小的函数定义为内联函数将会提高效率 例如setter 和getter方法
	inline void setSelected(bool b){selected = b;}
	int getColor();
	void setDesPosition(const Point& p);
	inline Point getDesPosition() {return desPosition;}

	void updatePosition();
	inline int getIndexI() {return index_i;}
	inline int getIndexJ() {return index_j;}
	inline void setIndex_ij(int i, int j) {
		index_i = i; 
		index_j = j;
	}

private:
	char *getImage(int color);

public:
	const static int MOVE_SPEED = 4;
	static const int COLOR_MAX_NUM = 5;
	static const int STAR_WIDTH = 48;
	static const int STAR_HEIGHT = 48;

private:
	int color;
	Point desPosition;
	bool selected;
	int index_i;
	int index_j;
};

#endif