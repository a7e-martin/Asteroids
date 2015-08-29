#ifndef DEF_C_PLAYER
#define DEF_C_PLAYER
#include "c_gameWorldObject.h"

class c_Bullet;
class c_Game;

class c_Player : public c_GameWorldObject{
public:
	c_Player();
	c_Player(sf::RenderWindow& renderer);
	void Accelerate();
	c_Bullet* Fire();
	sf::Vector2f GetPosition();
	void Reset();
	void AddBullet();
	int GetRemainingLives();
	void Draw();
	void ActivateShield();
private:
	sf::Clock _fireClock;
	int _activeBullets;
	int _maxBullets;
	float _timeBetweenFire;
	float _lastShieldTime;
	int _lives;
	bool _invicibility;
};

#endif