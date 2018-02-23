#ifndef _Cell_H_
#define _Cell_H_

#include"stdafx.h"
#include"graphic\MyTriangle.h"

class Cell : public MyTriangle{
public:
	Cell(){};
	
	Cell(Vector2f p1, Vector2f p2, Vector2f p3)
		:MyTriangle(p1, p2, p3),
		isOpen(false),
		parent(nullptr),
		m_ArrivalWall(0),
		sessionId(-1)
	{
		init();
	};

	~Cell(){};

	//vector<int> getLinks(){ return _links; }
	//void setLinks(const vector<int>& value){ _links = value; }

	void init();

	/**
	* ��鲢���õ�ǰ��������cellB�����ӹ�ϵ��������ͬʱ����cellB��������͵����ӣ�
	* @param cellB
	*/
	void checkAndLink(Cell* cellB);

	/**
	* ��¼·������һ���ڵ����ýڵ�ıߣ�������յ㿪ʼѰ·��Ϊ�����ߣ�
	* @param index	·����һ���ڵ������
	*/
	int setAndGetArrivalWall(const int index);

	/**
	* ������ۣ�h��  Compute the A* Heuristic for this cell given a Goal point
	* @param goal
	*/
	void computeHeuristic(const Vector2f& goal);

	/**
	* ����ֱ�����cell�������Σ��Ĺ�ϵ
	* @param motionPath
	* @return ClassifyResult����
	*/
	//ClassifyResult classifyPathToCell(const Line2D& motionPath);

	/**
	* ������������
	* @param sp
	*/
	void drawIndex();

public:
	int sessionId;

	int index;  //�������е�����ֵ
	vector<int> links;   // ������������ӵ������������� -1��ʾ�ı�û������

	int f; //f(n)=g(n)+h(n)--- g(n)��ʾ����㵽����ڵ�n��·������
	int h; //h(n)��ʾ�ӽڵ�n��Ŀ��ڵ�·�����ѵĹ���ֵ������ֵ��
	bool isOpen;
	Cell* parent;
	int m_ArrivalWall; //���ǵ������һ�ߡ�
	vector<Vector2f> m_WallMidpoint; // ÿ���ߵ��е�
	vector<float> m_WallDistance; //��ǽ�е�֮��ľ��루0-1��1-2��2-0��

private:
	/**
	* ��������������������
	* @param pA
	* @param pB
	* @param caller
	* @return ����ṩ����������caller��һ����, ����true
	*/
	bool requestLink(const Vector2f& pA, const Vector2f& pB, const int& index);
	/**
	* ����sideָ���ıߵ����ӵ�caller������
	* @param side
	* @param caller
	*/
	void setLink(const int& side, const Cell* caller);
	/**
	* ȡ��ָ���ߵ����������͵�����
	* @param side
	* @return
	*/
	int getLink(const int& side) const;

private:
};

#endif _Cell_H_