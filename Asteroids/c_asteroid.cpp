#include "c_asteroid.h"

c_Asteroid::c_Asteroid(sf::RenderWindow& renderer, int size, sf::Vector2i pos) : c_GameWorldObject(renderer, 0.0f, 0.5f, 1.5f, 0.0f), _size(size), _minVary(-20), _maxVary(20){

	//Create a cricle which will give us the coordinates to create the shape
	sf::CircleShape circle(60 / size, 20 / size);

	_shape.setPointCount(circle.getPointCount());

	for (int i = 0; i < circle.getPointCount(); i++)
	{
		_shape.setPoint(i, sf::Vector2f(circle.getPoint(i).x, circle.getPoint(i).y));
	}

	_shape.setOutlineThickness(1.0f);
	_shape.setFillColor(sf::Color::Transparent);
	_shape.setOutlineColor(sf::Color::Yellow);

	//Apply the deformations
	std::srand((int)this);
	for (int i = 0; i < _shape.getPointCount(); i++)
	{
		int vary = std::rand() % _maxVary - _minVary;
		float x = _shape.getPoint(i).x;
		float y = _shape.getPoint(i).y;
		_shape.setPoint(i, sf::Vector2f(x + vary, y + vary));
	}

	float xCenter = _shape.getPoint(0).x;
	float yCenter = _shape.getPoint(_shape.getPointCount() / 2).y / 2;
	//The origin is the point of the shape used to set the pos, rotate the shape, etc.
	_shape.setOrigin(xCenter, yCenter);

 	_rotationalSpeed = (std::rand() % 2) + 1;
	//Set the direction and velocity of the 'roid

	float xN = (FRAND(0, 2) - 1.0f) * _size * 2;
	float yN = (FRAND(0, 2) - 1.0f) * _size * 2;

	_x = pos.x;
	_y = pos.y;
	_vX = xN;
	_vY = yN;
}

int c_Asteroid::GetSize()
{
	return _size;
}

sf::Vector2i c_Asteroid::GetPos()
{
	return sf::Vector2i(_x, _y);
}

float c_Asteroid::GetVY()
{
	return _vY;
}

float c_Asteroid::GetVX()
{
	return _vX;
}