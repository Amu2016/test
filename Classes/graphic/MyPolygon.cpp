#include"MyPolygon.h"

bool MyPolygon::isSimplicity() const{
	// 边数组
	vector<Line2D> edges;
	for (unsigned int i = 0; i < vertexV.size(); i++)
		edges.push_back(Line2D(vertexV[i], vertexV[i + 1]));
	edges.push_back(Line2D(vertexV[vertexV.size() - 1], vertexV[0]));

	// 是否有内交点
	Vector2f itsPt;
	for (auto line : edges){
		for (unsigned int i = 0; i < edges.size(); i++){
			if (!line.equals(edges[i])){
				if (line.intersection(edges[i], itsPt) == Line2D::SEGMENTS_INTERSECT){
					//交点是两个线段的端点
					if (itsPt.equals(line.getPointA()) || itsPt.equals(line.getPointB())
						|| itsPt.equals(edges[i].getPointA()) || itsPt.equals(edges[i].getPointB())){

					}
					else
						return false;
				}
			}
		}
	}
	return true;
}

void MyPolygon::cw(){
	if (isCW())
		reverse(vertexV.begin(), vertexV.end());
}

bool MyPolygon::isCW(){
	if (vertexV.size() <= 0)
		return false;

	//最上（y最小）最左（x最小）点， 肯定是一个凸点
	//寻找最上点

	Vector2f topPt(vertexV[0]);
	int topPtId = 0;
	for (unsigned int i = 1; i < vertexV.size(); i++){
		if (topPt.y > vertexV[i].y){
			topPt = vertexV[i];
			topPtId = i;
		}
		else if (topPt.y == vertexV[i].y){
			if (topPt.x > vertexV[i].x){
				topPt = vertexV[i];
				topPtId = i;
			}
		}
	}

	//凸点的邻点
	int lastId = topPtId - 1 >= 0 ? topPtId - 1 : vertexV.size() - 1;
	int nextId = topPtId + 1 >= vertexV.size() ? 0 : topPtId + 1;
	Vector2f last = vertexV[lastId];
	Vector2f next = vertexV[nextId];

	//判断
	float r = multiply(last, next, topPt);
	if (r < 0)
		return true;
	//else if (r == 0)
	//三点共线情况不存在，若三点共线则说明必有一点的y（斜线）或x（水平线）小于topPt
	return false;
}

float MyPolygon::multiply(const Vector2f& sp, const Vector2f& ep, const Vector2f& op){
	return ((sp.x - op.x)*(ep.y - op.y) - (ep.x - op.x)*(sp.y - op.y));
}

Rect* MyPolygon::rectangle(){
	if (!vertexV.size())
		return nullptr;

	if (rectBox != nullptr)
		return rectBox;

	float lx = vertexV[0].x;
	float rx = vertexV[0].x;
	float ty = vertexV[0].y;
	float by = vertexV[0].y;

	Vector2f v;
	for (unsigned int i = 1; i < vertexV.size(); i++){
		v = vertexV[i];
		if (v.x < lx)
			lx = v.x;
		if (v.x > rx)
			rx = v.x;
		if (v.y < ty)
			ty = v.y;
		if (v.y > by)
			by = v.y;
	}
	rectBox = new Rect(lx, ty, rx - lx, by - ty);
	return rectBox;
}

vector<MyPolygon>* MyPolygon::combination(MyPolygon& polygon){
	//包围盒不相交
	if (rectangle()->intersectsRect(*(polygon.rectangle())) == false)
		return nullptr;

	//所有顶点和交点
	vector<MyNode*> cv0; //主多边形
	vector<MyNode*> cv1; //合并多边形

	//初始化
	MyNode* node = new MyNode();

	for (unsigned int i = 0; i < vertexV.size(); i++){
		node = new MyNode(vertexV[i], false, true);
		if (i > 0)
			cv0[i - 1]->next = node;
		cv0.push_back(node);
	}
	for (unsigned int i = 0; i < polygon.vertexV.size(); i++){
		node = new MyNode(polygon.vertexV[i], false, false);
		if (i > 0)
			cv1[i - 1]->next = node;
		cv1.push_back(node);
	}

	//插入交点
	int insCnt = intersectPoint(cv0, cv1);

	auto rtV = linkToPolygon(cv0, cv1);

	for (auto node : cv0)
		delete node;
	for (auto node : cv1)
		delete node;

	if (insCnt > 0)
		return rtV;
	else
		return nullptr;
}

