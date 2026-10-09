"""Problem statement: problem.md"""

from __future__ import annotations

from collections.abc import Sequence


def longest_consecutive(values: Sequence[int]) -> int:
    value_set = set(values)
    best = 0

    for value in value_set:
        if value - 1 in value_set:
            continue

        length = 1
        current = value
        while current + 1 in value_set:
            current += 1
            length += 1

        if length > best:
            best = length

    return best


def longest_consecutive_range(values: Sequence[int]) -> tuple[int, int] | None:
    value_set = set(values)
    best_length = 0
    best_start = None

    for value in value_set:
        if value - 1 in value_set:
            continue

        length = 1
        current = value
        while current + 1 in value_set:
            current += 1
            length += 1

        if length > best_length:
            best_length = length
            best_start = value
        elif length == best_length:
            if best_start is None or value < best_start:
                best_start = value

    if best_start is None:
        return None

    return (best_start, best_start + best_length - 1)
