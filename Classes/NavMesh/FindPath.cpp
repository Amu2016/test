#include"FindPath.h"
#include"graphic\GraphRender.h"

int FindPath::pathSessionId = 0;

Cell* FindPath::findClosestCell(const Vector2f& pt){
	for (auto cell : m_CellVector){
		if (cell->isPointIn(pt))
			return cell;
	}
	return nullptr;
}

vector<Vec2> FindPath::find(const Vec2& startPoint, const Vec2& endPoint){
	pathSessionId++;

	Vector2f startPos = Vector2f(startPoint.x, startPoint.y);
	Vector2f endPos = Vector2f(endPoint.x, endPoint.y);

	Cell* startCell = findClosestCell(startPos);
	Cell* endCell = findClosestCell(endPos);

	if (startCell == nullptr || endCell == nullptr){
		CCLOG("No  Path !!!!!!!!!!!!!!");
		return vector<Vec2>();
	}

	vector<Vec2> outPath;
	if (startCell == endCell){
		outPath.push_back(startPoint);
		outPath.push_back(endPoint);
	}
	else{
		outPath = bulidPath(startCell, startPos, endCell, endPos);
	}
	return outPath;
}

vector<Vec2> FindPath::bulidPath(const Cell* startCell, const Vector2f& startPos, Cell* endCell, const Vector2f& endPos){
	openList.clear();
	closeList = vector<Cell*>();

	openList.put(endCell);
	endCell->f = 0;
	endCell->h = 0;
	endCell->isOpen = false;
	endCell->parent = nullptr;
	endCell->sessionId = pathSessionId;

	bool foundPath = false; //�Ƿ��ҵ�·��
	Cell* currNode; //��ǰ�ڵ�
	Cell* adjacentTmp; //��ǰ�ڵ���ڽ�������

	while (openList.size()){
		// 1. �ѵ�ǰ�ڵ�ӿ����б�ɾ��, ���뵽����б�
		currNode = openList.pop();
		closeList.push_back(currNode);

		//·������ͬһ����������
		if (currNode == startCell){
			foundPath = true;
			break;
		}

		// 2. �Ե�ǰ�ڵ����ڵ�ÿһ���ڵ�����ִ�����²���:
		//�����ڽ�������
		int adjacentId;
		for (int i = 0; i < 3; i++){
			adjacentId = currNode->links[i];
			// 3. ��������ڽڵ㲻��ͨ�л��߸����ڽڵ��Ѿ��ڷ���б���,
			//    ��ʲô����Ҳ��ִ��,����������һ���ڵ�;
			if (adjacentId < 0)
				continue;
			else
				adjacentTmp = m_CellVector[adjacentId];
			//CCLOG("bulidPath  i : &d ----  1" + i);

			if (adjacentTmp != nullptr){
				//CCLOG("bulidPath  i : &d ----  2" + i);
				if (adjacentTmp->sessionId != pathSessionId){
					// 4. ��������ڽڵ㲻�ڿ����б���,�򽫸ýڵ���ӵ������б���, 
					//    ���������ڽڵ�ĸ��ڵ���Ϊ��ǰ�ڵ�,ͬʱ��������ڽڵ��G��Fֵ;
					adjacentTmp->sessionId = pathSessionId;
					adjacentTmp->parent = currNode;
					adjacentTmp->isOpen = true;
					//CCLOG("bulidPath  i : &d ----  3" + i);

					//���뿪���б�����
					openList.put(adjacentTmp);
					//��ס����������Ǵ��ĸ���ڽ�����
					adjacentTmp->setAndGetArrivalWall(currNode->index);
					//H��Fֵ
					adjacentTmp->computeHeuristic(startPos);
					adjacentTmp->f = currNode->f + adjacentTmp->m_WallDistance[abs(i - currNode->m_ArrivalWall)];
					//CCLOG("bulidPath  i : &d ----  4" + i);
				}
				else{
					// 5. ��������ڽڵ��ڿ����б���, 
					//    ���ж������ɵ�ǰ�ڵ㵽������ڽڵ��Gֵ�Ƿ�С��ԭ�������Gֵ,
					//    ��С��,�򽫸����ڽڵ�ĸ��ڵ���Ϊ��ǰ�ڵ�,���������ø����ڽڵ��G��Fֵ
					//CCLOG("bulidPath  i : &d ----  5" + i);
					if (adjacentTmp->isOpen){//�Ѿ���openList��
						if ((currNode->f + adjacentTmp->m_WallDistance[abs(i - currNode->m_ArrivalWall)]) < adjacentTmp->f){
							adjacentTmp->f = currNode->f;
							adjacentTmp->parent = currNode;

							//��ס����������Ǵ��ĸ���ڽ�����
							//CCLOG("bulidPath  i : &d ----  6" + i);
							adjacentTmp->setAndGetArrivalWall(currNode->index);
						}
					}
					else{//����closeList��
						adjacentTmp = nullptr;
						continue;
					}
				}
			}
		}
	}
	//������·������Point����·��
	if (foundPath) {
		return getPath(Vec2(startPos.x, startPos.y), Vec2(endPos.x, endPos.y));
	}
	else {
		return vector<Vec2>();
	}
}

