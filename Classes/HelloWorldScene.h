#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

struct SwapInfo
{
	// TODO Cocos complains about struct constructor (CREATE_FUNC(HelloWorld))... Make this a class?
	//SwapInfo(const int r, const int c, const int ty, const int tg)
	//{

	//}

	int row;
	int col;
	int type;
	int tag;
};

class HelloWorld : public cocos2d::Layer
{
public:

	~HelloWorld();

    static cocos2d::Scene* createScene();
    virtual bool init();

	// TODO CREATES PROBLEMS WHEN TOUCHBEGAN'S GONE
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);

	//void update(float deltaTime);
	void myUpdate(float deltaTime);
    
    CREATE_FUNC(HelloWorld);

private:

	void setListeners();
	//TODO	MAKE MORE GENERIC:
	//		NOW: It's more than enought for the game over and game pause scenes
	void drawGameMenu(const std::string& text1, const std::string& text2, const std::string& text3, bool isPause = false);

	const int getNewGemId();
	void initGameField();

	void removeFromGameField(const int fieldRow, const int fieldCol);
	void putNewToGameField(const int fieldRow, const int fieldCol);

	bool checkFallCondition();
	bool checkemptySquares();
	bool checkLines();
	bool checkGameOver();

	int checkRowLines(const int row, const int col);
	int checkColLines(const int row, const int col);
	bool haveSameGem(const int gemId, const int row, const int col);
	void swapGems(const SwapInfo& first, const SwapInfo& second, bool isSwapVisual = true);

	bool isClickEnable;
	bool isGameOver;

	float mSpriteBasePosX;
	float mSpriteBasePosY;

	// TODO Should keep them or get as selectedGemImage->getBoundingBox().size.width/height?
	//		I think spending 8 bytes is better than keep calling a function.
	float mSpriteWidth;
	float mSpriteHeight;

	SwapInfo mSelectedGem;

	int** mGameField;
	int** mSpriteTags;

	std::queue<int> mTagQueue;
};

#endif // __HELLOWORLD_SCENE_H__
