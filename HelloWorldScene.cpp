#include "HelloWorldScene.h"
#include<string>
#include<iostream>
#include"ui/CocosGUI.h"
#include"menudemo.h"
#include "place.h"

using namespace std;
using namespace ui;
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

bool HelloWorld::init()
{
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    //确定界面大小和基坐标
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 base = Vec2(visibleSize / 2);

    auto sprite = Sprite::create("bg.jpg");
    sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    sprite->setPosition(base);
    this->addChild(sprite);

    auto startItem = MenuItemImage::create(
        "start.jpg",
        "startbg.jpg",
        CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    auto menu = Menu::create(startItem, NULL);
    menu->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    menu->setPosition(base + Vec2(-150, -330));
    this->addChild(menu, 1);


    auto endItem = MenuItemImage::create(
        "back.jpg",
        "backbg.jpg",
        CC_CALLBACK_1(HelloWorld::menuCloseCallback_end, this));

    auto menu_back = Menu::create(endItem, NULL);
    menu_back->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    menu_back->setPosition(base + Vec2(150, -330));
    this->addChild(menu_back, 1);

    return true;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->replaceScene(TransitionCrossFade::create(0.7f, Menu1::createScene()));
}

void HelloWorld::menuCloseCallback_end(Ref* pSender)
{
    Director::getInstance()->end();
}
