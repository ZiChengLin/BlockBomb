#include "StarMatrix.h"
#include "StarParticle.h"

#include <ctime>

float StarMatrix::ONE_CLEAR_TIME = 0.05f;

// �����ʼ���Լ��ڴ����
StarMatrix* StarMatrix::create(GameLayer* layer) { 

	StarMatrix *ret = new StarMatrix();
	if (ret && ret->init(layer)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool StarMatrix::init(GameLayer* layer) {

	if (!Node::init()) {

		return false;
	}

	m_layer = layer;
	needClear = false;
	clearSumTime = 0;
	// ������ָ�����Щ�ڴ涼��ʼ��Ϊ�� ��ֹ��Щ�ڴ��е��Ѿ���ռ��
	memset(stars, 0, sizeof(Star*) * ROW_NUM * COL_NUM);
	initMatrix();    // ��ʼ�����Ǿ��� 

	return true;
}

void StarMatrix::initMatrix() {
	// �����������
	srand(time(0));
	for (int i = 0; i < ROW_NUM; i++) {
		for (int j = 0; j < COL_NUM; j++) {

			int color = abs(rand() % Star::COLOR_MAX_NUM);  // �����������0-5
			if (color < 0) {
				log ("color i = %d, j = %d");
			}
			star = Star::create(color);
			stars[i][j] = star;       // ���������������Ƿ���������
			star->setPosition(getPositionByIndex(i, j) + Point(0, 100));    // y�����100�ǳ�ʼλ�� Ϊ��������µĶ���
			star->setDesPosition(getPositionByIndex(i, j));
			star->setIndex_ij(i, j);  // ����setter������ֵ����Star
			this->addChild(star);

		}
	}
}

// ��ȡÿ���������ص�λ��(λ��i,j�ǴӾ�������Ͻǿ�ʼ�ݹ�,Ȼ��cocos2d������ϵ�������½�)
Point StarMatrix::getPositionByIndex(int i,int j) {

	float x = j * Star::STAR_WIDTH + Star::STAR_WIDTH / 2;
	float y = (StarMatrix::COL_NUM - i) * Star::STAR_HEIGHT - Star::STAR_HEIGHT / 2;
	return Point(x, y + 100);
}

// ��GameLayer�������
void StarMatrix::updateStar(float delta) {

	for (int i = 0; i < ROW_NUM; i++) {
		for (int j = 0; j < COL_NUM; j++) {
			if (stars[i][j] != nullptr) {
				// �����������Star���updatePosition()����
				stars[i][j]->updatePosition();
			}
		}
	}
	// �����ʱ��ﵽһ��ʱ�����������
	if (needClear) {
		clearSumTime += delta;
		if (clearSumTime > ONE_CLEAR_TIME) {
			//clearMatrixOneByOne();
			clearSumTime = 0;            // clearSumTime��һ���ۼ���
		}
	}
}

void StarMatrix::setNeedClear(bool b) {

	needClear = b;
}

void StarMatrix::clearMatrixOneByOne() {



}

// ���յ�GameLayer��Ĵ�����Ȼ����в���
void StarMatrix::onTouch(const Point& p) {

	Star *s = getStarByTouch( p );
	if (s) {
		// ��ȡѡ��Ķ���
		genSelectedList( s );

		// ��ӵ���

		CCLOG("size = %d", selectedList.size());
		deleteSelectedList();
	}
}

// ͨ��������õ������е�һ������ķ�����ͨ��һЩ��������������ת���õ��ģ�
Star* StarMatrix::getStarByTouch(const Point& p) {

	int k = (p.y - 100) / Star::STAR_HEIGHT;
	int i = ROW_NUM - 1 - k;
	int j = p.x / Star::STAR_WIDTH;
	// �жϵ�����Ƿ��ھ���������
	if ( i >= 0 && i < ROW_NUM && j >= 0 && j < COL_NUM && stars[i][j] != nullptr ) {
		log("i = %d,j = %d", i, j);
		return stars[i][j];
	} else {
		return nullptr;
	}
}

// �õ�һ�����ӵķ���ĺ���,����ʹ���˹�����ȱ�������������һ������������
void StarMatrix::genSelectedList(Star* s) {

	selectedList.clear();       // ÿ�ε����Ҫ�ȰѴ�ɾ�����б����
	deque<Star*> travelList;    // �¶���һ����������  
	travelList.push_back( s );  // �ѵ���ķ���Ž��¶���
	// ��������
	deque<Star *>::iterator it;
	for (it = travelList.begin(); it != travelList.end();) {
		// ����������Ϊ�յ�ʱ��ֹͣ
		Star *star = *it;
		Star *linkStar = nullptr;
		int index_i = star->getIndexI();    // getter����
		int index_j = star->getIndexJ();
		// ��
		if ( index_i - 1 >= 0 && (linkStar = stars[index_i - 1][index_j]) ) {  // �ж��Ƿ�����Խ��
			// �ж��Ƿ��Ѿ�������ѡ����в�����������еķ�����ɫһ��
			if (!linkStar->isSelected() && linkStar->getColor() == star->getColor()) // ���û�б�����ѡ����У�������ɫһ���ͼ����������
				travelList.push_back(stars[index_i - 1][index_j]);
		}
		//��
		if( index_i + 1 < ROW_NUM  && (linkStar = stars[index_i + 1][index_j]) ){
			if( !linkStar->isSelected() && linkStar->getColor() == star->getColor() )
				travelList.push_back(stars[index_i + 1][index_j]);
		}
		//��
		if( index_j-1 >= 0 && (linkStar = stars[index_i][index_j-1]) ){
			if(!linkStar->isSelected() && linkStar->getColor() == star->getColor())
				travelList.push_back(stars[index_i][index_j - 1]);
		}
		//��
		if( index_j + 1 < COL_NUM && (linkStar = stars[index_i][index_j + 1]) ){
			if(!linkStar->isSelected() && linkStar->getColor() == star->getColor())
				travelList.push_back(stars[index_i][index_j + 1]);
		}

		// ����ǰ���� �������Ѿ������뵽ѡ�������
		if (!star->isSelected()) {
			star->setSelected(true);
			selectedList.push_back(star);        // ���뵽ѡ����У���ɾ�����У�
		}

		travelList.pop_front();    // ��ͷ����
		it  = travelList.begin();  // �õ��µĶ�ͷ
	}
}

void StarMatrix::deleteSelectedList() {

	if (selectedList.size() <= 1) {
		// ���䵥��block���һ������Ч�� (�˴��������Ч)
		star = selectedList.at(0);
		star->setPosition(Point(star->getPositionX(), star->getPositionY()+10));

		selectedList.at(0)->setSelected(false);    // ɾ�����еĵ�һ��(��������Ǹ�����)����Ϊδѡ
		return;
	}
	// ����ѡ�����
	for (auto it = selectedList.begin(); it != selectedList.end(); it++) {
		Star *star = *it;
		// ����Ч��
		showStarParticleEffect(star->getColor(), star->getPosition(), this);
		stars[star->getIndexI()][star->getIndexJ()] = nullptr;   // �������Ԫ���ÿ�
		star->removeFromParentAndCleanup(true);
	}

	adjustMatrix();

}

void StarMatrix::adjustMatrix() {
	// ��ֱ�������
	for (int i = ROW_NUM - 1; i >= 0; i--) {
		for (int j = COL_NUM - 1; j >= 0; j--) {
			if (stars[i][j] == nullptr) {
				int up = i;
				int dis = 0;
				while (stars[up][j] == nullptr) {
					dis++;
					up--;
					if (up < 0) {
						break;
					}
				}
				for (int begin_i = i - dis; begin_i >= 0; begin_i--) {
					if (stars[begin_i][j] == nullptr)
						continue;
					Star *s = stars[begin_i + dis][j] = stars[begin_i][j];
					s->setIndex_ij(begin_i + dis, j);
					s->setDesPosition(getPositionByIndex(begin_i + dis, j));
					stars[begin_i][j] = nullptr;
				}
			} else {
				continue;
			}
		}
	}

	//ˮƽ�������
	for(int j = 0;j < COL_NUM;j++){
		if(stars[ROW_NUM-1][j] == nullptr){
			int des = 0;
			int right = j;
			while(stars[ROW_NUM-1][right] == nullptr){
				des++;
				right++;
			}
			for(int begin_i = 0;begin_i<ROW_NUM;begin_i++){
				for(int begin_j = j + des;begin_j < COL_NUM;begin_j++){
					if(stars[begin_i][begin_j] == nullptr)
						continue;
					Star* s = stars[begin_i][begin_j - des] = stars[begin_i][begin_j];
					s->setIndex_ij(begin_i,begin_j - des);
					s->setDesPosition(getPositionByIndex(begin_i,begin_j - des));
					stars[begin_i][begin_j] = nullptr;
				}
			}
		}
	}

}