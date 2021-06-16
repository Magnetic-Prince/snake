#include"menudemo.h"
#include"ui/CocosGUI.h"
#include<string>
#include "place.h"
#include "choice.h"
#include"cocos2d.h"

using namespace std;

USING_NS_CC;



Scene* Menu1::createScene()
{
	return Menu1::create();
}

bool Menu1::init()
{
	//super init first
	if (!Scene::init())
	{
		return false;
	}
	
	//ȷ�������С�ͻ�����
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Vec2 base = Vec2(visibleSize / 2);

	//�˵�����
	auto sprite = Sprite::create("menu.jpg");
	sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	sprite->setPosition(base);
	this->addChild(sprite);


	/**************************/
	//��������
	/**************************/
	auto spriteOn = Sprite::create("sound.jpg");
	auto spriteOff = Sprite::create("nosound.jpg");
	auto menu1 = MenuItemSprite::create(spriteOn, spriteOn);
	auto menu2 = MenuItemSprite::create(spriteOff, spriteOff);

	auto toggleMENU = MenuItemToggle::createWithCallback([&](Ref* ref)
		//��������Ƶѧϰ���ģ�����û�����ն��������Ƶķ�����QQȺ�﷢���鱾�ϵķ����������á�
		{
			auto item = dynamic_cast<MenuItemToggle*>(ref);
			if (item)
			{
			
				if (item->getSelectedIndex() == 0)
				{
					CCLOG("silence");//��ӡ�ı�����ʾ�÷������Կ��Ƶ�������
				}
				else if (item->getSelectedIndex() == 1)
				{
					CCLOG("play sound");
				}
			}

		}, menu1, menu2, NULL);
	toggleMENU->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	toggleMENU->setPosition(Vec2(visibleSize / 2) + Vec2(-50,100));

	auto configData = FileUtils::getInstance()->getValueMapFromFile("res/tips.plist");
	for (auto abs : configData)
	{
		int key = atoi(abs.first.c_str());
		Value val = abs.second;
		CCLOG("%d,%s", key, val);
		trans.insert(pair<int, Value>(key, val));
	}

	trans_content = trans.at(1001).asString();
	trans_label = Label::createWithSystemFont(trans_content, "����Ҧ��", 50);
	trans_label->enableBold();
	auto menulabel = MenuItemLabel::create(trans_label, NULL);
	menulabel->setColor(Color3B(10, 20, 250));//������
	menulabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	menulabel->setPosition(Vec2(visibleSize / 2)+ Vec2(-50, 100));

	Menu* menu = Menu::create(toggleMENU, menulabel,NULL);
	menu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu,1);


	/**************************/
	//���а�
	/**************************/
	auto placeItem = MenuItemImage::create(
		"place.jpg",
		"placebg.jpg",
		CC_CALLBACK_1(Menu1::menuCloseCallback_place, this));
	placeItem->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	placeItem->setPosition(Vec2(visibleSize / 2) + Vec2(-50, 30));

	trans_content = trans.at(1002).asString();
	trans_label = Label::createWithSystemFont(trans_content, "����Ҧ��", 50);
	trans_label->enableBold();
	auto menulabel_place = MenuItemLabel::create(trans_label, NULL);
	menulabel_place->setColor(Color3B(10, 20, 250));//������
	menulabel_place->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	menulabel_place->setPosition(Vec2(visibleSize / 2) + Vec2(-50, 30));

	Menu* menu_place = Menu::create(placeItem, menulabel_place, NULL);
	menu_place->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	menu_place->setPosition(Vec2::ZERO);
	this->addChild(menu_place, 1);

	/**************************/
	//��Ϸ�Ѷ�
	/**************************/
	auto choiceItem = MenuItemImage::create(
		"choice.jpg",
		"choicebg.jpg",
		CC_CALLBACK_1(Menu1::menuCloseCallback_choice, this));
	choiceItem->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	choiceItem->setPosition(Vec2(visibleSize / 2) + Vec2(-50, -40));

	trans_content = trans.at(1003).asString();
	trans_label = Label::createWithSystemFont(trans_content, "����Ҧ��", 50);
	trans_label->enableBold();
	auto menulabel_choice = MenuItemLabel::create(trans_label, NULL);
	menulabel_choice->setColor(Color3B(10, 20, 250));//������
	menulabel_choice->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	menulabel_choice->setPosition(Vec2(visibleSize / 2) + Vec2(-50, -40));

	Menu* menu_choice = Menu::create(choiceItem, menulabel_choice, NULL);
	menu_choice->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	menu_choice->setPosition(Vec2::ZERO);
	this->addChild(menu_choice, 1);

	return true;
}

void Menu1::menuCloseCallback_place(Ref* pSender)
{
	Director::getInstance()->replaceScene(TransitionCrossFade::create(0.7f, place::createScene()));
}

void Menu1::menuCloseCallback_choice(Ref* pSender)
{
	Director::getInstance()->replaceScene(TransitionCrossFade::create(0.7f, choice::createScene()));
}