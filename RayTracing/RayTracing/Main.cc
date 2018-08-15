#include <iostream>
#include <fstream>
#include <string>

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
			float r = float(i) / float(NumberOfX);
			float g = float(j) / float(NumberOfY);
			float b = 0.2;
			int ir = int(255.99*r);
			int ig = int(255.99*g);
			int ib = int(255.99*b);
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}