#ifndef DEF_C_PROJECTION
#define DEF_C_PROJECTION

class c_Projection{
public:
	float _min;
	float _max;
	c_Projection(float min, float max);
	c_Projection();
	bool IsSeparated(c_Projection otherProj);
};

#endif