/* Problem statement: problem.md / function contract: list.h */

#include "list.h"
#include <stdio.h>
#include <string.h>

int sum_values(const Node *head)
{
    if (head == NULL) {
        return 0;
    }

    return head->value + sum_values(head->next);
}

int max_value(const Node *head, int *out)
{
    if (head == NULL) {
        return -1;
    }

    if (head->next == NULL) {
        *out = head->value;
        return 0;
    }

    int rest;
    max_value(head->next, &rest);

    if (head->value > rest) {
        *out = head->value;
    } else {
        *out = rest;
    }

    return 0;
}

static int write_reverse(const Node *head, char *out, int out_capacity, int pos)
{
    if (head == NULL) {
        return pos;
    }

    pos = write_reverse(head->next, out, out_capacity, pos);
    if (pos < 0) {
        return -1;
    }

    char piece[16];
    int len = sprintf(piece, "%d ", head->value);

    if (pos + len >= out_capacity) {
        return -1;
    }

    memcpy(out + pos, piece, len);
    return pos + len;
}

int to_string_reverse(const Node *head, char *out, int out_capacity)
{
    int pos = write_reverse(head, out, out_capacity, 0);
    if (pos < 0) {
        return -1;
    }

    out[pos] = '\0';
    return pos;
}

Node *drop_value(Node *head, int x)
{
    if (head == NULL) {
        return NULL;
    }

    head->next = drop_value(head->next, x);

    if (head->value == x) {
        return head->next;
    }

    return head;
}
