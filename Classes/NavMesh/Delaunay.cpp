#include"Delaunay.h"

vector<MyTriangle> Delaunay::createDelaunay(const vector<MyPolygon>& polyV){
	//Step1. 	建立单元大小为 E*E 的均匀网格，并将多边形的顶点和边放入其中.
	//			其中 E=sqrt(w*h/n)，w 和 h 分别为多边形域包围盒的宽度、高度，n 为多边形域的顶点数 .
	initData(polyV);

	//Step2.	取任意一条外边界边 p1p2 .
	Line2D initEdge = getInitOutEdge();
	lineV.push_back(initEdge);

	Line2D edge;
	do{
		//Step3. 	计算 DT 点 p3，构成约束 Delaunay 三角形 Δp1p2p3 .
		edge = lineV.back();
		lineV.pop_back();
		//开始处理edge
		Vector2f* p3 = findDT(edge);
		if (p3 == nullptr)
			continue;

		Line2D line13 = Line2D(edge.getPointA(), *p3);
		Line2D line32 = Line2D(*p3, edge.getPointB());

		//Delaunay三角形放入输出数组
		//MyTriangle trg = MyTriangle(edge.getPointA(), edge.getPointB(), *p3); 
		MyTriangle trg = MyTriangle(*p3, edge.getPointB(), edge.getPointA()); // 顺时针
		triangleV.push_back(trg);
		delete p3;

		//Step4.	如果新生成的边 p1p3 不是约束边，若已经在堆栈中，
		//			则将其从中删除；否则，将其放入堆栈；类似地，可处理 p3p2 .
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
		//Step5.	若堆栈不空，则从中取出一条边，转Step3；否则，算法停止 .
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
	//检查是否有顶点p在该边上，如果有则换一个外边界
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
	} while (loopSign && loopIdx < outEdgeVecNmu - 1); //只取外边界
	return initEdge;
}

void Delaunay::putVertex(vector<Vector2f>& dstV, const vector<Vector2f>& srcV){
	for (auto vec : srcV)
		vertexV.push_back(vec);
}

void Delaunay::putEdge(vector<Line2D>& dstV, const vector<Vector2f>& srcV){
	if (srcV.size() < 3) return; //不是一个多边形
	
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

	//搜索所有可见点 TODO 按y方向搜索距线段终点最近的点

	CCLOG("findDT=========== Line (%f, %f) (%f, %f)", line.getPointA().x, +line.getPointA().y, line.getPointB().x, line.getPointB().y);
	vector<Vector2f> allVPoint = vector<Vector2f>(); // line的所有可见点
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

		//Step1. 构造 Δp1p2p3 的外接圆 C（p1，p2，p3）及其网格包围盒 B（C（p1，p2，p3））
		MyCircle circle = circumCircle(p1, p2, p3);
		Rect boundsBox = circleBounds(circle);

		//Step2. 依次访问网格包围盒内的每个网格单元：
		//		 若某个网格单元中存在可见点 p, 并且 ∠p1pp2 > ∠p1p3p2，则令 p3=p，转Step1；否则，转Step3.
		float angle132 = abs(lineAngle(p1, p3, p2));
		for (auto vec : allVPoint){
			if (vec.equals(p1) || vec.equals(p2) || vec.equals(p3))
				continue;
			//不在包围盒中
			if (boundsBox.containsPoint(Vec2(vec.x, vec.y)) == false)
				continue;
			//夹角
			float a1 = abs(lineAngle(p1, vec, p2));
			if (a1 > angle132){
				//转Step1
				p3 = vec;
				loopSign = true;
				break;
			}
		}

	} while (loopSign);
	//Step3. 若当前网格包围盒内所有网格单元都已被处理完，
	//		 也即C（p1，p2，p3）内无可见点，则 p3 为的 p1p2 的 DT 点
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
	if (dsx*dey - dsy*dex)// 说明矢量os 在矢量 oe的顺时针方向 
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

	//检查重合点
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
	//（1） p3 在边 p1p2 的右侧 (多边形顶点顺序为顺时针)；
	CCLOG("classifyPoint : %f, %f  :SIDE: %d", vec.x, vec.y, line.classifyPoint(vec));
	if (line.classifyPoint(vec) != Line2D::RIGHT_SIDE)
		return false;
	//（2） p3 与 p1 可见，即 p1p3 不与任何一个约束边相交；
	if (isVisibleIn2Point(line.getPointA(), vec) == false){
		CCLOG("isVisibleIn2Point  A false");
		return false;
	}
	//（3） p3 与 p2 可见
	if (isVisibleIn2Point(line.getPointB(), vec) == false){
		CCLOG("isVisibleIn2Point  B false");
		return false;
	}
	CCLOG("isVisibleIn2Point true");
	return true;
}

bool Delaunay::isVisibleIn2Point(const Vector2f& pa, const Vector2f& pb) const{
	Line2D linePaPb = Line2D(pa, pb);
	Vector2f interscetVector = Vector2f(); //线段交点

	for (auto lineTmp : edgeV){
		//两线段相交
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