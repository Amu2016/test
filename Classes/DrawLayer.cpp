#include"DrawLayer.h"
#include"game\scene\control\scene_map_mgr.h"
#include"game\scene\control\scene_object_mgr.h"

DrawLayer * DrawLayer::create(const Color4B& color, GLfloat width, GLfloat height)
{
	DrawLayer * layer = new (std::nothrow) DrawLayer();
	if (layer && layer->initWithColor(color, width, height))
	{
		layer->autorelease();
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return nullptr;
}

bool DrawLayer::initWithColor(const Color4B& color, GLfloat width, GLfloat height)
{
	CCLOG("------------------DrawLayer init-------------------------");
	glwidth = width;
	glheight = height;

	MapMgr::getInstance()->glwidth = width;
	MapMgr::getInstance()->glheight = height;

	vector<Vector2f> vec;
	vec.push_back(Vector2f(0, 0));
	vec.push_back(Vector2f(glwidth, 0));
	vec.push_back(Vector2f(glwidth, glheight));
	vec.push_back(Vector2f(0, glheight));

	MyPolygon polygon(4, vec);
	polygonV.push_back(polygon);

	if (!LayerColor::initWithColor(color, width, height))
	{
		return false;
	}

	//SceneMonster* monster = SceneMonster::create();
	//monster->setPosition(glwidth / 2, glheight / 2);
	//this->addChild(monster);


	for (int i = 0; i < 20; i++){
		SceneObjectMgr::getInstance()->CreateObject(GameSceneObjType::MONSTER, i);
	}

	listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch* touch, Event* event){
		auto target = static_cast<DrawLayer*>(event->getCurrentTarget());
		return true;
	};
	listener->onTouchMoved = [=](Touch* touch, Event* event){
		auto target = static_cast<DrawLayer*>(event->getCurrentTarget());
	};
	//listener->onTouchEnded = touchCallBack;
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	changeTouchType(TouchType::DELAUNAY);

	//GraphRender::getInstance()->setCanvas(this);

	Line2D line = Line2D(Vector2f(20, 20), Vector2f(700, 500));
	Line2D line2 = Line2D(Vector2f(258, 138), Vector2f(694, 335));

	Vector2f vec2;

	line.intersection(line2, vec2);
	auto lable1 = Label::create("Delaunay", "fonts/arial.ttf", 20);
	lable1->setTextColor(Color4B::GREEN);
	auto menuLabel1 = MenuItemLabel::create(lable1, [=](Ref* ref){
		CCLOG("DELAUNAY!!!!!!!!!!!!!!!!!!");
		if (triangleV.size() > 0)
			return;
		unionpolygonV = polygonV;
		this->unionAll();
		Delaunay delaunay;
		triangleV = delaunay.createDelaunay(unionpolygonV);
		for (auto& trg : triangleV){
			trg.line.push_back(GraphRender::getInstance()->drawLine(this, trg._pointA, trg._pointB, Color4F::GRAY));
			trg.line.push_back(GraphRender::getInstance()->drawLine(this, trg._pointB, trg._pointC, Color4F::GRAY));
			trg.line.push_back(GraphRender::getInstance()->drawLine(this, trg._pointC, trg._pointA, Color4F::GRAY));

			CCLOG("PtA %f, %f", trg._pointA.x, trg._pointA.y);
			CCLOG("PtB %f, %f", trg._pointB.x, trg._pointB.y);
			CCLOG("PtC %f, %f", trg._pointC.x, trg._pointC.y);
		}

		MapMgr::getInstance()->setPolygon(unionpolygonV);
		MapMgr::getInstance()->setTriangleV(triangleV);

		//构建寻路数据
		MyTriangle trg;
		Cell* cell;
		for (unsigned int i = 0; i < triangleV.size(); i++){
			auto a = triangleV[i].getVertex(0);
			auto b = triangleV[i].getVertex(1);
			auto c = triangleV[i].getVertex(2);
			cell = new Cell(triangleV[i].getVertex(0), triangleV[i].getVertex(1), triangleV[i].getVertex(2));
			cell->index = i;
			cellV.push_back(cell);
			//triangleV[i].centerPos.push_back(setText(StringUtils::format("%d", i + 1),
			//	cell->_center, Color3B::RED));
		}
		linkCells(cellV);
		changeTouchType(TouchType::FINDPATH);
	});

	menuLabel1->setPositionY(-menuLabel1->getContentSize().height);

	auto lable2 = Label::create("Clear", "fonts/arial.ttf", 20);
	lable2->setTextColor(Color4B::GREEN);
	auto menuLabel2 = MenuItemLabel::create(lable2, [=](Ref* ref){
		CCLOG("Clear!!!!!!!!!!!!!!!!!!");
		changeTouchType(TouchType::DELAUNAY);
		this->clear();
	});
	menuLabel2->setPositionY(-menuLabel1->getContentSize().height * 2);

	auto lable3 = Label::create("FindPath", "fonts/arial.ttf", 20);
	lable3->setTextColor(Color4B::GREEN);
	auto menuLabel3 = MenuItemLabel::create(lable3, [=](Ref* ref){
		CCLOG("FindPath!!!!!!!!!!!!!!!!!!");

	});
	menuLabel3->setPositionY(-menuLabel1->getContentSize().height * 3);


	auto menu = Menu::create(menuLabel1, menuLabel2, menuLabel3, NULL);
	menu->setPosition(Vec2(width + menuLabel1->getContentSize().width / 2, height - menuLabel1->getContentSize().height / 2));
	this->addChild(menu, 20);


	return true;
}

