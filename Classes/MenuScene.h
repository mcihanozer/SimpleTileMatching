#ifndef MENUSCENE_H
#define MENUSCENE_H

#include "cocos2d.h"

// For taking grid size from the user
// TODO For now it is enough, but it is supposed to evolve a generic window or layer structure
class OptionsScene : public cocos2d::Layer
{
public:
	~OptionsScene();

	static cocos2d::Scene* createScene();
	bool init();

	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);

	CREATE_FUNC(OptionsScene);
};

class MenuScene : public cocos2d::Layer
{
public:
	~MenuScene();

	static cocos2d::Scene* createScene();
	virtual bool init();

	CREATE_FUNC(MenuScene);
};


#endif