vector<Cell*> FindPath::getCellPath(){
	vector<Cell*> pth;

	Cell* st = closeList[closeList.size() - 1];
	pth.push_back(st);

	int i = 1;
	while (st->parent != nullptr){
		pth.push_back(st->parent);
		st = st->parent;
		i++;
		if (i > 1000)
			break;
	}
	CCLOG("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!path length :: %d", i);
	return pth;
}

vector<Vec2> FindPath::getPath(const Vec2& start, const Vec2& end){
	//������������
	vector<Cell*> cellPath = getCellPath();
	//if (cellPath.size() == 0)
	//�������յ�·����Point���飩


	vector<Vec2> path;
	path.push_back(Vec2(start.x, start.y));
	//������յ���ͬһ��������
	if (cellPath.size() == 1){
		path.push_back(Vec2(end.x, end.y));
		return path;
	}

	//��ȡ·��
	WayPoint wayPoint(cellPath[0], Vector2f(start.x, start.y));
	while (!wayPoint.position.equals(Vector2f(end.x, end.y))){
		wayPoint = getFurthestWayPoint(wayPoint, cellPath, Vector2f(end.x, end.y));
		path.push_back(Vec2(wayPoint.position.x, wayPoint.position.y));
	}
	return path;
}

WayPoint FindPath::getFurthestWayPoint(const WayPoint& wayPoint, const vector<Cell*>& cellPath, const Vector2f& end){
	Vector2f startPt = wayPoint.position;
	Cell* cell = wayPoint.cell;
	Cell* lastCell = wayPoint.cell;
	int startIndex = indexOf(cellPath, cell);  //��ʼ·�����ڵ���������
	Line2D outSide = cell->_sides[cell->m_ArrivalWall];  //·�����������еĴ�����
	Vector2f lastPtA = outSide.getPointA();
	Vector2f lastPtB = outSide.getPointB();
	Line2D lastLineA(startPt, lastPtA);
	Line2D lastLineB(startPt, lastPtB);

	Vector2f testPtA, testPtB;  //Ҫ���Եĵ�

	CCLOG("-----------BEGIN startPt (%d, %d)", (int)startPt.x, (int)startPt.y);
	CCLOG("lastPtA (%d, %d)", (int)lastPtA.x, (int)lastPtA.y);
	CCLOG("lastPtB (%d, %d)", (int)lastPtB.x, (int)lastPtB.y);


	for (unsigned int i = startIndex + 1; i < cellPath.size(); i++){
		cell = cellPath[i];
		outSide = cell->_sides[cell->m_ArrivalWall];
		if (i == cellPath.size() - 1){
			testPtA = end;
			testPtB = end;
		}
		else{
			testPtA = outSide.getPointA();
			testPtB = outSide.getPointB();
		}

		CCLOG("testPtA (%d, %d)", (int)testPtA.x, (int)testPtA.y);
		CCLOG("testPtB (%d, %d)", (int)testPtB.x, (int)testPtB.y);

		if (!lastPtA.equals(testPtA)){
			CCLOG("!lastPtA.equals(testPtA)  %d", lastLineB.classifyPoint(testPtA));
			if (lastLineB.classifyPoint(testPtA) == Line2D::LEFT_SIDE){
				//·��
				CCLOG("lastLineB.classifyPoint POINT lastPtB (%d %d)", (int)lastPtB.x, (int)lastPtB.y);
				return WayPoint(lastCell, lastPtB);
			}
			else{
				if (lastLineA.classifyPoint(testPtA) != Line2D::RIGHT_SIDE){
					lastPtA = testPtA;
					lastCell = cell;
					//����ֱ��
					CCLOG("---------lastPtA (%d, %d)", (int)lastPtA.x, (int)lastPtA.y);
					lastLineA.setPointB(lastPtA);
				}
			}
		}

		if (!lastPtB.equals(testPtB)){
			CCLOG("!lastPtB.equals(testPtB)  %d", lastLineA.classifyPoint(testPtB));
			if (lastLineA.classifyPoint(testPtB) == Line2D::RIGHT_SIDE) {
				//·����
				CCLOG("lastLineA.classifyPoint POINT lastPtA (%d, %d)", (int)lastPtA.x, (int)lastPtA.y);
				return WayPoint(lastCell, lastPtA);
			}
			else {
				if (lastLineB.classifyPoint(testPtB) != Line2D::LEFT_SIDE) {
					lastPtB = testPtB;
					lastCell = cell;
					//����ֱ��
					CCLOG("---------- lastPtB (%d, %d)", (int)lastPtB.x, (int)lastPtB.y);
					lastLineB.setPointB(lastPtB);
				}
			}
		}
	}

	return WayPoint(cellPath[cellPath.size() - 1], end);
}

int FindPath::indexOf(const vector<Cell*>& cellPath, const Cell* cell){
	for (unsigned int i = 0; i < cellPath.size(); i++){
		if (cell == cellPath[i])
			return i;
	}
	return -1;
}