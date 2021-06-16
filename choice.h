#pragma once
#include"cocos2d.h"
#include"ui/CocosGUI.h"
#include<iostream>
#include<map>
using namespace std;


USING_NS_CC;

class choice :public Scene
{
public:
	static Scene* createScene();
	virtual bool init();
	void menuCloseCallback_easy(cocos2d::Ref* pSender);
	void menuCloseCallback_general(cocos2d::Ref* pSender);
	void menuCloseCallback_hard(cocos2d::Ref* pSender);

	CREATE_FUNC(choice);
private:
	map<int, Value>trans;
	string trans_content;
	Label* trans_label;
};