void DrawLayer::drawPoint(const Vector2f& v){
	auto point = v;
	//CCLOG("%d" , drawPath.size());
	if (drawPath.empty()){
		drawPath.push_back(point);
		previousP = point;
		polyPointPos.push_back(setText(StringUtils::format("(%d,%d)", (int)(point.x), (int)(point.y)), point, Color3B::BLACK));
	}
	else{
		//CCLOG("%d", point.distanceSquared(drawPath[0]));
		if (point.distanceSquared(drawPath[0]) < 200 && drawPath.size() >= 3){
			point = drawPath[0];
			polyLine.push_back(GraphRender::getInstance()->drawLine(this, previousP, point, Color4F(0, 1, 1, 0.2f)));

			MyPolygon polygon(drawPath.size(), drawPath);
			polygon.cw();
			polygon.polyLine = polyLine;
			polyLine.clear();
			polyLine = vector<DrawNode*>();
			polygon.polyPointPos = polyPointPos;
			polyPointPos.clear();
			polyPointPos = vector<Text*>();
			polygon.polyFill = GraphRender::getInstance()->fillPolygon(this, polygon, Color4F(0, 1, 0, 0.2f));
			polygonV.push_back(polygon);

			//for (auto pt : polygon.vertexV)
			//	CCLOG("%f,%f", pt.x, pt.y);

			drawPath = vector<Vector2f>();
		}
		else{
			drawPath.push_back(point);
			polyPointPos.push_back(setText(StringUtils::format("(%d,%d)", (int)(point.x), (int)(point.y)), point, Color3B::BLACK));
			polyLine.push_back(GraphRender::getInstance()->drawLine(this, previousP, point, Color4F(0, 1, 1, 0.2f)));
			previousP = point;
		}
		//CCLOG("(%f,%f) to (%f,%f)", previousP.x, previousP.y, point.x, point.y);
	}
}

Text* DrawLayer::setText(const std::string &textContent, const Vector2f& pos, const Color3B& color){
	auto text = Text::create(textContent,
		"fonts/arial.ttf", 10);

	text->setColor(color);
	text->setPosition(Vec2(pos.x, pos.y));
	this->addChild(text, 20);
	return text;
}

