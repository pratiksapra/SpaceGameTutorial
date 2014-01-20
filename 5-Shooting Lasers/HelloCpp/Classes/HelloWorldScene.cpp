#include "HelloWorldScene.h"

USING_NS_CC;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
//    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
//                                        "CloseNormal.png",
//                                        "CloseSelected.png",
//                                        this,
//                                        menu_selector(HelloWorld::menuCloseCallback));
//
//	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
//                                origin.y + pCloseItem->getContentSize().height/2));
//
//    // create menu, it's an autorelease object
//    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
//    pMenu->setPosition(CCPointZero);
//    this->addChild(pMenu, 1);
//
//    /////////////////////////////
//    // 3. add your codes below...
//
//    // add a label shows "Hello World"
//    // create and initialize a label
//
//    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
//
//    // position the label on the center of the screen
//    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
//                            origin.y + visibleSize.height - pLabel->getContentSize().height));
//
//    // add the label as a child to this layer
//    this->addChild(pLabel, 1);
//
//    // add "HelloWorld" splash screen"
//    CCSprite* pSprite = CCSprite::create("HelloWorld.png");
//
//    // position the sprite on the center of the screen
//    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
//
//    // add the sprite as a child to this layer
//    this->addChild(pSprite, 0);
	do {
		//Initialize game time
		_gameTime = 0;
		//Initialize scrolling variables
		_scrollSpeed = 0;
		_accelerate = true;
		_scrollAxis = 0;

		//Get window width/height
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		_winHeight = winSize.height;
		_winWidth = winSize.width;

		//Get origin
		_origin = CCDirector::sharedDirector()->getVisibleOrigin();

		//Load spritesheet
		_spriteBatchNode = CCSpriteBatchNode::create("spriteSheet.png");
		this->addChild(_spriteBatchNode);
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("spriteSheet.plist");

		//Load background sprites
		_background1 = CCSprite::createWithSpriteFrameName("background.png");
		_background1->setAnchorPoint(ccp(0.0f, 0.0f));
		_background1->setPosition(ccp(0.0f, 0.0f));

		CCRect bgRect = _background1->getTextureRect();
		_background1->setScaleX(winSize.width / bgRect.size.width);
		_background1->setScaleY(winSize.height / bgRect.size.height);

		_background2 = CCSprite::createWithSpriteFrameName("background.png");
		_background2->setAnchorPoint(ccp(0.0f, 0.0f));
		_background2->setPosition(ccp(0.0f, winSize.height));

		_background2->setScaleX(winSize.width / bgRect.size.width);
		_background2->setScaleY(winSize.height / bgRect.size.height);
		this->addChild(_background2);
		this->addChild(_background1);

		//Load Player Spaceship sprite
		_player = CCSprite::createWithSpriteFrameName("playership.png");
		_player->setScale(SPRITE_SCALE_FACTOR);
		//Position the player sprite at the bottom of the screen
		_player->setPosition(ccp(_winWidth/2,_origin.y + (_player->getScaleY()*_player->getContentSize().height)));
		this->addChild(_player,10); //Highest Z-order since player sprite needs to be always on the top

		this->setAccelerometerEnabled(true);

		//Preload sound effects
		CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("shoot.wav");
		CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("explode.wav");
		CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("explode_ship.wav");
		CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("main_background_music.ogg");
		CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("enemy_background_music.ogg");
		CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("enemy_background_music.ogg");

		//Play background music with looping enabled
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("main_background_music.ogg",true);

		//Initialize Player projectile count
		_playerProjectileCount = 0;

		//Initialize Player projectile sprites
		for (int i = 0; i < MAX_PLAYER_PROJECTILES; ++i) {
		    _playerProjectiles[i] = CCSprite::createWithSpriteFrameName(
		            "playerlaser.png");
		    _playerProjectiles[i]->setScale(SPRITE_SCALE_FACTOR);
		    _playerProjectiles[i]->setVisible(false);
		    _playerProjectiles[i]->setPosition(ccp(_player->getPositionX(),_player->getPositionY()+(_player->getScaleY()*_player->getContentSize().height)));
		    this->addChild(_playerProjectiles[i]);
		}

		//Enable touch events
		this->setTouchEnabled(true);

		//schedule updateGame to update the game at regular intervals
		this->schedule(schedule_selector(HelloWorld::updateGame));
	} while (0);

    return true;
}



