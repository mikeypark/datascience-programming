// Problem statement: problem.md
//
// The grader calls these through ctypes, so the symbols must be extern "C".
// All three must be written recursively: no for, no while.

#include <cctype>

static int reverse_helper(const char *s, int i, char *out, int out_capacity, int pos)
{
    if (s[i] == '\0') {
        return pos;
    }

    pos = reverse_helper(s, i + 1, out, out_capacity, pos);
    if (pos < 0) {
        return -1;
    }

    if (pos + 1 >= out_capacity) {
        return -1;
    }

    out[pos] = s[i];
    return pos + 1;
}

extern "C" int reverse_string(const char *s, char *out, int out_capacity)
{
    int pos = reverse_helper(s, 0, out, out_capacity, 0);
    if (pos < 0) {
        return -1;
    }

    out[pos] = '\0';
    return pos;
}

extern "C" int count_vowels(const char *s)
{
    if (s[0] == '\0') {
        return 0;
    }

    char c = tolower(s[0]);
    int is_vowel = (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');

    return is_vowel + count_vowels(s + 1);
}

static int subseq_helper(const char *a, int i, const char *b, int j)
{
    if (a[i] == '\0') {
        return 1;
    }

    if (b[j] == '\0') {
        return 0;
    }

    if (a[i] == b[j]) {
        return subseq_helper(a, i + 1, b, j + 1);
    }

    return subseq_helper(a, i, b, j + 1);
}

extern "C" int is_subsequence(const char *a, const char *b)
{
    return subseq_helper(a, 0, b, 0);
}
