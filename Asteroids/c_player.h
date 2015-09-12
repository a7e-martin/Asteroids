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
	bool IsInvicible();
	void Update();
	void AddPoints(int points);
private:
	sf::Clock _fireClock;
	sf::Font _uiFont;
	int _activeBullets;
	int _maxBullets;
	float _timeBetweenFire;
	float _lastShieldTime;
	float _timeBetweenShield;
	int _lives;
	bool _invicibility;
	float _shieldTime;
	int _score;
};

#endif