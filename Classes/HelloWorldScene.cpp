#include "Common.h"

#include "MenuScene.h"
#include "HelloWorldScene.h"

#include "SimpleAudioEngine.h"
#include "ui\CocosGUI.h"


USING_NS_CC;
//using namespace cocos2d;
using namespace cocos2d::ui;

HelloWorld::~HelloWorld()
{
	for (int ri = 0; ri < GFIELD_SIZE; ri++)
	{
		delete[] mGameField[ri];
		delete[] mSpriteTags[ri];
	}
	delete[] mGameField;
	delete[] mSpriteTags;

	std::queue<int> empty;
	std::swap(mTagQueue, empty);

	this->removeAllComponents();
}

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	setListeners();
	initGameField();

	//this->scheduleUpdate();
	this->schedule(schedule_selector(HelloWorld::myUpdate), 0.15);

	return true;
}

void HelloWorld::setListeners()
{
	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

// TODO CREATES PROBLEMS WHEN TOUCHBEGAN GONE
bool HelloWorld::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
	return true;
}

void HelloWorld::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
	// Get mouse click, convert it into grid indices, make the move if you can
	if (isClickEnable)
	{
		// Mouse click -> field row-column conversation
		int colAbs = (int)(touch->getLocation().x / mSpriteWidth);
		int rowAbs = (int)((Director::getInstance()->getWinSize().height - touch->getLocation().y) / mSpriteHeight);

		// Because we are leaving some space from the top and left
		int row = rowAbs - (GGEM_SPACE_FROMUP - 1);
		int col = colAbs - (GGEM_SPACE_FROMLEFT - 1);

		if (col > -1 && col < GFIELD_SIZE && row > -1 && row < GFIELD_SIZE)
		{
			if(mSelectedGem.row == row && mSelectedGem.col == col)
			{
				mSelectedGem.row = -1;
				mSelectedGem.col = -1;

				this->getChildByTag(-2)->setPosition(0,0);
				this->getChildByTag(-2)->setVisible(false);
			}
			else if (mSelectedGem.row == -1 && mSelectedGem.col == -1)
			{
				mSelectedGem.row = row;
				mSelectedGem.col = col;

				this->getChildByTag(-2)->setPosition(this->getChildByTag(mSpriteTags[row][col])->getPosition());
				this->getChildByTag(-2)->setVisible(true);
			}
			else
			{
				SwapInfo secondGem;
				secondGem.row = row;
				secondGem.col = col;

				if (	((secondGem.row == mSelectedGem.row + 1 || secondGem.row == mSelectedGem.row - 1) && secondGem.col == mSelectedGem.col) ||
						(( secondGem.col == mSelectedGem.col + 1 || secondGem.col == mSelectedGem.col - 1) && secondGem.row == mSelectedGem.row)
					)
				{
					swapGems(mSelectedGem, secondGem);

					if (checkRowLines(mSelectedGem.row, mSelectedGem.col) > 2 || checkColLines(mSelectedGem.row, mSelectedGem.col) > 2 ||
						checkRowLines(secondGem.row, secondGem.col) > 2 || checkColLines(secondGem.row, secondGem.col) > 2)
					{
						isClickEnable = false;
					}
					else
					{
						swapGems(secondGem, mSelectedGem);
					}
				}

				this->getChildByTag(-2)->setPosition(0, 0);
				this->getChildByTag(-2)->setVisible(false);

				mSelectedGem.row = -1;
				mSelectedGem.col = -1;
			}
		}

	}	// End of if isClickedEnable
}

const int HelloWorld::getNewGemId()
{
	return rand() % GGEM_NUMBER;
}

void HelloWorld::putNewToGameField(const int fieldRow, const int fieldCol)
{
	mGameField[fieldRow][fieldCol] = getNewGemId();

	auto newSprite = Sprite::create((GTILE_FILENAME + std::to_string(mGameField[fieldRow][fieldCol]) + GTILE_EXTENSION));

	float spriteY = mSpriteBasePosY - (mSpriteHeight + GPIXEL_SPACE) * fieldRow;
	float spriteX = mSpriteBasePosX + (mSpriteWidth + GPIXEL_SPACE) * fieldCol;
	newSprite->setPosition(spriteX, spriteY);

	newSprite->setName(GTILE_FILENAME + std::to_string(mGameField[fieldRow][fieldCol]) + GTILE_EXTENSION);
	mSpriteTags[fieldRow][fieldCol] = mTagQueue.front();
	newSprite->setTag(mSpriteTags[fieldRow][fieldCol]);
	mTagQueue.pop();

	newSprite->setAnchorPoint(Vec2(1.0, 0.0));

	addChild(newSprite, 1);
}

