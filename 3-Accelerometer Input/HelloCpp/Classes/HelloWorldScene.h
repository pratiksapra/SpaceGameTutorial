#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
#define MAX_SCROLL_SPEED 15 //Maximum background scroll speed
#define SPRITE_SCALE_FACTOR 0.5f //Scale factor for sprite assets
#define ACC_ADJUST_FACTOR 8.0f //Accelerometer Adjustment

#include "cocos2d.h"

class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();

    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    //Handle Accelerometer Input
    virtual void didAccelerate(cocos2d::CCAcceleration* pAccelerationValue);

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

private:

    //Keep track of game time in seconds
    float _gameTime;

    //Keep track of scrolling
    float _scrollAxis;
    float _scrollSpeed;
    bool _accelerate;

    //Window width/height
    int _winWidth;
    int _winHeight;

    //Origin
    cocos2d::CCPoint _origin;

    //Batch node for loading the spritesheet
    cocos2d::CCSpriteBatchNode* _spriteBatchNode;

    //Background sprites
    cocos2d::CCSprite* _background1;
    cocos2d::CCSprite* _background2;

    //Game Update Method
    void updateGame(float dt);

    //Player Spaceship sprite
    cocos2d::CCSprite* _player;

    //Accelerometer Values
    double _aX;
    double _aY;
    double _aZ;
};

#endif // __HELLOWORLD_SCENE_H__
