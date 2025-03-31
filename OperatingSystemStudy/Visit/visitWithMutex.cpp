#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>

using namespace std;

int counter = 0; // Counter shared by all threads
mutex mtx;

// Visit function (increments counter)
void visit(int& a) {
	mtx.lock();
	a++;
	mtx.unlock();
}

int main() {
	vector<thread> threads;

	auto start = chrono::high_resolution_clock::now(); // Start our stopwatch

	for (int i = 0; i < 10000; i++) {
		threads.emplace_back(visit, ref(counter));
	}

	for (int i = 0; i < threads.size(); i++) {
		threads[i].join();
	}

	auto end = chrono::high_resolution_clock::now(); // End stopwatch
	chrono::duration<double> time = end - start; // Calculate time

	cout << "Visit counter: " << counter << endl;
	cout << "Time: " << time.count() << endl;

	return 0;
}