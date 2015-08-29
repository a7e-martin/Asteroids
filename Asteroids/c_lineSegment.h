#ifndef DEF_C_LINESEGMENT
#define DEF_C_LINESEGMENT
#include "SFML/Graphics.hpp"

class c_LineSegment{
private:
public:
	sf::Vector2i _first;
	sf::Vector2i _second;
	c_LineSegment(sf::Vector2i first, sf::Vector2i second);
	c_LineSegment();
	sf::IntRect GetBoundingBox();

};
#endif