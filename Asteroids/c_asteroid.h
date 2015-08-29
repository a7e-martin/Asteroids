#ifndef DEf_C_ASTEROID
#define DEF_C_ASTEROID
#include "c_gameWorldObject.h"
#include <cstdlib>
#define RAND() ((static_cast<double>(rand()) / static_cast<double>(RAND_MAX))
#define FRAND(min, max) RAND()*(max-min)+min)

class c_Asteroid : public c_GameWorldObject{
public:
	c_Asteroid(sf::RenderWindow& renderer, int size, sf::Vector2i pos, float vX, float vY);
	int GetSize();
	sf::Vector2i GetPos();
	float GetVX();
	float GetVY();
private:
	int _pointCount;
	int _minVary;
	int _maxVary;
	int _size;
};

#endif