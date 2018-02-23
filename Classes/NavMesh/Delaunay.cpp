#include"Delaunay.h"

vector<MyTriangle> Delaunay::createDelaunay(const vector<MyPolygon>& polyV){
	//Step1. 	������Ԫ��СΪ E*E �ľ������񣬲�������εĶ���ͱ߷�������.
	//			���� E=sqrt(w*h/n)��w �� h �ֱ�Ϊ��������Χ�еĿ�ȡ��߶ȣ�n Ϊ�������Ķ����� .
	initData(polyV);

	//Step2.	ȡ����һ����߽�� p1p2 .
	Line2D initEdge = getInitOutEdge();
	lineV.push_back(initEdge);

	Line2D edge;
	do{
		//Step3. 	���� DT �� p3������Լ�� Delaunay ������ ��p1p2p3 .
		edge = lineV.back();
		lineV.pop_back();
		//��ʼ����edge
		Vector2f* p3 = findDT(edge);
		if (p3 == nullptr)
			continue;

		Line2D line13 = Line2D(edge.getPointA(), *p3);
		Line2D line32 = Line2D(*p3, edge.getPointB());

		//Delaunay�����η����������
		//MyTriangle trg = MyTriangle(edge.getPointA(), edge.getPointB(), *p3); 
		MyTriangle trg = MyTriangle(*p3, edge.getPointB(), edge.getPointA()); // ˳ʱ��
		triangleV.push_back(trg);
		delete p3;

		//Step4.	��������ɵı� p1p3 ����Լ���ߣ����Ѿ��ڶ�ջ�У�
		//			�������ɾ�������򣬽�������ջ�����Ƶأ��ɴ��� p3p2 .
		int index;
		if (indexOfVector(line13, edgeV) < 0){
			index = indexOfVector(line13, lineV);
			if (index > -1)
				lineV.erase(lineV.begin() + index);
			else
				lineV.push_back(line13);
		}
		if (indexOfVector(line32, edgeV) < 0){
			index = indexOfVector(line32, lineV);
			if (index > -1)
				lineV.erase(lineV.begin() + index);
			else
				lineV.push_back(line32);
		}
		//Step5.	����ջ���գ������ȡ��һ���ߣ�תStep3�������㷨ֹͣ .
	} while (lineV.size() > 0);
	return triangleV;
}

void Delaunay::initData(const vector<MyPolygon>& polyV){
	vertexV = vector<Vector2f>();
	edgeV = vector<Line2D>();

	MyPolygon poly;
	for (unsigned int i = 0; i < polyV.size(); i++){
		poly = polyV[i];
		putVertex(vertexV, poly.vertexV);
		putEdge(edgeV, poly.vertexV);
	}

	outEdgeVecNmu = polyV[0].vertexNum;

	lineV = vector<Line2D>();
	triangleV = vector<MyTriangle>();
}

Line2D Delaunay::getInitOutEdge(){
	Line2D initEdge = edgeV[0];
	//����Ƿ��ж���p�ڸñ��ϣ��������һ����߽�
	bool loopSign;
	int loopIdx = 0;

	do{
		loopSign = false;
		loopIdx++;
		for (auto testV : vertexV){
			if (testV.equals(initEdge.getPointA()) || testV.equals(initEdge.getPointB()))
				continue;
			if (initEdge.classifyPoint(testV) == Line2D::ON_LINE){
				loopSign = true;
				initEdge = edgeV[loopIdx];
				break;
			}
		}
	} while (loopSign && loopIdx < outEdgeVecNmu - 1); //ֻȡ��߽�
	return initEdge;
}

void Delaunay::putVertex(vector<Vector2f>& dstV, const vector<Vector2f>& srcV){
	for (auto vec : srcV)
		vertexV.push_back(vec);
}

void Delaunay::putEdge(vector<Line2D>& dstV, const vector<Vector2f>& srcV){
	if (srcV.size() < 3) return; //����һ�������
	
	Vector2f p1 = srcV[0];
	Vector2f p2;
	for (unsigned int i = 1; i < srcV.size(); i++){
		p2 = srcV[i];
		edgeV.push_back(Line2D(p1, p2));
		p1 = p2;
	}
	p2 = srcV[0];
	edgeV.push_back(Line2D(p1, p2));
}

int Delaunay::indexOfVector(const Line2D& line, const vector<Line2D>& vec){
	Line2D lt;
	for (unsigned int i = 0; i < vec.size(); i++){
		lt = vec[i];
		if (lt.equals(line))
			return i;
	}
	return -1;
}

Vector2f* Delaunay::findDT(Line2D& line){
	Vector2f p1 = line.getPointA();
	Vector2f p2 = line.getPointB();

	//�������пɼ��� TODO ��y�����������߶��յ�����ĵ�

	CCLOG("findDT=========== Line (%f, %f) (%f, %f)", line.getPointA().x, +line.getPointA().y, line.getPointB().x, line.getPointB().y);
	vector<Vector2f> allVPoint = vector<Vector2f>(); // line�����пɼ���
	for (auto vt : vertexV){
		if (isVisiblePointOfLine(vt, line)){
			allVPoint.push_back(vt);
			CCLOG("findDT  allVPoint %f, %f", vt.x, vt.y);
		}
	}

	if (allVPoint.size() == 0)
		return nullptr;

	Vector2f p3 = allVPoint[0];
	bool loopSign = false;
	do{
		loopSign = false;

		//Step1. ���� ��p1p2p3 �����Բ C��p1��p2��p3�����������Χ�� B��C��p1��p2��p3����
		MyCircle circle = circumCircle(p1, p2, p3);
		Rect boundsBox = circleBounds(circle);

		//Step2. ���η��������Χ���ڵ�ÿ������Ԫ��
		//		 ��ĳ������Ԫ�д��ڿɼ��� p, ���� ��p1pp2 > ��p1p3p2������ p3=p��תStep1������תStep3.
		float angle132 = abs(lineAngle(p1, p3, p2));
		for (auto vec : allVPoint){
			if (vec.equals(p1) || vec.equals(p2) || vec.equals(p3))
				continue;
			//���ڰ�Χ����
			if (boundsBox.containsPoint(Vec2(vec.x, vec.y)) == false)
				continue;
			//�н�
			float a1 = abs(lineAngle(p1, vec, p2));
			if (a1 > angle132){
				//תStep1
				p3 = vec;
				loopSign = true;
				break;
			}
		}

	} while (loopSign);
	//Step3. ����ǰ�����Χ������������Ԫ���ѱ������꣬
	//		 Ҳ��C��p1��p2��p3�����޿ɼ��㣬�� p3 Ϊ�� p1p2 �� DT ��
	return new Vector2f(p3.x, p3.y);
}