void HelloWorld::initGameField()
{
	isClickEnable = false;
	isGameOver = false;
	mSelectedGem.col = -1;
	mSelectedGem.row = -1;

	// TODO Pause button: Adding a new layer could be better for the pause visual (not the button but the menu)?
	// TODO Still having a generic layer/window system is way better
	//auto pauseButton = Button::create("pause_normal.png", "pause_selected.png", "pause_disabled.png");
	//pauseButton->setPosition(Vec2((Director::getInstance()->getWinSize().width - Director::getInstance()->getWinSize().width * 0.1),
	//	(Director::getInstance()->getWinSize().height * 0.1)));

	//pauseButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
	//	switch (type)
	//	{
	//	case ui::Widget::TouchEventType::BEGAN:
	//		break;
	//	case ui::Widget::TouchEventType::ENDED:
	//		drawGameMenu("PAUSE", "CONTINUE", "MAIN MENU", true);
	//		break;
	//	default:
	//		break;
	//	}
	//});

	//this->addChild(pauseButton);

	auto selectedGemImage = Sprite::create("jewel_selected.png");
	selectedGemImage->setPosition(0, 0);
	selectedGemImage->setAnchorPoint(Vec2(1, 0));
	selectedGemImage->setTag(-2);
	selectedGemImage->setVisible(false);
	this->addChild(selectedGemImage, 2);

	mSpriteWidth = selectedGemImage->getBoundingBox().size.width;
	mSpriteHeight = selectedGemImage->getBoundingBox().size.height;

	mSpriteBasePosX = GGEM_SPACE_FROMLEFT * mSpriteWidth;
	mSpriteBasePosY = Director::getInstance()->getWinSize().height - GGEM_SPACE_FROMUP * mSpriteHeight;

	for (int tag = 0; tag < GFIELD_SIZE * GFIELD_SIZE; tag++)
	{
		mTagQueue.push(tag);
	}

	mGameField = new int*[GFIELD_SIZE];
	mSpriteTags = new int*[GFIELD_SIZE];
	for (int ri = 0; ri < GFIELD_SIZE; ri++)
	{
		mGameField[ri] = new int[GFIELD_SIZE];
		mSpriteTags[ri] = new int[GFIELD_SIZE];

		for (int gi = 0; gi < GFIELD_SIZE; gi++)
		{
			putNewToGameField(ri, gi);
		}

	}
}

bool HelloWorld::checkFallCondition()
{
	//Checks the gems whether they have an empty space underneath

	// TODO -or not TODO- std::vector<SwapInfo> } Uses extra memory but creates a good "fake physics" for fall down

	// TODO Might be too much dynamic freeing and allocation, consider to right your own smart structure
	// TODO Update the name of "SwapInfo" to "TileInfo"
	std::vector<SwapInfo> fallList;

	for (int row = GFIELD_SIZE - 2; row > -1; row--)	// -2 because of C++
	{
		for (int col = 0; col < GFIELD_SIZE; col++)
		{
			if (mGameField[row][col] != -1 && mGameField[row + 1][col] == -1)
			{
				SwapInfo newTile;
				newTile.row = row;
				newTile.col = col;
				newTile.tag = mSpriteTags[row][col];
				newTile.type = mGameField[row][col];
				fallList.push_back(newTile);
			}
		}

	}

	// Let 'em fall
	for (int ti = 0; ti < fallList.size(); ti++)
	{
		this->getChildByTag(fallList[ti].tag)->setPositionY(mSpriteBasePosY -
															(this->getChildByTag(fallList[ti].tag)->getBoundingBox().size.height + GPIXEL_SPACE) *
															(fallList[ti].row + 1));

		mGameField[fallList[ti].row][fallList[ti].col] = mSpriteTags[fallList[ti].row][fallList[ti].col] = -1;
		mGameField[fallList[ti].row + 1][fallList[ti].col] = fallList[ti].type;
		mSpriteTags[fallList[ti].row + 1][fallList[ti].col] = fallList[ti].tag;
	}
	
	return (fallList.size() > 0);
}

