#include <math.h>
#include <stdlib.h>
#include <iostream>
/*
int main()
{
	int inside_circle = 0;
	int inside_circle_stratified = 0;
	int N = 1000000;
	float sum = 0;
	for (int i = 0; i < N; i++)
	{
		float x = 2 * (float)rand() / RAND_MAX;
		sum += x * x;
	}
	std::cout << "I =" << 2 * sum / N << "\n";
	system("PAUSE");
}*/

int main()
{
	int inside_circle = 0;
	int runs = 0;
	while (true)
	{
		runs++;
		float x = 2 * ((float)rand() / RAND_MAX) - 1;
		float y = 2 * ((float)rand() / RAND_MAX) -1;
		if (x*x + y * y < 1)
		{
			inside_circle++;
		}
		if (runs % 100000 == 0)
		{
			std::cout << "Estimate of pi = " << 4 * float(inside_circle) / runs << "\n";
		}
	}
}