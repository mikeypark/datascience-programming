// Problem statement: problem.md
//
// The grader calls this through ctypes, so the symbol must be exposed as extern "C".

#include <unordered_set>
#include <vector>

extern "C" int intersection(const int *a, int na, const int *b, int nb, int *out,
                            int out_capacity)
{
    std::unordered_set<int> setA;
    for (int i = 0; i < na; i++) {
        setA.insert(a[i]);
    }

    std::unordered_set<int> added;
    std::vector<int> result;

    for (int i = 0; i < nb; i++) {
        if (setA.find(b[i]) != setA.end()) {
            if (added.find(b[i]) == added.end()) {
                result.push_back(b[i]);
                added.insert(b[i]);
            }
        }
    }

    if ((int)result.size() > out_capacity) {
        return -1;
    }

    for (int i = 0; i < (int)result.size(); i++) {
        out[i] = result[i];
    }

    return (int)result.size();
}
