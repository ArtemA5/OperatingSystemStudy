#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <chrono>
#include <thread>

using namespace std;

const wchar_t* SEM_READ = L"Global\\ChatSemaphoreRead";
const wchar_t* SEM_WRITE = L"Global\\ChatSemaphoreWrite";
const char* SHARED_FILE = "buffer.txt";

chrono::milliseconds COOLDOWN(50);
HANDLE semRead = NULL;
HANDLE semWrite = NULL;

void initialize_semaphores(){{;

    semRead = CreateSemaphoreW(NULL, 0, 1, SEM_READ);
    semWrite = CreateSemaphoreW(NULL, 1, 1, SEM_WRITE);
    }
}

void cleanup_semaphores() {
    if (semRead) CloseHandle(semRead);
    if (semWrite) CloseHandle(semWrite);
}


void write(string userName) {
    string message;

    while (true) {

        // Get user input
        getline(cin, message);

        WaitForSingleObject(semWrite, INFINITE);

        // Write to file
        ofstream outfile(SHARED_FILE);
        if (!outfile) {
            cerr << "Error opening file for writing\n";
            break;
        }

        message = userName + ": " + message;
        outfile << message;
        outfile.close();

        // Let processes read file
        ReleaseSemaphore(semRead, 1, NULL);
        std::this_thread::sleep_for(COOLDOWN);
        WaitForSingleObject(semRead, INFINITE);

        // Signal that message can be read, give turn to other user
        ReleaseSemaphore(semWrite, 1, NULL);
    }
}


void read(string userName) {
    string message;

    while (true) {

        WaitForSingleObject(semRead, INFINITE);
        ReleaseSemaphore(semRead, 1, NULL);


        // Read new data from the file
        ifstream infile(SHARED_FILE);
        if (!infile) {
            cerr << "Error opening file for reading\n";
            break;
        }

        getline(infile, message);

        if (message.length() > userName.length()) {
            if (message.substr(0, userName.length() + 1) != userName + ":") {cout << message << endl;}
        }
        else { cout << message << endl; }

        infile.close();
        std::this_thread::sleep_for(COOLDOWN * 2);
    }
}


int main() {
    initialize_semaphores();
    
    // Set up cleanup on exit
    atexit(cleanup_semaphores);

    // User's name
    string userName;
    cout << "Enter your name: ";
    getline(cin, userName);

    thread th_write([userName] {write(userName); });
    thread th_read([userName] {read(userName); });

    th_write.join();
    th_read.join();

    return 0;
}