void DrawLayer::unionAll(){
	for (unsigned int i = 1; i < unionpolygonV.size(); i++){
		auto p0 = &unionpolygonV[i];
		for (unsigned int j = 1; j < unionpolygonV.size(); j++){
			auto p1 = &unionpolygonV[j];
			if (p0 != p1 && !p0->isCW() && !p1->isCW()){
				vector<MyPolygon>* v = p0->combination(*p1);//合并

				if (v != nullptr && v->size() > 0){
					if (i < j)
						swap(i, j);
					unionpolygonV.erase(unionpolygonV.begin() + i);
					unionpolygonV.erase(unionpolygonV.begin() + j);

					for (auto pv : *v)
						unionpolygonV.push_back(pv);

					delete v;

					i = 0; // 重新开始
					break;
				}
			}
		}
	}
}

void DrawLayer::linkCells(const vector<Cell*>& pv){
	for (auto& pCellA : pv){
		for (auto& pCellB : pv){
			if (pCellA != pCellB)
				pCellA->checkAndLink(pCellB);
		}
	}
}

void DrawLayer::changeTouchType(const TouchType& type){
	switch (type){
	case DrawLayer::TouchType::DELAUNAY:
		listener->onTouchEnded = [=](Touch* touch, Event* event){
			auto target = static_cast<DrawLayer*>(event->getCurrentTarget());
			CCLOG("touch DELAUNAY!!!!!!!!!!!!!!!!!!");
			if (target == this){
				auto x = touch->getLocation().x - this->getPosition().x;
				auto y = touch->getLocation().y - this->getPosition().y;
				//CCLOG("pos    %f   %f", x, y);
				if (x < 0 || x > glwidth || y < 0 || y > glheight)
					return;
				this->drawPoint(Vector2f(x, y));
			}
		};
		break;
	case DrawLayer::TouchType::FINDPATH:
		listener->onTouchEnded = [=](Touch* touch, Event* event){
			auto target = static_cast<DrawLayer*>(event->getCurrentTarget());
			CCLOG("touch FINDPATH!!!!!!!!!!!!!!!!!!");
			if (target == this){
				auto x = touch->getLocation().x - this->getPosition().x;
				auto y = touch->getLocation().y - this->getPosition().y;
				//CCLOG("pos    %f   %f", x, y);
				if (x < 0 || x > glwidth || y < 0 || y > glheight)
					return;
				if (startPtSign){
					startPtSign = false;
					endPot = Vec2(x, y);

					GraphRender::getInstance()->drawPoint(this, endPot, 8, Color4F::RED);

					FindPath find(cellV);
					auto path = find.find(startPot, endPot);

					for (unsigned int i = 1; i < path.size(); i++){
						GraphRender::getInstance()->drawLine(this, Vector2f(path[i - 1].x, path[i - 1].y),
							Vector2f(path[i].x, path[i].y), Color4F::MAGENTA);
					}
				}
				else{
					startPtSign = true;
					startPot = Vec2(x, y);
					GraphRender::getInstance()->drawPoint(this, startPot, 8, Color4F::BLUE);
				}
			}
		};
		break;
	default:
		//_eventDispatcher->removeEventListener(listener);
		break;
	}
}

void DrawLayer::clear(){
	for (unsigned int i = 1; i < polygonV.size(); i++)
		polygonV[i].clear();
	polygonV.clear();
	polygonV = vector<MyPolygon>();
	unionpolygonV.clear();
	unionpolygonV = vector<MyPolygon>();

	for (auto& trig : triangleV)
		trig.clear();
	triangleV.clear();
	triangleV = vector<MyTriangle>();

	vector<Vector2f> vec;
	vec.push_back(Vector2f(0, 0));
	vec.push_back(Vector2f(glwidth, 0));
	vec.push_back(Vector2f(glwidth, glheight));
	vec.push_back(Vector2f(0, glheight));

	MyPolygon polygon(4, vec);
	polygonV.push_back(polygon);
}

void DrawLayer::clearCellV(){
	vector<Cell*>::iterator iter = cellV.begin();
	while (iter != cellV.end()){
		delete *iter;
		iter++;
	}
	cellV.clear();
}