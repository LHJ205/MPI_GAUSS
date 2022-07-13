#include "stdio.h" 
#include "stdlib.h" 
#include "math.h" 
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
using namespace std;
int main(void)
{
	int p;
	double spi;
	double mpi;
	int M;
	cin >> M;
	cin >> p;
	cin >> spi;
	cin >> mpi;
	double speedup = spi / mpi;
	double E = speedup / p;
	cout << endl;
	cout << "  矩阵大小：" << M << " * " << M + 1 << "\n  核数：" << p << endl;
	cout << "  加速比：" << speedup << "\n  并行效率：" << E << endl;
	cout << endl;
	cout << endl;
	system("pause");
	return 0;
}