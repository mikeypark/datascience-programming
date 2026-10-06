// Problem statement: problem.md
//
// The grader calls this through ctypes, so the symbol must be exposed as extern "C".

#include <vector>

namespace {

void merge(std::vector<int> &arr, int lo, int mid, int hi, std::vector<int> &temp)
{
    int i = lo;
    int j = mid + 1;
    int k = lo;

    while (i <= mid && j <= hi) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }
    while (i <= mid) {
        temp[k++] = arr[i++];
    }
    while (j <= hi) {
        temp[k++] = arr[j++];
    }
    for (int idx = lo; idx <= hi; ++idx) {
        arr[idx] = temp[idx];
    }
}

void mergeSort(std::vector<int> &arr, int lo, int hi, std::vector<int> &temp)
{
    if (lo >= hi) {
        return;
    }
    int mid = lo + (hi - lo) / 2;
    mergeSort(arr, lo, mid, temp);
    mergeSort(arr, mid + 1, hi, temp);
    merge(arr, lo, mid, hi, temp);
}

} // namespace

// Sorts values ascending into out without touching the input.
// Returns the number of items written (n), or -1 when out_capacity is too small.
extern "C" int merge_sort(const int *values, int n, int *out, int out_capacity)
{
    if (n < 0 || out_capacity < n) {
        return -1;
    }
    if (n == 0) {
        return 0;
    }

    std::vector<int> arr(values, values + n);
    std::vector<int> temp(n);
    mergeSort(arr, 0, n - 1, temp);

    for (int i = 0; i < n; ++i) {
        out[i] = arr[i];
    }
    return n;
}
