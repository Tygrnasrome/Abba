#ifndef CLOTH_H
#define CLOTH_H
#include "shape.h"
#include "math.h"


class Cloth: public Shape
{
public:
	class Point{
	public:
		glm::vec3 current, previous;
		double inv_mass;
		Point(float x, float y, float z);
		void move(glm::vec3 force);

	};

	class Constraint{
	public:
		Point *p1, *p2;
		Constraint(Point *p1, Point *p2);
		double squared_rest_length;
		void satisfy();

	};

	Cloth(int segments);
	void generatePoints();
	void generateStruct();
	void update(float wind);
//	void draw();

	std::vector<std::vector<Point*>> points;
	std::vector<Constraint*> constraints;
	int maxPoints;

};

#endif // CLOTH_H
