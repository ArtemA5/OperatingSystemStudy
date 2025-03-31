#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <atomic>

using namespace std;

atomic<int> counter(0); // Atomic counter shared by all threads

// Visit function (increments counter)
void visit(atomic<int>& a) {
	++a;
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