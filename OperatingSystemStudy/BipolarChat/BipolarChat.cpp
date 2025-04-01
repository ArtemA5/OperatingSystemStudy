#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

const wchar_t* SEM_ONE = L"Global\\ChatSemaphoreOne";
const wchar_t* SEM_TWO = L"Global\\ChatSemaphoreTwo";
const wchar_t* SEM_READY = L"Global\\ChatSemaphoreReady";
const char* SHARED_FILE = "buffer.txt";

HANDLE semOne = NULL;
HANDLE semTwo = NULL;
HANDLE semReady = NULL;

void initialize_semaphores() {
    // Create or open semaphores with initial values
    semOne = CreateSemaphoreW(NULL, 1, 1, SEM_ONE);
    semTwo = CreateSemaphoreW(NULL, 0, 1, SEM_TWO);
    semReady = CreateSemaphoreW(NULL, 1, 1, SEM_READY);

    if (semOne == NULL || semTwo == NULL) {
        cerr << "CreateSemaphore error: " << GetLastError() << std::endl;
        ExitProcess(1);
    }
}

void cleanup_semaphores() {
    if (semOne) CloseHandle(semOne);
    if (semTwo) CloseHandle(semTwo);
}

void chat_loop(string userName, HANDLE mySem, HANDLE theirSem) {
    string message;

    while (true) {
        // Waiting for our turn
        WaitForSingleObject(mySem, INFINITE);

        // Read new data from the file
        ifstream infile(SHARED_FILE);
        if (!infile) {
            cerr << "Error opening file for reading\n";
            break;
        }

        getline(infile, message);
        cout << message << endl;
        infile.close();

        // Get user input
        cout << userName << " (You): ";
        getline(cin, message);

        // Write to file
        ofstream outfile(SHARED_FILE);
        if (!outfile) {
            cerr << "Error opening file for writing\n";
            break;
        }

        message = userName + ": " + message + "\n";
        outfile << message;
        outfile.close();

        // Signal that message can be read, give turn to other user
        ReleaseSemaphore(theirSem, 1, NULL);
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

    HANDLE mySem = NULL;
    HANDLE theirSem = NULL;

    DWORD stateOfTheFirst = WaitForSingleObject(semReady, 0);
    if (stateOfTheFirst == WAIT_OBJECT_0) {
        mySem = semOne;
        theirSem = semTwo;

        // File creation
        ofstream file(SHARED_FILE);
        if (!file) {
            cerr << "Error while creating file\n";
            return -1;
        }
        file.close();

        cout << "Waiting for other person to join..." << endl;
        WaitForSingleObject(semReady, INFINITE);
        cout << "Other person has joined successfully!" << endl;
    }
    else
    {
        mySem = semTwo;
        theirSem = semOne;
        ReleaseSemaphore(semReady, 1, NULL);
        if (semReady) CloseHandle(semReady);
    }

    chat_loop(userName, mySem, theirSem);

    return 0;
}