vector<MyPolygon>* MyPolygon::linkToPolygon(vector<MyNode*>& cv0, vector<MyNode*>& cv1){
	//保存合并后的多边形数组
	vector<MyPolygon>* rtV = new vector<MyPolygon>();

	//1. 选取任一没有被跟踪过的交点为始点，将其输出到结果多边形顶点表中．
	for (MyNode* node : cv0){
		if (node->i && !node->p){
			vector<Vector2f> rcNodes;

			while (node)
			{
				node->p = true;

				// 如果是交点
				if (node->i){
					node->other->p = true;

					if (node->o == false){ //该交点为进点（跟踪裁剪多边形边界）
						if (node->isMain) //当前点在主多边形中
							node = node->other; //切换到裁剪多边形中
					}
					else{ //该交点为出点（跟踪主多边形边界）
						if (!node->isMain) // 当前点在裁剪多边形中
							node = node->other; //切换到主多边形中
					}
				}

				rcNodes.push_back(node->v); // 如果是多边形顶点，将其输出到结果多边形顶点表中

				if (!node->next){ //末尾点返回到开始点
					if (node->isMain){
						node = cv0[0];
					}
					else{
						node = cv1[0];
					}
				}
				else{
					node = node->next;
				}
				//与首点相同，生成一个多边形
				if (node->v.equals(rcNodes[0]))
					break;
			}
			//提取
			rtV->push_back(MyPolygon(rcNodes.size(), rcNodes));
		}
	}
	return rtV;
}

int MyPolygon::intersectPoint(vector<MyNode*>& cv0, vector<MyNode*>& cv1){
	int insCnt = 0; //交点数
	bool findEnd = false;
	MyNode* startNode0 = cv0[0];
	MyNode* startNode1;
	Line2D line0;
	Line2D line1;
	Vector2f ins;
	bool hasIns;
	//int result; //进出点判断结果

	while (startNode0 != nullptr){ //主多边形
		if (startNode0->next == nullptr) //最后一个点，跟首点相连
			line0 = Line2D(startNode0->v, cv0[0]->v);
		else
			line0 = Line2D(startNode0->v, startNode0->next->v);

		startNode1 = cv1[0];
		hasIns = false;

		while (startNode1 != nullptr){ //合并多边形
			if (startNode1->next == nullptr) //最后一个点，跟首点相连
				line1 = Line2D(startNode1->v, cv1[0]->v);
			else
				line1 = Line2D(startNode1->v, startNode1->next->v);

			ins = Vector2f();  //接受返回的交点
			//有交点
			if (line0.intersection(line1, ins) == Line2D::SEGMENTS_INTERSECT){
				//忽略交点已在顶点列表中的
				if (getNodeIndex(cv0, ins) == -1){
					insCnt++;

					// 插入交点
					MyNode* node0 = new MyNode(ins, true, true);
					MyNode* node1 = new MyNode(ins, true, false);
					cv0.push_back(node0);
					cv1.push_back(node1);

					//双向引用
					node0->other = node1;
					node1->other = node0;

					//插入
					node0->next = startNode0->next;
					startNode0->next = node0;
					node1->next = startNode1->next;
					startNode1->next = node1;
					//出点
					if (line0.classifyPoint(line1.getPointB()) == Line2D::LEFT_SIDE){
						node0->o = true;
						node1->o = true;
					}

					//TODO 线段重合

					hasIns = true; //有交点，返回重新处理
					break;
				}
			}
			startNode1 = startNode1->next;
		}
		//如果没有交点继续处理下一个边，否则重新处理该点与插入的交点所形成的线段
		if (hasIns == false)
			startNode0 = startNode0->next;
	}
	return insCnt;
}

int MyPolygon::getNodeIndex(const vector<MyNode*>& cv, const Vector2f& node){
	for (unsigned int i = 0; i < cv.size(); i++){
		if (cv[i]->v.equals(node))
			return i;
	}
	return -1;
}

void MyPolygon::fullMyPolygon(Node* parent){
	//GraphRender::getInstance()->fillPolygon(vertexV, Color4F(0, 1, 0, 20));
}

void MyPolygon::clear(){
	if (polyFill != nullptr)
		polyFill->removeFromParent();
	for (auto& line : polyLine){
		if (line != nullptr)
			line->removeFromParent();
	}

	for (auto& text : polyPointPos){
		if (text != nullptr)
			text->removeFromParent();
	}
}