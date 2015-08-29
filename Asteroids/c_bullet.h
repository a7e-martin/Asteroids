#ifndef DEF_C_BULLET
#define DEF_C_BULLET
#include "c_gameWorldObject.h"
#include "c_player.h"

class c_Bullet : public c_GameWorldObject{
public:
	c_Bullet(sf::RenderWindow& renderer, c_Player& player, sf::Vector2f position, float direction);
	~c_Bullet();
	void SetPosition(sf::Vector2f newPos);
	bool Remove();
private:
	c_Player *_player;
	float _direction;
	sf::Vector2f _dirVector;
	sf::Clock _lifeClock;
	float _timeToLive;
};

#endif