#pragma once
#include"cocos2d.h"
#include"ui/CocosGUI.h"
#include<iostream>
#include<map>
using namespace std;


USING_NS_CC;


class place :public Scene
{
public:
	static Scene* createScene();
	virtual bool init();
	void menuCloseCallback(cocos2d::Ref* pSender);
	CREATE_FUNC(place);
private:
	map<int, Value>trans;
	string trans_content;
	Label* trans_label;

};
