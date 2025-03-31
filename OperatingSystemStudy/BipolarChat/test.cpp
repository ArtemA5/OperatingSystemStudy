#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <thread>

using namespace std;

void write() {
	string output;
	while (true) {
		cin >> output;
	}
}

void print() {
	while (true) {
		this_thread::sleep_for(chrono::milliseconds(3000));;
		cout << "hello!\n";
	}
}

int main() {
	thread th1(write);
	thread th2(print);

	th1.join();
	th2.join();
}