bool HelloWorld::checkemptySquares()
{
	// TODO Might be too much dynamic allocation, consider to right your own smart structure
	// TODO Update the name of "SwapInfo" to "TileInfo"
	std::vector<SwapInfo> emptyList;

	for (int row = GFIELD_SIZE - 1; row > -1; row--)
	{
		for (int col = 0; col < GFIELD_SIZE; col++)
		{
			// If there is an empty spot
			if (mGameField[row][col] == -1)
			{
				SwapInfo newTile;
				newTile.row = row;
				newTile.col = col;
				emptyList.push_back(newTile);
			}
		}
	}

	for (int ti = 0; ti < emptyList.size(); ti++)
	{
		putNewToGameField(emptyList[ti].row, emptyList[ti].col);
	}

	return (emptyList.size() > 0);
}

bool HelloWorld::checkLines()
{
	//Checks for the connections in the grid

	// TODO Make map assignments more efficient if it's possible
	//		([] works with the logic but is the assigment ([] =) slower than .find()?)
	// TODO This version supports L and T junctions but there is
	//		a possible improvement for the line search. Check it later if you can

	std::map<int, SwapInfo> lineList;

	for (int row = GFIELD_SIZE - 1; row > -1; row--)
	{
		for (int col = 0; col < GFIELD_SIZE; col++)
		{
			if (checkRowLines(row, col) > 2 || checkColLines(row, col) > 2)	// TODO Replace 2 with a constant for the future
			{
				// TODO WARNING: Possible code repetation or useless function call: Be sure there is no problem

				if (checkRowLines(row, col) > 2)
				{
					int checkPosition = col;
					while (haveSameGem(mGameField[row][col], row, checkPosition - 1))
					{
						checkPosition--;
						SwapInfo newInfo;
						newInfo.row = row;
						newInfo.col = checkPosition;
						lineList[mSpriteTags[newInfo.row][newInfo.col]] = newInfo;
					}

					checkPosition = col;
					while (haveSameGem(mGameField[row][col], row, checkPosition + 1))
					{
						checkPosition++;
						SwapInfo newInfo;
						newInfo.row = row;
						newInfo.col = checkPosition;
						lineList[mSpriteTags[newInfo.row][newInfo.col]] = newInfo;
					}
				}

				if (checkColLines(row, col) > 2)
				{
					int checkPosition = row;
					while (haveSameGem(mGameField[row][col], checkPosition - 1, col))
					{
						checkPosition--;
						SwapInfo newInfo;
						newInfo.row = checkPosition;
						newInfo.col = col;
						lineList[mSpriteTags[newInfo.row][newInfo.col]] = newInfo;
					}

					checkPosition = row;
					while (haveSameGem(mGameField[row][col], checkPosition + 1, col))
					{
						checkPosition++;
						SwapInfo newInfo;
						newInfo.row = checkPosition;
						newInfo.col = col;
						lineList[mSpriteTags[newInfo.row][newInfo.col]] = newInfo;
					}

				}

				SwapInfo newInfo;
				newInfo.row = row;
				newInfo.col = col;
				lineList[mSpriteTags[newInfo.row][newInfo.col]] = newInfo;
			}
		}
	}

	for (std::map<int, SwapInfo>::iterator it = lineList.begin(); it != lineList.end(); it++)
	{
		removeFromGameField(it->second.row, it->second.col);
	}

	return (lineList.size() > 0);
}

bool HelloWorld::checkGameOver()
{
	for (int row = 0; row < GFIELD_SIZE; row++)
	{
		for (int col = 0; col < GFIELD_SIZE; col++)
		{
			//TODO Maybe you can write these processes as a single method?
			if (row < GFIELD_SIZE - 1)
			{
				SwapInfo first, second;
				first.row = row;
				first.col = col;
				
				second.row = row + 1;
				second.col = col;

				swapGems(first, second, false);

				if (checkRowLines(first.row, first.col) > 2 || checkColLines(first.row, first.col) > 2 ||
					checkRowLines(second.row, second.col) > 2 || checkColLines(second.row, second.col) > 2)
				{
					// TODO You can also give this as a hint
					swapGems(first, second, false);
					return false;
				}

				swapGems(first, second, false);

			}

			if (col < GFIELD_SIZE - 1)
			{
				SwapInfo first, second;
				first.row = row;
				first.col = col;

				second.row = row;
				second.col = col + 1;

				swapGems(first, second, false);

				if (checkRowLines(first.row, first.col) > 2 || checkColLines(first.row, first.col) > 2 ||
					checkRowLines(second.row, second.col) > 2 || checkColLines(second.row, second.col) > 2)
				{
					// TODO You can also give this as a hint
					swapGems(first, second, false);
					return false;
				}

				swapGems(first, second, false);

			}
		}
	}

	return true;
}

