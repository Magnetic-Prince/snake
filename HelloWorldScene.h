#pragma once
#include"cocos2d.h"
#include"ui/CocosGUI.h"
#include<iostream>
using namespace std;


USING_NS_CC;

class HelloWorld :public Scene
{
public:
	static Scene* createScene();
	virtual bool init();
	void menuCloseCallback(cocos2d::Ref* pSender);
	void HelloWorld::menuCloseCallback_end(Ref* pSender);
	CREATE_FUNC(HelloWorld);

};