#pragma once
#include"cocos2d.h"
#include"ui/CocosGUI.h"
#include<iostream>
#include<string>
#include<map>

using namespace std;
USING_NS_CC;

class Menu1 :public Scene
{
public:
	static Scene* createScene();
	virtual bool init();
	void menuCloseCallback_place(cocos2d::Ref* pSender);
	void menuCloseCallback_choice(cocos2d::Ref* pSender);
	CREATE_FUNC(Menu1);

private:
	map<int, Value>trans;
	string trans_content;
	Label* trans_label;
};