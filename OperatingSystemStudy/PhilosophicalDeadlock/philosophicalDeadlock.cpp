#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <thread>
#include <chrono>
#include <vector>
#include <random>


using namespace std;

int nForks = 5;
vector<mutex> mutexes(nForks);

int eatTMin = 500;
int eatTDelta = 1000;

int ponderTMin = 500;
int ponderTDelta = 1000;

auto complainTime = 10000ms;

void philosopher(string name) {
	// philosopher init
	std::chrono::milliseconds eatTime(0);
	std::chrono::milliseconds ponderTime(0);

	srand(stoi(name.substr(12, name.length() - 1)));
	std::this_thread::sleep_for(10000ms / (rand() % 10));
	int mtxN;

	auto searchStart = chrono::high_resolution_clock::now();
	auto searchEnd = chrono::high_resolution_clock::now();
	auto duration = searchEnd - searchStart;

	// philosopher life cycle
	while (true){
		mtxN = rand() % nForks;

		// takes one fork
		if (mutexes[mtxN].try_lock()) {
			searchStart = chrono::high_resolution_clock::now();
			mtxN = rand() % nForks;

			// takes another
			while (true) {
				if (mutexes[mtxN].try_lock()) {
					// eats
					cout << name << " eats" << endl;

					eatTime = (rand() % eatTDelta + eatTMin) * 1ms;
					std::this_thread::sleep_for(eatTime);
					mutexes[mtxN].unlock();

					// ponders
					ponderTime = (rand() % ponderTDelta + ponderTMin) * 1ms;
					std::this_thread::sleep_for(ponderTime);

					// starts searching for forks again
					searchStart = chrono::high_resolution_clock::now();
				}
				else
				{
					searchEnd = chrono::high_resolution_clock::now();

					if (std::chrono::duration_cast<std::chrono::milliseconds>(searchEnd - searchStart) > complainTime) {
						searchStart = chrono::high_resolution_clock::now();
						cout << name << " can't find the second fork for too long!" << endl;
					}
				}
			}
		}
		else
		{
			searchEnd = chrono::high_resolution_clock::now();

			if (std::chrono::duration_cast<std::chrono::milliseconds>(searchEnd - searchStart) > complainTime) {
				searchStart = chrono::high_resolution_clock::now();
				cout << name << " can't find the first fork for too long!" << endl;
			}
		}

	}
}

int main() {
	vector<thread> threads;
	vector<string> names;


	for (int i = 1; i < 6; i++) {
		names.push_back("Philosopher " + to_string(i));
	}

	for (int i = 0; i < 5; i++) {
		threads.emplace_back(philosopher, ref(names[i]));
	}

	for (int i = 0; i < threads.size(); i++) {
		threads[i].join();
	}
}
