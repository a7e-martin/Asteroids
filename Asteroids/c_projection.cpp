#include "c_projection.h"

c_Projection::c_Projection(float min, float max) : _min(min), _max(max)
{

}

bool c_Projection::IsSeparated(c_Projection otherProj)
{
	return otherProj._max < _min || _max < otherProj._min;
}
