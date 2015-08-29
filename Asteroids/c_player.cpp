#include "c_player.h"
#include "c_game.h"
#include "c_bullet.h"

c_Player::c_Player(sf::RenderWindow& renderer) : c_GameWorldObject(renderer, 0.05f, 7.0f, 1.5f, 0.0f), _lives(3), _maxBullets(10), _invicibility(false){

	sf::ConvexShape shape;
	shape.setPointCount(4);
	shape.setPoint(0, sf::Vector2f(25, 25));
	shape.setPoint(1, sf::Vector2f(0, 75));
	shape.setPoint(2, sf::Vector2f(25, 50));
	shape.setPoint(3, sf::Vector2f(50, 75));
	shape.setOutlineColor(sf::Color::White);
	shape.setOutlineThickness(1.0f);
	shape.setFillColor(sf::Color::Transparent);
	_shape = shape;
	_x = _renderer->getSize().x / 2;
	_y = _renderer->getSize().y / 2;
	_shape.setOrigin(25, 50);

	_timeBetweenFire = 250.0f;
}

void c_Player::Accelerate()
{
	float xN = std::sinf(2 * 3.14 * (_shape.getRotation() / 360));
	float yN = std::cosf(2 * 3.14 * (_shape.getRotation() / 360));
	_vX += xN * _accFactor;
	_vY += yN * _accFactor;
}

c_Bullet* c_Player::Fire()
{
	if (_active && _fireClock.getElapsedTime().asMilliseconds() >= _timeBetweenFire)
	{
		if (_activeBullets < _maxBullets)
		{
			c_Bullet* bullet = new c_Bullet(*_renderer, *this, sf::Vector2f(_x, _y), GetRotation());
			_activeBullets++;
			_fireClock.restart();
			return bullet;
		}
	}
	return nullptr;
}

sf::Vector2f c_Player::GetPosition()
{
	return _shape.getPosition();
}

void c_Player::Reset()
{
	_x = _renderer->getSize().x / 2;
	_y = _renderer->getSize().y / 2;

	_vX = 0;
	_vY = 0;
	_lives--;
}

void c_Player::AddBullet()
{
	_activeBullets--;
}

int c_Player::GetRemainingLives()
{
	return _lives;
}

void c_Player::Draw()
{
	c_GameWorldObject::Draw();	//Base class Draw

	for (int i = 1; i <= _lives; i++)
	{
		sf::RectangleShape life;
		life.setFillColor(sf::Color::Green);
		life.setSize(sf::Vector2f(10, 10));
		life.setOutlineColor(sf::Color::White);
		life.setPosition(sf::Vector2f(i * 20, 10));
		_renderer->draw(life);
	}
}

void c_Player::ActivateShield()
{
	if (_lastShieldTime < c_Game::GetTickCount())
	{

	}
}
