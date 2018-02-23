#include"MyPolygon.h"

bool MyPolygon::isSimplicity() const{
	// ������
	vector<Line2D> edges;
	for (unsigned int i = 0; i < vertexV.size(); i++)
		edges.push_back(Line2D(vertexV[i], vertexV[i + 1]));
	edges.push_back(Line2D(vertexV[vertexV.size() - 1], vertexV[0]));

	// �Ƿ����ڽ���
	Vector2f itsPt;
	for (auto line : edges){
		for (unsigned int i = 0; i < edges.size(); i++){
			if (!line.equals(edges[i])){
				if (line.intersection(edges[i], itsPt) == Line2D::SEGMENTS_INTERSECT){
					//�����������߶εĶ˵�
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

	//���ϣ�y��С������x��С���㣬 �϶���һ��͹��
	//Ѱ�����ϵ�

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

	//͹����ڵ�
	int lastId = topPtId - 1 >= 0 ? topPtId - 1 : vertexV.size() - 1;
	int nextId = topPtId + 1 >= vertexV.size() ? 0 : topPtId + 1;
	Vector2f last = vertexV[lastId];
	Vector2f next = vertexV[nextId];

	//�ж�
	float r = multiply(last, next, topPt);
	if (r < 0)
		return true;
	//else if (r == 0)
	//���㹲����������ڣ������㹲����˵������һ���y��б�ߣ���x��ˮƽ�ߣ�С��topPt
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
	//��Χ�в��ཻ
	if (rectangle()->intersectsRect(*(polygon.rectangle())) == false)
		return nullptr;

	//���ж���ͽ���
	vector<MyNode*> cv0; //�������
	vector<MyNode*> cv1; //�ϲ������

	//��ʼ��
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

	//���뽻��
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
	//����ϲ���Ķ��������
	vector<MyPolygon>* rtV = new vector<MyPolygon>();

	//1. ѡȡ��һû�б����ٹ��Ľ���Ϊʼ�㣬����������������ζ�����У�
	for (MyNode* node : cv0){
		if (node->i && !node->p){
			vector<Vector2f> rcNodes;

			while (node)
			{
				node->p = true;

				// ����ǽ���
				if (node->i){
					node->other->p = true;

					if (node->o == false){ //�ý���Ϊ���㣨���ٲü�����α߽磩
						if (node->isMain) //��ǰ�������������
							node = node->other; //�л����ü��������
					}
					else{ //�ý���Ϊ���㣨����������α߽磩
						if (!node->isMain) // ��ǰ���ڲü��������
							node = node->other; //�л������������
					}
				}

				rcNodes.push_back(node->v); // ����Ƕ���ζ��㣬����������������ζ������

				if (!node->next){ //ĩβ�㷵�ص���ʼ��
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
				//���׵���ͬ������һ�������
				if (node->v.equals(rcNodes[0]))
					break;
			}
			//��ȡ
			rtV->push_back(MyPolygon(rcNodes.size(), rcNodes));
		}
	}
	return rtV;
}

int MyPolygon::intersectPoint(vector<MyNode*>& cv0, vector<MyNode*>& cv1){
	int insCnt = 0; //������
	bool findEnd = false;
	MyNode* startNode0 = cv0[0];
	MyNode* startNode1;
	Line2D line0;
	Line2D line1;
	Vector2f ins;
	bool hasIns;
	//int result; //�������жϽ��

	while (startNode0 != nullptr){ //�������
		if (startNode0->next == nullptr) //���һ���㣬���׵�����
			line0 = Line2D(startNode0->v, cv0[0]->v);
		else
			line0 = Line2D(startNode0->v, startNode0->next->v);

		startNode1 = cv1[0];
		hasIns = false;

		while (startNode1 != nullptr){ //�ϲ������
			if (startNode1->next == nullptr) //���һ���㣬���׵�����
				line1 = Line2D(startNode1->v, cv1[0]->v);
			else
				line1 = Line2D(startNode1->v, startNode1->next->v);

			ins = Vector2f();  //���ܷ��صĽ���
			//�н���
			if (line0.intersection(line1, ins) == Line2D::SEGMENTS_INTERSECT){
				//���Խ������ڶ����б��е�
				if (getNodeIndex(cv0, ins) == -1){
					insCnt++;

					// ���뽻��
					MyNode* node0 = new MyNode(ins, true, true);
					MyNode* node1 = new MyNode(ins, true, false);
					cv0.push_back(node0);
					cv1.push_back(node1);

					//˫������
					node0->other = node1;
					node1->other = node0;

					//����
					node0->next = startNode0->next;
					startNode0->next = node0;
					node1->next = startNode1->next;
					startNode1->next = node1;
					//����
					if (line0.classifyPoint(line1.getPointB()) == Line2D::LEFT_SIDE){
						node0->o = true;
						node1->o = true;
					}

					//TODO �߶��غ�

					hasIns = true; //�н��㣬�������´���
					break;
				}
			}
			startNode1 = startNode1->next;
		}
		//���û�н������������һ���ߣ��������´���õ������Ľ������γɵ��߶�
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