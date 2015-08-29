#include "c_lineSegment.h"

c_LineSegment::c_LineSegment(sf::Vector2i first, sf::Vector2i second) : _first(first), _second(second){

}

sf::IntRect c_LineSegment::GetBoundingBox()
{
	sf::IntRect rect;
	rect.left = _first.x;
	rect.height = _second.y;
	rect.top = _first.y;
	rect.width = _second.x;
	return rect;
}
