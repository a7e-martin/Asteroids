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
	_timeBetweenShield = 10000.0f;
	_shieldTime = 2500.0f;
	_lastShieldTime = -10000.0f;
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
	_invicibility = true;
	_lastShieldTime = c_Game::GetTickCount();
// 	ActivateShield();
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
	c_GameWorldObject::Draw();

	sf::Font font;
	font.loadFromFile("arial.ttf");
	sf::Text livesText;
	livesText.setFont(font);
	livesText.setCharacterSize(30);
	livesText.setString("Lives");
	livesText.setPosition(sf::Vector2f(10, 10));
	_renderer->draw(livesText);

	for (int i = 1; i <= _lives; i++)
	{
		sf::RectangleShape life;
		life.setFillColor(sf::Color::Green);
		life.setSize(sf::Vector2f(20, 20));
		life.setOutlineColor(sf::Color::White);
		life.setPosition(sf::Vector2f(livesText.getLocalBounds().width + i * 30, 20));
		_renderer->draw(life);
	}
	//Draw the score of the player
	sf::Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(30);
	scoreText.setString("Score " + std::to_string(_score));
	scoreText.setPosition(_renderer->getSize().x - scoreText.getLocalBounds().width - 10, 10);
	_renderer->draw(scoreText);

	//Indicate the status of the shield
	sf::Text shieldStatusText;
	shieldStatusText.setFont(font);
	shieldStatusText.setCharacterSize(30);
	shieldStatusText.setString("Shield");
	shieldStatusText.setPosition(sf::Vector2f(10, _renderer->getSize().y - 40));
	sf::RectangleShape shieldStatus;
	shieldStatus.setSize(sf::Vector2f(20, 20));
	shieldStatus.setOutlineColor(sf::Color::White);
	shieldStatus.setFillColor((c_Game::GetTickCount() - _lastShieldTime > _timeBetweenShield && !_invicibility) ? sf::Color::Green : sf::Color::Red);
	shieldStatus.setPosition(sf::Vector2f(shieldStatusText.getLocalBounds().width + 30, _renderer->getSize().y - 30));
	_renderer->draw(shieldStatusText);
	_renderer->draw(shieldStatus);

	if (_invicibility)
	{
		//We draw a circle to represent the shield
		sf::CircleShape shield(50);
		shield.setOrigin(25, 25);
		shield.setFillColor(sf::Color::Transparent);
		shield.setOutlineThickness(1);
		shield.setOutlineColor(sf::Color::Cyan);
		shield.setPosition(_x - (_shape.getLocalBounds().width / 2), _y - (_shape.getLocalBounds().height / 2));
		_renderer->draw(shield);
	}
}

void c_Player::ActivateShield()
{
	if ((c_Game::GetTickCount() - _lastShieldTime) > _timeBetweenShield)
	{
		_invicibility = true;
		_lastShieldTime = c_Game::GetTickCount();
	}
}

bool c_Player::IsInvicible()
{
	return _invicibility;
}

void c_Player::Update()
{
	c_GameWorldObject::Update();

	_invicibility = (c_Game::GetTickCount() - _lastShieldTime) < _shieldTime;
}

void c_Player::AddPoints(int points)
{
	_score += points;
}
