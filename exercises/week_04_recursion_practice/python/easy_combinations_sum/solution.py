"""Problem statement: problem.md"""

from __future__ import annotations

from collections.abc import Sequence


def combinations_with_sum(values: Sequence[int], k: int, target: int) -> list[list[int]]:
    """Every way to pick k values (in input order) that add up to target."""
    if k < 0 or k > len(values):
        return []

    result = []
    _build_combination(values, 0, k, target, [], 0, result)
    return result


def _build_combination(values, index, k, target, chosen, total, result):
    if len(chosen) == k:
        if total == target:
            result.append(chosen[:])
        return

    if index == len(values):
        return

    chosen.append(values[index])
    _build_combination(values, index + 1, k, target, chosen, total + values[index], result)
    chosen.pop()

    _build_combination(values, index + 1, k, target, chosen, total, result)
