#include "StarMatrix.h"
#include "StarParticle.h"

#include <ctime>

float StarMatrix::ONE_CLEAR_TIME = 0.05f;

// 对象初始化以及内存管理
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
	// 将数组指向的那些内存都初始化为零 防止哪些内存有的已经被占用
	memset(stars, 0, sizeof(Star*) * ROW_NUM * COL_NUM);
	initMatrix();    // 初始化星星矩阵 

	return true;
}

void StarMatrix::initMatrix() {
	// 加入随机种子
	srand(time(0));
	for (int i = 0; i < ROW_NUM; i++) {
		for (int j = 0; j < COL_NUM; j++) {

			int color = abs(rand() % Star::COLOR_MAX_NUM);  // 随机生成数字0-5
			if (color < 0) {
				log ("color i = %d, j = %d");
			}
			star = Star::create(color);
			stars[i][j] = star;       // 将创建出来的星星放入数组中
			star->setPosition(getPositionByIndex(i, j) + Point(0, 100));    // y方向加100是初始位置 为了添加落下的动画
			star->setDesPosition(getPositionByIndex(i, j));
			star->setIndex_ij(i, j);  // 调用setter方法将值赋给Star
			this->addChild(star);

		}
	}
}

// 获取每个星星像素的位置(位置i,j是从矩阵的左上角开始递归,然而cocos2d的坐标系是在左下角)
Point StarMatrix::getPositionByIndex(int i,int j) {

	float x = j * Star::STAR_WIDTH + Star::STAR_WIDTH / 2;
	float y = (StarMatrix::COL_NUM - i) * Star::STAR_HEIGHT - Star::STAR_HEIGHT / 2;
	return Point(x, y + 100);
}

// 在GameLayer里面调用
void StarMatrix::updateStar(float delta) {

	for (int i = 0; i < ROW_NUM; i++) {
		for (int j = 0; j < COL_NUM; j++) {
			if (stars[i][j] != nullptr) {
				// 整个矩阵调用Star类的updatePosition()方法
				stars[i][j]->updatePosition();
			}
		}
	}
	// 当间隔时间达到一定时间后，消除星星
	if (needClear) {
		clearSumTime += delta;
		if (clearSumTime > ONE_CLEAR_TIME) {
			//clearMatrixOneByOne();
			clearSumTime = 0;            // clearSumTime是一个累加器
		}
	}
}

void StarMatrix::setNeedClear(bool b) {

	needClear = b;
}

void StarMatrix::clearMatrixOneByOne() {



}

// 接收到GameLayer层的触摸点然后进行操作
void StarMatrix::onTouch(const Point& p) {

	Star *s = getStarByTouch( p );
	if (s) {
		// 获取选择的队列
		genSelectedList( s );

		// 添加道具

		CCLOG("size = %d", selectedList.size());
		deleteSelectedList();
	}
}

// 通过触摸点得到矩阵中的一个方块的方法（通过一些像素与矩阵坐标的转换得到的）
Star* StarMatrix::getStarByTouch(const Point& p) {

	int k = (p.y - 100) / Star::STAR_HEIGHT;
	int i = ROW_NUM - 1 - k;
	int j = p.x / Star::STAR_WIDTH;
	// 判断点击的是否在矩阵区域内
	if ( i >= 0 && i < ROW_NUM && j >= 0 && j < COL_NUM && stars[i][j] != nullptr ) {
		log("i = %d,j = %d", i, j);
		return stars[i][j];
	} else {
		return nullptr;
	}
}

// 得到一串连接的方块的函数,这里使用了广度优先遍历，并且用了一个队列来辅助
void StarMatrix::genSelectedList(Star* s) {

	selectedList.clear();       // 每次点击都要先把待删除队列表清空
	deque<Star*> travelList;    // 新定义一个遍历队列  
	travelList.push_back( s );  // 把点击的方块放进新队列
	// 遍历队列
	deque<Star *>::iterator it;
	for (it = travelList.begin(); it != travelList.end();) {
		// 当遍历队列为空的时候停止
		Star *star = *it;
		Star *linkStar = nullptr;
		int index_i = star->getIndexI();    // getter方法
		int index_j = star->getIndexJ();
		// 上
		if ( index_i - 1 >= 0 && (linkStar = stars[index_i - 1][index_j]) ) {  // 判断是否数组越界
			// 判断是否已经被纳入选择队列并且与遍历队列的方块颜色一样
			if (!linkStar->isSelected() && linkStar->getColor() == star->getColor()) // 如果没有被纳入选择队列，并且颜色一样就加入遍历队列
				travelList.push_back(stars[index_i - 1][index_j]);
		}
		//下
		if( index_i + 1 < ROW_NUM  && (linkStar = stars[index_i + 1][index_j]) ){
			if( !linkStar->isSelected() && linkStar->getColor() == star->getColor() )
				travelList.push_back(stars[index_i + 1][index_j]);
		}
		//左
		if( index_j-1 >= 0 && (linkStar = stars[index_i][index_j-1]) ){
			if(!linkStar->isSelected() && linkStar->getColor() == star->getColor())
				travelList.push_back(stars[index_i][index_j - 1]);
		}
		//右
		if( index_j + 1 < COL_NUM && (linkStar = stars[index_i][index_j + 1]) ){
			if(!linkStar->isSelected() && linkStar->getColor() == star->getColor())
				travelList.push_back(stars[index_i][index_j + 1]);
		}

		// 处理当前方块 并设置已经被加入到选择队列中
		if (!star->isSelected()) {
			star->setSelected(true);
			selectedList.push_back(star);        // 加入到选择队列（待删除队列）
		}

		travelList.pop_front();    // 队头出队
		it  = travelList.begin();  // 得到新的队头
	}
}

void StarMatrix::deleteSelectedList() {

	if (selectedList.size() <= 1) {
		// 给落单的block添加一个颤动效果 (此处需添加音效)
		star = selectedList.at(0);
		star->setPosition(Point(star->getPositionX(), star->getPositionY()+10));

		selectedList.at(0)->setSelected(false);    // 删除队列的第一个(即点击的那个方块)设置为未选
		return;
	}
	// 遍历选择队列
	for (auto it = selectedList.begin(); it != selectedList.end(); it++) {
		Star *star = *it;
		// 粒子效果
		showStarParticleEffect(star->getColor(), star->getPosition(), this);
		stars[star->getIndexI()][star->getIndexJ()] = nullptr;   // 将数组该元素置空
		star->removeFromParentAndCleanup(true);
	}

	adjustMatrix();

}

void StarMatrix::adjustMatrix() {
	// 垂直方向调整
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

	//水平方向调整
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