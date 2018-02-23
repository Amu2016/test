#ifndef _scene_map_mgr_h_
#define _scene_map_mgr_h_

#include"graphic\MyPolygon.h"
#include"graphic\MyTriangle.h"

class MapMgr{
public:
	static MapMgr* getInstance(){
		static MapMgr* mapMgr = new MapMgr();
		return mapMgr;
	}

	void setPolygon(const vector<MyPolygon>& vecPoly){
		polygonV = vecPoly;
	}
	void setTriangleV(const vector<MyTriangle>& vecTrig){
		triangleV = vecTrig;
	}

	vector<MyPolygon> getPolygon() const{ return polygonV; }
	vector<MyTriangle> getTriangle() const{ return triangleV; }

public:
	GLfloat glwidth;
	GLfloat glheight;
private:
	MapMgr(){}
	const MapMgr operator=(const MapMgr&);


	vector<MyPolygon> polygonV;  //所有多边形障碍物
	vector<MyTriangle> triangleV;  //Delaunay生成的三角形网格
};

#endif _scene_map_mgr_h_