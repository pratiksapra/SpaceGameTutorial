#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
#define MAX_SCROLL_SPEED 15 //Maximum background scroll speed
#define SPRITE_SCALE_FACTOR 0.5f //Scale factor for sprite assets
#define ACC_ADJUST_FACTOR 8.0f //Accelerometer Adjustment
#define MAX_PLAYER_PROJECTILES 10 //Max Number of Player Lasers
#define NUM_ASTEROIDS 30 //Max Number of Asteroids
#define ASTEROID_SPAWN_INTERVAL 0.2f //Interval delay for creating new Asteroids
#define ASTEROID_SPAWN_START 3.0f //Game time when we start spawning the Asteroids
#define ASTEROID_SPAWN_END 20.0f //Game time when we stop spawning the Asteroids
#define MAX_PLAYER_HEALTH 10.0f //Max health of the Player spaceship

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

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

    //Keep track of current Player projectile
    int _playerProjectileCount;

    //Player Laser sprites
    cocos2d::CCSprite* _playerProjectiles[MAX_PLAYER_PROJECTILES];

    //Handle touch events
    void ccTouchesBegan(cocos2d::CCSet *touches, cocos2d::CCEvent *event);

    //Standard sprite action finished Handler
    void spriteMoveFinished(CCNode *sender);

    //Keep track of current Asteroid
    int _asteroidCount;

    //Time since last Asteroid spawn
    float _asteroidSpawnTime;
    //Asteroid sprites
    cocos2d::CCSprite* _asteroids[NUM_ASTEROIDS];

    //Return a random number between min and max
    float randomRange(float min, float max);

    //Keep track of Player spaceship health
    float _playerHealth;

    //Create a particle effect with a plist file and position
    void createParticleEffect(const char* filename, float x, float y);

    //Create a particle effect using a plist file, position, start color, duration and end size
    void createParticleEffect(const char* filename, float x, float y, cocos2d::ccColor4F startColor, float duration, float endSize);
};

#endif // __HELLOWORLD_SCENE_H__
