#include <iostream>
#include <fstream>
#include <string>
#include "vec3.h"

int main()
{
	int NumberOfX = 200;
	int NumberOfY = 100;
	std::ofstream out("out.ppm");
	std::streambuf *coutbuf = std::cout.rdbuf();
	std::cout.rdbuf(out.rdbuf());
	std::cout << "P3\n" << NumberOfX << " " << NumberOfY << "\n255\n";
	for (int j = NumberOfY - 1; j >= 0; j--)
	{
		for (int i = 0; i < NumberOfX; i++)
		{
			vec3 col(float(i) / float(NumberOfX), float(j) / float(NumberOfY), 0.2);
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}