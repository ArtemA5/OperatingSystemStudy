#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

using namespace std;


void visit(int& a) {
	a++;
}

int main() {
	int counter = 0;
	vector<thread> threads;

	for (int i = 0; i < 10000; i++) {
		threads.emplace_back(visit, ref(counter));
	}

	for (int i = 0; i < threads.size(); i++) {
		threads[i].join();
	}

	cout << counter;
}
