#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <windows.h>
#include <psapi.h>

using namespace std;
using namespace std::chrono;

void insertionSort(vector<int>& arr) {
    int i, j, key;
    for (i = 1; i < arr.size(); i++) {
        key = arr[i];
        j = i - 1;

        // Move elements of arr[0..i-1], that are greater than key, to one position ahead of their current position
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void selectionSort(vector<int>& arr) {
    int i, j, min_idx;

    // One by one move boundary of unsorted subarray
    for (i = 0; i < arr.size() - 1; i++) {
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i + 1; j < arr.size(); j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;

        // Swap the found minimum element with the first element
        swap(arr[min_idx], arr[i]);
    }
}

void bubbleSort(vector<int>& arr) {
    int i, j;
    bool swapped;
    for (i = 0; i < arr.size() - 1; i++) {
        swapped = false;
        for (j = 0; j < arr.size() - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }

        // IF no two elements were swapped by inner loop, then break
        if (!swapped)
            break;
    }
}

int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];  // pivot
    int i = (low - 1);  // Index of smaller element

    for (int j = low; j <= high - 1; j++) {
        // If current element is smaller than the pivot
        if (arr[j] < pivot) {
            i++;  // increment index of smaller element
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        // Separately sort elements before
        // partition and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void merge(vector<int>& arr, int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // Create temp arrays
    vector<int> L(n1), R(n2);

    // Copy data to temp arrays L[] and R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Merge the temp arrays back into arr[l..r]
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(vector<int>& arr, int l, int r) {
    if (l < r) {
        // Same as (l+r)/2, but avoids overflow for large l and h
        int m = l + (r - l) / 2;

        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

void heapify(vector<int>& arr, int n, int i) {
    int largest = i; // Initialize largest as root
    int l = 2 * i + 1; // left = 2*i + 1
    int r = 2 * i + 2; // right = 2*i + 2

    // If left child is larger than root
    if (l < n && arr[l] > arr[largest])
        largest = l;

    // If right child is larger than largest so far
    if (r < n && arr[r] > arr[largest])
        largest = r;

    // If largest is not root
    if (largest != i) {
        swap(arr[i], arr[largest]);

        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest);
    }
}

void heapSort(vector<int>& arr) {
    int n = arr.size();

    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // One by one extract an element from heap
    for (int i = n - 1; i >= 0; i--) {
        // Move current root to end
        swap(arr[0], arr[i]);

        // call max heapify on the reduced heap
        heapify(arr, i, 0);
    }
}


// Function to measure current memory usage
SIZE_T getMemoryUsage() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize;
    }
    else {
        cerr << "Failed to get memory info." << endl;
        return 0;
    }
}

// Template function to test sorting algorithms
template<typename Function>
void testSort(Function sort, const string& name, vector<int>& arr) {
    SIZE_T startMem = getMemoryUsage();
    auto startTime = high_resolution_clock::now();

    sort(arr);  // Sort using provided function

    auto endTime = high_resolution_clock::now();
    SIZE_T endMem = getMemoryUsage();

    auto duration = duration_cast<milliseconds>(endTime - startTime).count();

    if (endMem >= startMem) {
        SIZE_T usedMem = endMem - startMem;
        double memInMB = usedMem / 1048576.0;
        cout << name << " - Time: " << duration << " ms, Memory used: " << usedMem << " bytes / " << memInMB << " megabytes\n";
    }
    else {
        SIZE_T usedMem = startMem - endMem;
        double memInMB = usedMem / 1048576.0;
        cout << name << " - Time: " << duration << " ms, Memory released: " << usedMem << " bytes / " << memInMB << " megabytes\n";
    }
}

int main() {
    srand(time(nullptr)); // Initialize random seed

    vector<int> sizes = { 100, 1000, 10000, 100000, 1000000, 10000000, 50000000 };
    for (int size : sizes) {
        vector<int> arr(size);
        for (int& x : arr) {
            x = rand() % 1000000; // Random values between 0 and 999999
        }

        cout << "Array size: " << size << "\n";
        vector<int> copy = arr;

        /*testSort([&](vector<int>& a) { insertionSort(a); }, "Insertion Sort", arr);
        arr = copy;
        testSort([&](vector<int>& a) { selectionSort(a); }, "Selection Sort", arr);
        arr = copy;
        testSort([&](vector<int>& a) { bubbleSort(a); }, "Bubble Sort", arr);
        arr = copy;*/

        if (size <= 100000) {
            testSort([&](vector<int>& a) { insertionSort(a); }, "Insertion Sort", arr);
            arr = copy;
            testSort([&](vector<int>& a) { selectionSort(a); }, "Selection Sort", arr);
            arr = copy;
            testSort([&](vector<int>& a) { bubbleSort(a); }, "Bubble Sort", arr);
            arr = copy;
        }

        testSort([&](vector<int>& a) { quickSort(a, 0, a.size() - 1); }, "Quick Sort", arr);
        arr = copy;
        testSort([&](vector<int>& a) { mergeSort(a, 0, a.size() - 1); }, "Merge Sort", arr);
        arr = copy;
        testSort([&](vector<int>& a) { heapSort(a); }, "Heap Sort", arr);

        cout << "\n";
    }
    return 0;
}