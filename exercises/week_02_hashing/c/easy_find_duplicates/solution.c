/* Problem statement: problem.md */

#define MAX_VALUE 99

static int out_of_range(int v)
{
    return v < 0 || v > MAX_VALUE;
}

int has_duplicate(const int *values, int n)
{
    for (int i = 0; i < n; i++) {
        if (out_of_range(values[i])) {
            return -1;
        }
    }

    int seen[100] = {0};
    int found = 0;
    for (int i = 0; i < n; i++) {
        if (seen[values[i]] == 1) {
            found = 1;
        }
        seen[values[i]] = 1;
    }

    return found;
}

int first_duplicate(const int *values, int n)
{
    for (int i = 0; i < n; i++) {
        if (out_of_range(values[i])) {
            return -2;
        }
    }

    int seen[100] = {0};
    for (int i = 0; i < n; i++) {
        if (seen[values[i]] == 1) {
            return values[i];
        }
        seen[values[i]] = 1;
    }

    return -1;
}

int find_duplicates(const int *values, int n, int *out, int out_capacity)
{
    for (int i = 0; i < n; i++) {
        if (values[i] < 0 || values[i] > 99) {
            return -2;
        }
    }

    int cnt[100];
    for (int i = 0; i < 100; i++) {
        cnt[i] = 0;
    }
    for (int i = 0; i < n; i++) {
        cnt[values[i]] = cnt[values[i]] + 1;
    }

    int dup_count = 0;
    for (int v = 0; v < 100; v++) {
        if (cnt[v] > 1) {
            dup_count++;
        }
    }

    if (dup_count > out_capacity) {
        return -1;
    }

    int idx = 0;
    for (int v = 0; v < 100; v++) {
        if (cnt[v] > 1) {
            out[idx] = v;
            idx++;
        }
    }

    return idx;
}