void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

void HelloWorld::updateGame(float dt) {

    _gameTime += dt; //Add dt to game time

    //Background Scrolling
    if (_accelerate) { //accelerate
        //Move the scroll axis at scroll speed
        _scrollAxis -= _scrollSpeed;

        //if we have scrolled through both backgrounds reset scroll axis
        if (_scrollAxis <= -_winHeight) {
            _scrollAxis = 0;
        }

        //update positions for the background sprites as per the scrollAxis
        _background1->setPosition(ccp(0.0f,_scrollAxis));
        _background2->setPosition(ccp(0.0f,_scrollAxis+_winHeight));

        //Keep increasing the scroll speed until we approach the max
        if (_scrollSpeed < MAX_SCROLL_SPEED) {
            _scrollSpeed += 0.1;
        }
    } else { //decelerate
        //Move the scroll axis at scroll speed
        _scrollAxis -= _scrollSpeed;

        //if we have scrolled through both backgrounds reset scroll axis
        if (_scrollAxis <= -_winHeight) {
            _scrollAxis = 0;
        }
        //Keep decreasing the scroll speed until we approach reach 0
        if (_scrollSpeed > 0) {
            _scrollSpeed -= 0.1;
            _background1->setPosition(ccp(0.0f,_scrollAxis));
            _background2->setPosition(ccp(0.0f,_scrollAxis+_winHeight));
        }
    }

    //Update player position based on accelerometer values
        CCPoint location;
        location = ccp(_player->getPositionX()+_aX * ACC_ADJUST_FACTOR,_player->getPositionY());

        //Bound the ship location to size of the screen width so we don't go off the screen
        if (location.x> (_origin.x + _player->getContentSize().width/2 * _player->getScaleX()) && location.x < (_winWidth - _player->getScaleX()*_player->getContentSize().width/2)){
            _player->setPosition(location);

        }
}



void HelloWorld::didAccelerate(CCAcceleration *pAccelerationValue) {
    _aX = pAccelerationValue->x;
    _aY = pAccelerationValue->y;
    _aZ = pAccelerationValue->z;

}

void HelloWorld::ccTouchesBegan(cocos2d::CCSet *touches,cocos2d::CCEvent *event) {
	//We can choose one of the touches to work with
	CCTouch* touch = (CCTouch*) (touches->anyObject());
	CCPoint location = touch->getLocation();

	//Pick laser sprite from the projectile array and run an action on it
	_playerProjectiles[_playerProjectileCount]->stopAllActions();
	_playerProjectiles[_playerProjectileCount]->setPosition(ccp(_player->getPositionX(),_player->getPositionY()+(_player->getScaleY()*_player->getContentSize().height)));
	_playerProjectiles[_playerProjectileCount]->setVisible(true);

	//Create a move action that is 0.4s long and moves the projectile starting from the player position to the top of the screen
	_playerProjectiles[_playerProjectileCount]->runAction(CCSequence::create(CCMoveTo::create(0.4f,ccp(_player->getPositionX(),_winHeight)),CCCallFuncN::create(this,callfuncN_selector(HelloWorld::spriteMoveFinished)),NULL));
	++_playerProjectileCount;

	//If reached the maximum number of sprites reset the the count to recycle the sprites
	if (_playerProjectileCount >= MAX_PLAYER_PROJECTILES) {
		_playerProjectileCount = 0;
	}
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("shoot.wav");
}

void HelloWorld::spriteMoveFinished(CCNode* sender) {
    sender->setVisible(false);
}