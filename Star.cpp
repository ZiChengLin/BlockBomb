#include "Star.h"

// ��ʼ������
Star* Star::create(int color) {

	Star *ret = new Star();
	if (ret && ret->initWithFile(ret->getImage(color))) {

		ret->color = color;
		ret->selected = false;
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

char* Star::getImage(int color) {

	switch (color) {
	case color::BLUE:
		return "blue@64.png";
	case color::GREEN:
		return "green@64.png";
	case color::YELLOW:
		return "yellow@64.png";
	case color::RED:
		return "red@64.png";
	case color::PURPLE:
		return "purple@64.png";
	}
}

bool Star::isSelected(){
	return selected;
}

int Star::getColor(){
	return color;
}

void Star::setDesPosition(const Point& p){
	desPosition = p;
}

// ��StarMatrix�е���
void Star::updatePosition() {

	// �жϵ�ǰλ����Ŀ��λ��desPosition�Ƿ���ͬ
	if (desPosition.y != getPositionY()) {
		// �𽥸ı�Star��position��ʹ֮���ƶ���desPosition
		setPositionY(getPositionY() - MOVE_SPEED);
		if (getPositionY() < desPosition.y) {
			setPositionY(desPosition.y);
		}
	}
	if (desPosition.x != getPositionX()) {
	
		setPositionX(getPositionX() - MOVE_SPEED);
		if(getPositionX() < desPosition.x)
		{
			setPositionX(desPosition.x);
		}
	}
}