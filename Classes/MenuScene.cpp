#include "Common.h"

#include "MenuScene.h"
#include "HelloWorldScene.h"

#include "ui\CocosGUI.h"

USING_NS_CC;
//using namespace cocos2d;
using namespace cocos2d::ui;

OptionsScene::~OptionsScene()
{
	this->removeAllComponents();
}

Scene* OptionsScene::createScene()
{
	auto scene = Scene::create();
	auto layer = OptionsScene::create();
	scene->addChild(layer);
	return scene;
}

bool OptionsScene::init()
{
	// Add a slider, back button, and three labels related with grid size (min, current, max)
	if (!Layer::init())
	{
		return false;
	}

	auto slider = ui::Slider::create("sliderTrack.png", "sliderThumb.png");
	slider->setPosition(Vec2((Director::getInstance()->getWinSize().width * 0.5),
							(Director::getInstance()->getWinSize().height - Director::getInstance()->getWinSize().height * 0.4)));
	slider->setPercent(((float)(GFIELD_SIZE - GMIN_GRIDSIZE) / (GMAX_GRIDSIZE - GMIN_GRIDSIZE)) * 100);
	slider->setSwallowTouches(false);
	this->addChild(slider, 1, 1);

	auto minLabel = Label::createWithSystemFont(std::to_string(GMIN_GRIDSIZE), "Arial", 20);
	minLabel->setPosition((slider->getPositionX() - slider->getBoundingBox().size.width * 0.6), slider->getPositionY());
	this->addChild(minLabel, 1);

	auto maxLabel = Label::createWithSystemFont(std::to_string(GMAX_GRIDSIZE), "Arial", 20);
	maxLabel->setPosition((slider->getPositionX() + slider->getBoundingBox().size.width * 0.55), slider->getPositionY());
	this->addChild(maxLabel, 1);

	auto currentLabel = Label::createWithSystemFont(std::to_string(GFIELD_SIZE), "Arial", 20);
	currentLabel->setPosition((slider->getPositionX()), (slider->getPositionY()) + slider->getBoundingBox().size.height + GPIXEL_SPACE * 15);
	this->addChild(currentLabel, 1, 2);

	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(OptionsScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(OptionsScene::onTouchMoved, this);

	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	auto backButton = Button::create("button_normal.png", "button_selected.png", "button_disabled.png");
	backButton->setTitleColor(Color3B(255, 0, 0));
	backButton->setTitleFontName("Arial");
	backButton->setTitleFontSize(20);
	backButton->setTitleText("BACK");
	backButton->setPosition(Vec2((Director::getInstance()->getWinSize().width * 0.7),
							(Director::getInstance()->getWinSize().height - Director::getInstance()->getWinSize().height * 0.9)));

	backButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			Director::getInstance()->replaceScene(MenuScene::createScene());
			break;
		default:
			break;
		}
	});

	this->addChild(backButton);

	return true;
}

// TODO CREATES PROBLEMS WHEN TOUCHBEGAN GONE: Is it Cocos2d-x related?
bool OptionsScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
	return true;
}

void OptionsScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
	// Update slider label and grid size
	auto slider = dynamic_cast<ui::Slider*>(this->getChildByTag(1));

	GFIELD_SIZE = round((GMAX_GRIDSIZE - GMIN_GRIDSIZE) * (slider->getPercent() * 0.01) + GMIN_GRIDSIZE);

	auto label = dynamic_cast<Label*>(this->getChildByTag(2));
	label->setString(std::to_string(GFIELD_SIZE));
}

MenuScene::~MenuScene()
{
	this->removeAllComponents();
}

Scene* MenuScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MenuScene::create();
	scene->addChild(layer);
	return scene;
}

bool MenuScene::init()
{
	// Init game menu
	if (!Layer::init())
	{
		return false;
	}

	auto size = Director::getInstance()->getWinSize();

	auto label = Label::createWithSystemFont("A Simple Tile-matching Game", "Arial", 40);
	label->setPosition((size.width * 0.5), (size.height - size.height * 0.2));
	this->addChild(label);

	auto playButton = Button::create("button_normal.png", "button_selected.png", "button_disabled.png");
	playButton->setTitleColor(Color3B(255, 0, 0));
	playButton->setTitleFontName("Arial");
	playButton->setTitleFontSize(20);
	playButton->setTitleText("PLAY");
	playButton->setPosition(Vec2((size.width * 0.5), (size.height - size.height * 0.4)));

	playButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			Director::getInstance()->replaceScene(HelloWorld::createScene());
			break;
		default:
			break;
		}
	});

	this->addChild(playButton);

	auto optionButton = Button::create("button_normal.png", "button_selected.png", "button_disabled.png");
	optionButton->setTitleColor(Color3B(255, 0, 0));
	optionButton->setTitleFontName("Arial");
	optionButton->setTitleFontSize(20);
	optionButton->setTitleText("OPTIONS");
	optionButton->setPosition(Vec2((size.width * 0.5), (size.height - size.height * 0.5)));

	optionButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			Director::getInstance()->replaceScene(OptionsScene::createScene());
			break;
		default:
			break;
		}
	});

	this->addChild(optionButton);

	return true;
}