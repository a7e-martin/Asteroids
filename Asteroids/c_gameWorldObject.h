#ifndef DEF_C_GAMEWORLDOBJECT
#define DEF_C_GAMEWORLDOBJECT
#include "SFML/Graphics.hpp"
#include "c_projection.h"
#include <iostream>

class c_Game;

class c_GameWorldObject{
protected:
	sf::RenderWindow *_renderer;
	//Object shape (Convex)
	sf::ConvexShape _shape;
	//Object position
	float _x, _y;
	//Current angle
	float _angle;
	//Current velocity
	float _vX, _vY;
	float _dragFactor;
	float _rotFactor;
	float _accFactor;
	float _rotationalSpeed;
	bool _active;
	float _inactiveTime;
public:
	c_GameWorldObject(sf::RenderWindow& renderer, float dragForce, float rotationalForce, float accelerationFactor, float rotationalSpeed);
	virtual ~c_GameWorldObject() { };
	void Update();
	virtual void Draw();
	void Rotate(int direction);
	float GetRotation();
	virtual bool Remove() { return false; };
	sf::ConvexShape& GetShape();
	void MarkForDelete();
	std::vector<sf::Vector2f*> GetNormalsShape();
	c_Projection Project(sf::Vector2f axis);
	float DotProduct(sf::Vector2f a, sf::Vector2f b);
	bool IsColliding(c_GameWorldObject otherObject);
	void MakeInactive(float time);
	virtual void Reset() { };
};

#endif