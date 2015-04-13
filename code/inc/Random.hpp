#ifndef RANDOM_H
#define RANDOM_H

#include <vector>
#include <glm/glm.hpp>
#include <Polygon2D.hpp>
	
class Random {
	private:	
		int graine;
		int init;
		static const unsigned long rand_max = 32767;
	public:
		Random();
		Random(int seed);
		void reset();
		int rand2();
		int RandomInt(int a, int b);
		double RandomNumber(double a, double b);
		int Simulation(double *discreteLaw);
		int Simulation(std::vector<float> law);
		glm::vec2 RandomPoint(Polygon2D poly);
};

extern Random GRandom;

#endif
