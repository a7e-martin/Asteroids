#include "c_gameWorldObject.h"
#include "c_game.h"

c_GameWorldObject::c_GameWorldObject(sf::RenderWindow& renderer, float dragForce, float rotationalForce, float accelerationFactor, float rotationalSpeed) : _renderer(&renderer), _dragFactor(dragForce), _rotFactor(rotationalForce), _accFactor(accelerationFactor), _rotationalSpeed(rotationalSpeed), _active(true)
{

}

void c_GameWorldObject::Draw(){
	if (_active)
	{
		_renderer->draw(_shape);
	}
}

void c_GameWorldObject::Rotate(int direction){
	_shape.rotate(direction * _rotFactor);
}

void c_GameWorldObject::Update()
{
	if (!_active)
	{
		if (c_Game::GetTickCount() < _inactiveTime)
			return;
		_active = true;
		Reset();
	}
	if (_x < 0) _x += _renderer->getSize().x;
	if (_x >= _renderer->getSize().x) _x -= _renderer->getSize().x;
	if (_y < 0) _y += _renderer->getSize().y;
	if (_y >= _renderer->getSize().y) _y -= _renderer->getSize().y;

	_x += _vX;
	_y += -_vY;

	_shape.rotate(_rotationalSpeed);
	_vX = _vX - _vX * _dragFactor;
	_vY = _vY - _vY * _dragFactor;
	_shape.setPosition(_x, _y);

}

float c_GameWorldObject::GetRotation()
{
	return _shape.getRotation();
}

sf::ConvexShape& c_GameWorldObject::GetShape()
{
	return _shape;
}


std::vector<sf::Vector2f*> c_GameWorldObject::GetNormalsShape()
{
	std::vector<sf::Vector2f*> result;
	for (int i = 0; i < _shape.getPointCount(); i++)
	{
		sf::Vector2f p1(_shape.getTransform().transformPoint(_shape.getPoint(i)));
		sf::Vector2f p2(_shape.getTransform().transformPoint(_shape.getPoint(i + 1 == _shape.getPointCount() ? 0 : i + 1)));
		float x = p2.x - p1.x;
		float y = p2.y - p1.y;
		sf::Vector2f * edge = new sf::Vector2f(y, -x); //Left normal (SFML starts top left 0,0)
		result.push_back(edge);
	}
	return result;
}

c_Projection c_GameWorldObject::Project(sf::Vector2f axis)
{
	float min = DotProduct(_shape.getTransform().transformPoint(_shape.getPoint(0)), axis);
	float max = min;

	for (int i = 1; i < _shape.getPointCount(); i++)
	{
		float p = DotProduct(_shape.getTransform().transformPoint(_shape.getPoint(i)), axis);
		if (p < min)
		{
			min = p;
		}
		else if (p > max)
		{
			max = p;
		}
	}
	c_Projection proj(min, max);
	return proj;
}

float c_GameWorldObject::DotProduct(sf::Vector2f a, sf::Vector2f b)
{
	return (a.x * b.x) + (a.y * b.y);
}

bool c_GameWorldObject::IsColliding(c_GameWorldObject otherObject)
{
	std::vector<sf::Vector2f*> norms1;
	std::vector<sf::Vector2f*> norms2;

	norms1 = GetNormalsShape();
	norms2 = otherObject.GetNormalsShape();

	for (int i = 0; i < norms1.size(); i++)
	{
		c_Projection p1 = Project(*norms1[i]);
		c_Projection p2 = otherObject.Project(*norms1[i]);
		if (p1.IsSeparated(p2))
		{
			return false;
		}
	}

	for (int i = 0; i < norms2.size(); i++)
	{
		c_Projection p1 = Project(*norms2[i]);
		c_Projection p2 = otherObject.Project(*norms2[i]);
		if (p1.IsSeparated(p2))
		{
			return false;
		}
	}
	return true;
}

void c_GameWorldObject::MakeInactive(float time)
{
	_inactiveTime = c_Game::GetTickCount() + time;
	_active = false;
}