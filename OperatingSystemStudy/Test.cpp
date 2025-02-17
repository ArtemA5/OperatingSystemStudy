#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>

using namespace std;

void fucnA(vector<int>& A, int b)
{
	A[b] = b * b;
}
/*
int main()
{
	vector<int> A(5, 0);
	vector<thread> threads;

	for (int i = 0; i < 5; i++) {
		A[i] = 1;
	}
}
*/
