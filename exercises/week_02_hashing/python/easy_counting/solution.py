"""Problem statement: problem.md"""

from __future__ import annotations

from collections.abc import Iterable


def count_values(values: Iterable[int]) -> dict[int, int]:
    result = {}
    for value in values:
        if value in result:
            result[value] = result[value] + 1
        else:
            result[value] = 1
    return result


def first_unique(text: str) -> str | None:
    counts = {}
    for ch in text:
        counts[ch] = counts.get(ch, 0) + 1

    for ch in text:
        if counts[ch] == 1:
            return ch

    return None


def is_anagram(left: str, right: str) -> bool:
    if len(left) != len(right):
        return False

    counts = {}
    for ch in left:
        if ch in counts:
            counts[ch] += 1
        else:
            counts[ch] = 1

    for ch in right:
        if ch not in counts:
            return False
        counts[ch] -= 1

    for ch in counts:
        if counts[ch] != 0:
            return False

    return True
