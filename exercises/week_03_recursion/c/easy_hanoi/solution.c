/* Problem statement: problem.md */

#define MAX_DISKS 31

/* Minimum number of moves for n disks. -1 when n is out of range. */
int hanoi_count(int n)
{
    if (n < 0 || n > MAX_DISKS) {
        return -1;
    }
    if (n == 0) {
        return 0;
    }
    return 2 * hanoi_count(n - 1) + 1;
}

static int solve(int n, int from, int to, int via, int *out, int out_capacity, int written)
{
    if (n == 0) {
        return written;
    }
    written = solve(n - 1, from, via, to, out, out_capacity, written);
    if (written < 0) {
        return -1;
    }
    if (written + 2 > out_capacity) {
        return -1;
    }
    out[written] = from;
    out[written + 1] = to;
    written += 2;
    written = solve(n - 1, via, to, from, out, out_capacity, written);
    return written;
}

/* Writes two ints per move (from peg, to peg) and returns the number of moves.
 * -1 when n is out of range or out_capacity is too small. */
int hanoi_moves(int n, int *out, int out_capacity)
{
    if (n < 0 || n > MAX_DISKS) {
        return -1;
    }
    if (n == 0) {
        return 0;
    }

    int written = solve(n, 1, 3, 2, out, out_capacity, 0);
    if (written < 0) {
        return -1;
    }
    return written / 2;
}
