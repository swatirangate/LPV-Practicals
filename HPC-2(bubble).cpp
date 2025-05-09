#include <omp.h>
#include <iostream>
#include <string>
#include <chrono>   // bench marking

using namespace std::chrono;
using namespace std;

void displayArray(int nums[], int length)
{
    cout << "\t" << ": [";
    for (int i = 0; i < length; i++)
    {
        cout << nums[i];
        if (i != length - 1)
            cout << ", ";
    }
    cout << "]" << endl;
}

void merge(int nums[], int leftStart, int leftEnd, int rightStart, int rightEnd)
{
    int n = (rightEnd - leftStart) + 1; // Size of both arrays
    int tempArray[n];

    int t = 0;           // Index for temporary array
    int l = leftStart;   // Index for left array
    int r = rightStart;  // Index for right array

    // Merge both arrays into tempArray
    while (l <= leftEnd && r <= rightEnd)
    {
        if (nums[l] <= nums[r])
            tempArray[t++] = nums[l++];
        else
            tempArray[t++] = nums[r++];
    }

    // Copy remaining elements from left array
    while (l <= leftEnd)
        tempArray[t++] = nums[l++];

    // Copy remaining elements from right array
    while (r <= rightEnd)
        tempArray[t++] = nums[r++];

    // Copy back to original array
    for (int i = 0; i < n; i++)
        nums[leftStart + i] = tempArray[i];
}

void mergeSort(int nums[], int start, int end)
{
    if (start < end)
    {
        int mid = (start + end) / 2;
#pragma omp parallel sections num_threads(2)
        {
#pragma omp section
            mergeSort(nums, start, mid);
#pragma omp section
            mergeSort(nums, mid + 1, end);
        }
        merge(nums, start, mid, mid + 1, end);
    }
}

void seq_mergeSort(int nums[], int start, int end)
{
    if (start < end)
    {
        int mid = (start + end) / 2;
        seq_mergeSort(nums, start, mid);
        seq_mergeSort(nums, mid + 1, end);
        merge(nums, start, mid, mid + 1, end);
    }
}

void bubbleSort(int nums[], int length)
{
    for (int i = 0; i < length; i++)
    {
        int start = i % 2; // Start from 0 if i is even else 1
#pragma omp parallel for
        for (int j = start; j < length - 1; j += 2)
        {
            if (nums[j] > nums[j + 1])
            {
                int temp = nums[j];
                nums[j] = nums[j + 1];
                nums[j + 1] = temp;
            }
        }
    }
}

void seq_bubbleSort(int nums[], int length) {
   
    for (int i = 0; i < length - 1; i++) {
        for (int j = 0; j < length - i - 1; j++) {
            if (nums[j] > nums[j + 1]) {
                int temp = nums[j];
                nums[j] = nums[j + 1];
                nums[j + 1] = temp;
            }
        }
    }
}

int main()
{
    int length1;
    cout << "Enter length of array: ";
    cin >> length1;

    int original[length1];
    cout << "Enter elements of array: ";
    for (int i = 0; i < length1; i++)
        cin >> original[i];

    // Create copies for each sorting algorithm
    int nums_bubble[length1];
    int nums_merge[length1];
    int nums_seq_bubble[length1];
    int nums_seq_merge[length1];

    for (int i = 0; i < length1; i++) {
        nums_bubble[i] = original[i];
        nums_merge[i] = original[i];
        nums_seq_bubble[i] = original[i];
        nums_seq_merge[i] = original[i];
    }

    cout<<"\nSequential Bubble Sort:\n";
    auto start_seq_bubble = high_resolution_clock::now();
    seq_bubbleSort(nums_seq_bubble, length1);
    auto end_seq_bubble = high_resolution_clock::now();
    displayArray(nums_seq_bubble, length1);
    auto duration_seq_bubble = duration_cast<microseconds>(end_seq_bubble - start_seq_bubble);
    cout << "Execution time for sequential Bubble Sort: " << duration_seq_bubble.count() << " microseconds\n";

    cout << "\nParallel Bubble Sort:\n";
    auto start_bubble = high_resolution_clock::now();
    bubbleSort(nums_bubble, length1);
    auto end_bubble = high_resolution_clock::now();
    displayArray(nums_bubble, length1);
    auto duration_bubble = duration_cast<microseconds>(end_bubble - start_bubble);
    cout << "Execution time for Bubble Sort: " << duration_bubble.count() << " microseconds\n";
    
    cout << "\nSequential Merge Sort:\n";
    auto start_seq_merge = high_resolution_clock::now();
    seq_mergeSort(nums_seq_merge, 0, length1 - 1);
    auto end_seq_merge = high_resolution_clock::now();
    displayArray(nums_seq_merge, length1);
    auto duration_seq_merge = duration_cast<microseconds>(end_seq_merge - start_seq_merge);
    cout << "Execution time for Merge Sort: " << duration_seq_merge.count() << " microseconds\n";

    cout << "\nParallel Merge Sort:\n";
    auto start_merge = high_resolution_clock::now();
    mergeSort(nums_merge, 0, length1 - 1);
    auto end_merge = high_resolution_clock::now();
    displayArray(nums_merge, length1);
    auto duration_merge = duration_cast<microseconds>(end_merge - start_merge);
    cout << "Execution time for Merge Sort: " << duration_merge.count() << " microseconds\n";

    return 0;
}


/* 
    int nums1[] = {4, 6, 2, 0, 7, 6, 1, 9, -3, -5};
    int length1 = sizeof(nums1) / sizeof(int);
*/
