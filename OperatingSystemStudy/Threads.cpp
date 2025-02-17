#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>

using namespace std;

// One row calculation
//
void calculateRow(int row, const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C) {
    int aRows = A.size();
    int aCols = A[0].size();
    int bRows = B.size();
    int bCols = B[0].size();

    for (int j = 0; j < bCols; ++j) {
        int sum = 0;
        for (int k = 0; k < aCols; ++k) {
            sum += A[row][k] * B[k][j];
        }
        C[row][j] = sum;
    }
}

// Matrix multiplication
//
vector<vector<int>> multiThreadedMultiply(const vector<vector<int>>& A, const vector<vector<int>>& B) 
{
    int aRows = A.size();
    int aCols = A[0].size();
    int bRows = B.size();
    int bCols = B[0].size();

    if (aCols != bRows) return {};

    vector<vector<int>> C(aRows, vector<int>(bCols, 0)); // Creation of a new matrix
    vector<thread> threads; // Vector with threads


    for (int i = 0; i < aRows; ++i) {
        // Creation of new threads with a function that calculates one row
        threads.emplace_back([i, &A, &B, &C]() {calculateRow(i, A, B, C); }); 
    }

    for (int i = 0; i < threads.size(); ++i) { // Thread waiting
        threads[i].join();
    }

    return C;
}

// Single threaded multiplication
//
vector<vector<int>> singleThreadedMultiply(const vector<vector<int>>& A, const vector<vector<int>>& B) 
{
    int aRows = A.size();
    int aCols = A[0].size();
    int bRows = B.size();
    int bCols = B[0].size();

    if (aCols != bRows) return {};

    vector<vector<int>> C(aRows, vector<int>(bCols, 0));

    for (int i = 0; i < aRows; ++i) {
        for (int j = 0; j < bCols; ++j) {
            int sum = 0;
            for (int k = 0; k < aCols; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }

    return C;
}

int main() {
    const int N = 100; // Size of matrices

    // Creation of matrices with size N x N
    vector<vector<int>> A(N, vector<int>(N));
    vector<vector<int>> B(N, vector<int>(N));

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }
    }

    // Single threaded calculations
    auto start = chrono::high_resolution_clock::now();
    auto C_single = singleThreadedMultiply(A, B);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> single_time = end - start;

    // Multi threaded calculations
    start = chrono::high_resolution_clock::now();
    auto C_multi = multiThreadedMultiply(A, B);
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> multi_time = end - start;

    // Results printing
    cout << "Singlethread calculations: " << single_time.count() << " second\n";
    cout << "Multithread calculations: " << multi_time.count() << " second\n";

    return 0;
}