void HelloWorld::drawGameMenu(const std::string& text1, const std::string& text2, const std::string& text3, bool isPause)
{
	// TODO Right now only prints game over menu. But, can this be used also for PAUSE MENU? 

	isClickEnable = false;

	auto label = Label::createWithSystemFont(text1, "Arial", 40);
	label->setPosition((Director::getInstance()->getWinSize().width * 0.5),
						(Director::getInstance()->getWinSize().height - Director::getInstance()->getWinSize().height * 0.2));
	this->addChild(label);

	auto playButton = Button::create("button_normal.png", "button_selected.png", "button_disabled.png");
	playButton->setTitleColor(Color3B(255, 0, 0));
	playButton->setTitleFontName("Arial");
	playButton->setTitleFontSize(20);
	playButton->setTitleText(text2);
	playButton->setPosition(Vec2((Director::getInstance()->getWinSize().width * 0.5),
								(Director::getInstance()->getWinSize().height - Director::getInstance()->getWinSize().height * 0.4)));

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
	optionButton->setTitleText("MAIN MENU");
	optionButton->setTitleText(text3);
	optionButton->setPosition(Vec2((Director::getInstance()->getWinSize().width * 0.5),
									(Director::getInstance()->getWinSize().height - Director::getInstance()->getWinSize().height * 0.5)));

	optionButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
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

	this->addChild(optionButton);
}

//void HelloWorld::update(float deltaTime)
void HelloWorld::myUpdate(float deltaTime)
{
	if (!isGameOver)
	{
		if (!checkFallCondition())
		{
			if (!checkemptySquares())
			{
				if (!checkLines())
				{
					if (!checkGameOver())
					{
						isClickEnable = true;
					}
					else
					{
						isGameOver = true;
						drawGameMenu("GAME OVER", "PLAY AGAIN", "MAIN MENU");
					}

				}
			}
		}
	}	// End of if isGameOver

}	// End of update()

// TODO CAN BE ONE FUNCTION with checkColLines()?
int HelloWorld::checkRowLines(const int row, const int col)
{
	int lineLength = 1;
	int nextCol = col;

	// Check left side
	while (haveSameGem(mGameField[row][col], row, nextCol-1))
	{
		nextCol--;
		lineLength++;
	}

	// Check right side
	nextCol = col;
	while (haveSameGem(mGameField[row][col], row, nextCol + 1))
	{
		nextCol++;
		lineLength++;
	}

	return lineLength;
}

// TODO CAN BE ONE FUNCTION with checkRowLines()?
int HelloWorld::checkColLines(const int row, const int col)
{
	int lineLength = 1;

	// Check up
	int nextRow = row;
	while (haveSameGem(mGameField[row][col], nextRow - 1, col))
	{
		nextRow--;
		lineLength++;
	}

	// Check down/below
	nextRow = row;
	while (haveSameGem(mGameField[row][col], nextRow + 1, col))
	{
		nextRow++;
		lineLength++;
	}

	return lineLength;
}

bool HelloWorld::haveSameGem(const int gemId, const int row, const int col)
{
	if (row < 0 || row >= GFIELD_SIZE || col < 0 || col >= GFIELD_SIZE)
	{
		return false;
	}

	return gemId == mGameField[row][col];
}

void HelloWorld::removeFromGameField(const int fieldRow, const int fieldCol)
{
	int tag = mSpriteTags[fieldRow][fieldCol];
	this->removeChildByTag(tag, true);
	mGameField[fieldRow][fieldCol] = -1;
	mSpriteTags[fieldRow][fieldCol] = -1;
	mTagQueue.push(tag);
}

void HelloWorld::swapGems(const SwapInfo& first, const SwapInfo& second, bool isSwapVisual)
{
	if (isSwapVisual)	// It is set from checkGameOver() as FALSE
	{
		Vec2 sourcePos = this->getChildByTag(mSpriteTags[first.row][first.col])->getPosition();
		Vec2 destPos = this->getChildByTag(mSpriteTags[second.row][second.col])->getPosition();

		this->getChildByTag(mSpriteTags[first.row][first.col])->setPosition(destPos);
		this->getChildByTag(mSpriteTags[second.row][second.col])->setPosition(sourcePos);
	}

	int firstType = mGameField[first.row][first.col];
	mGameField[first.row][first.col] = mGameField[second.row][second.col];
	mGameField[second.row][second.col] = firstType;

	int firstTag = mSpriteTags[first.row][first.col];
	mSpriteTags[first.row][first.col] = mSpriteTags[second.row][second.col];
	mSpriteTags[second.row][second.col] = firstTag;
}