float Delaunay::lineAngle(const Vector2f& s, const Vector2f& o, const Vector2f& e){
	float cosfi, fi, norm;
	float dsx = s.x - o.x;
	float dsy = s.y - o.y;
	float dex = e.x - o.x;
	float dey = e.y - o.y;

	cosfi = dsx*dex + dsy*dey;
	norm = (dsx*dsx + dsy*dsy)*(dex*dex + dey*dey);
	cosfi /= sqrt(norm);

	if (cosfi >= 1.0f) return 0;
	if (cosfi <= -1.0f) return (float)PI;
	fi = acos(cosfi);
	if (dsx*dey - dsy*dex)// ˵��ʸ��os ��ʸ�� oe��˳ʱ�뷽�� 
		return fi;
	return -fi;
}

Rect Delaunay::circleBounds(const MyCircle& c){
	return Rect(c.center.x - c.radius, c.center.y - c.radius, c.radius * 2, c.radius * 2);
}

MyCircle Delaunay::circumCircle(const Vector2f& p1, const Vector2f& p2, const Vector2f& p3){
	float m1, m2, mx1, mx2, my1, my2;
	float dx, dy, rsqr;
	float xc, yc, r;

	//����غϵ�
	if (abs((int)(p1.y - p2.y)) < 0.000001f && abs((int)(p2.y - p3.y)) < 0.01f){
		CCLOG("error:circumCircle!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
		return MyCircle();
	}

	m1 = -(p2.x - p1.x) / (p2.y - p1.y);
	m2 = -(p3.x - p2.x) / (p3.y - p2.y);
	mx1 = (p1.x + p2.x) / 2.0;
	mx2 = (p2.x + p3.x) / 2.0;
	my1 = (p1.y + p2.y) / 2.0;
	my2 = (p2.y + p3.y) / 2.0;

	if (abs((int)(p2.y - p1.y)) < 0.01f){
		xc = (p2.x + p1.x) / 2.0;
		yc = m2 * (xc - mx2) + my2;
	}
	else if (abs((int)(p3.y - p2.y)) < 0.01f){
		xc = (p3.x + p2.x) / 2.0;
		yc = m1 * (xc - mx1) + my1;
	}
	else {
		xc = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);
		yc = m1 * (xc - mx1) + my1;
	}
	dx = p2.x - xc;
	dy = p2.y - yc;
	rsqr = dx*dx + dy*dy;
	r = sqrt(rsqr);

	return MyCircle(Vector2f(xc, yc), r);
}

bool Delaunay::isVisiblePointOfLine(const Vector2f& vec, Line2D& line) const{
	if (vec.equals(line.getPointA()) || vec.equals(line.getPointB()))
		return false;
	//��1�� p3 �ڱ� p1p2 ���Ҳ� (����ζ���˳��Ϊ˳ʱ��)��
	CCLOG("classifyPoint : %f, %f  :SIDE: %d", vec.x, vec.y, line.classifyPoint(vec));
	if (line.classifyPoint(vec) != Line2D::RIGHT_SIDE)
		return false;
	//��2�� p3 �� p1 �ɼ����� p1p3 �����κ�һ��Լ�����ཻ��
	if (isVisibleIn2Point(line.getPointA(), vec) == false){
		CCLOG("isVisibleIn2Point  A false");
		return false;
	}
	//��3�� p3 �� p2 �ɼ�
	if (isVisibleIn2Point(line.getPointB(), vec) == false){
		CCLOG("isVisibleIn2Point  B false");
		return false;
	}
	CCLOG("isVisibleIn2Point true");
	return true;
}

bool Delaunay::isVisibleIn2Point(const Vector2f& pa, const Vector2f& pb) const{
	Line2D linePaPb = Line2D(pa, pb);
	Vector2f interscetVector = Vector2f(); //�߶ν���

	for (auto lineTmp : edgeV){
		//���߶��ཻ
		if (linePaPb.intersection(lineTmp, interscetVector) == Line2D::SEGMENTS_INTERSECT){
			if (!pa.equals(interscetVector) && !pb.equals(interscetVector)){
				CCLOG("linePaPb=========== linePaPb (%f, %f) (%f, %f)",
					linePaPb.getPointA().x, +linePaPb.getPointA().y, linePaPb.getPointB().x, linePaPb.getPointB().y);
				CCLOG("linePaPb=========== linePaPb (%f, %f) (%f, %f)", 
					lineTmp.getPointA().x, +lineTmp.getPointA().y, lineTmp.getPointB().x, lineTmp.getPointB().y);
				CCLOG("interscetVector : %f, %f  ", interscetVector.x, interscetVector.y);
				return false;
			}
		}
	}
	return true;
}