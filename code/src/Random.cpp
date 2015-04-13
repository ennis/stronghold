#include <Random.hpp>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cmath>


Random GRandom;

Random::Random() {
	graine = time(NULL);
	init = graine;
}

Random::Random(int seed) {
	init = seed;
	graine = seed;
}

void Random::reset()
{
	graine = init;
}

int Random::rand2() {	
		graine = graine * 1103515245 + 12345;
		return (unsigned int)(graine / 65536) % 32768;
}	

int Random::RandomInt(int a, int b) {
	return(floor(rand2() % (b - a) + a));
}

double Random::RandomNumber(double a, double b) {
	return(((double)rand2() / (double)rand_max)*(b - a) + a);
}

int Random::Simulation(double *discreteLaw) {
	if (discreteLaw == NULL) {
		return -1;
	}
	double x = (double)rand2() / (double)rand_max;
	double threshold = 0;
	int i = 0;
	while (x > threshold) {
		if (threshold > 1 || threshold < 0) {
			std::cout << "Loi en entree fausse !" << std::endl;
			throw "ERROR";
	}
		threshold += discreteLaw[i];
		i++;
	}
	return i-1;
}

int Random::Simulation(std::vector<float> law) {
    double *discreteLaw = new double[law.size()];

    for (int i = 0; i < law.size(); i++) {
        discreteLaw[i] = law[i];
    }

    return Simulation(discreteLaw);
}

glm::vec2 Random::RandomPoint(Polygon2D poly) {
	glm::vec2 res = glm::vec2(0,0);
	double weight;
	double sum = 0;
	std::vector<glm::vec2> points = poly.getPoints();
	for (int i = 0; i < points.size(); i++) {
		weight = RandomNumber(0, 1);
		res.x += weight*points[i].x;
		res.y += weight*points[i].y;
		sum += weight;
	}
	res.x /= sum;
	res.y /= sum;
	return res;
}
