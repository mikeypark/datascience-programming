// Problem statement: problem.md
//
// The grader calls this through ctypes, so the symbol must be exposed as extern "C".

#include <unordered_map>
#include <vector>

extern "C" int subarray_sum(const int *values, int n, int k)
{
    std::vector<long long> prefix;
    prefix.push_back(0);

    long long running = 0;
    for (int i = 0; i < n; i++) {
        running += values[i];
        prefix.push_back(running);
    }

    std::unordered_map<long long, int> seen;
    int total = 0;

    for (int i = 0; i < (int)prefix.size(); i++) {
        long long need = prefix[i] - k;

        if (seen.find(need) != seen.end()) {
            total += seen[need];
        }

        seen[prefix[i]] = seen[prefix[i]] + 1;
    }

    return total;
}
