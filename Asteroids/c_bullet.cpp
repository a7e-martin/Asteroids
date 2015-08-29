#include "c_bullet.h"

c_Bullet::c_Bullet(sf::RenderWindow& renderer, c_Player& player, sf::Vector2f position, float direction) : c_GameWorldObject(renderer, 0.0f, 0.0f, 12.0f, 0.0f), _timeToLive(4000.0f), _direction(direction), _player(&player){
	_lifeClock.restart();
	_shape.setPointCount(4);
	_shape.setPoint(0, sf::Vector2f(10,10));
	_shape.setPoint(1, sf::Vector2f(9, 9));
	_shape.setPoint(2, sf::Vector2f(10, 8));
	_shape.setPoint(3, sf::Vector2f(11, 9));
	_shape.setOrigin(10, 9);
	_x = position.x;
	_y = position.y;
	_shape.setOutlineColor(sf::Color::White);
	_shape.setOutlineThickness(1.0f);

	//Set the direction and velocity of the bullet

	float xN = std::sinf(2 * 3.14 * (direction / 360));
	float yN = std::cosf(2 * 3.14 * (direction / 360));
	_vX = xN * _accFactor;
	_vY = yN * _accFactor;
}

bool c_Bullet::Remove()
{
	return _lifeClock.getElapsedTime().asMilliseconds() >= _timeToLive;
}

c_Bullet::~c_Bullet()
{
	_player->AddBullet();
}
