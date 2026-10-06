"""Problem statement: problem.md"""

from __future__ import annotations

from collections.abc import Sequence


def permutations(values: Sequence[int]) -> list[list[int]]:
    """Every ordering of the values. The empty input has one permutation: []."""
    result = []
    _build_permutation(list(values), [], result)
    return result


def _build_permutation(remaining, chosen, result):
    if len(remaining) == 0:
        # 더 고를 값이 없으니, 지금까지 고른 순서 하나를 답에 담는다.
        result.append(chosen[:])
        return

    for i in range(len(remaining)):
        value = remaining[i]
        # remaining 에서 i번째 값만 빼고 나머지로 새 리스트를 만든다.
        rest = remaining[:i] + remaining[i + 1:]

        chosen.append(value)
        _build_permutation(rest, chosen, result)
        chosen.pop()  # 되돌리기: 다음 값을 고르기 전에 지금 고른 값을 뺀다.


def subsets(values: Sequence[int]) -> list[list[int]]:
    """Every subset, each keeping the input order. The empty input has one subset: []."""
    result = []
    _build_subset(values, 0, [], result)
    return result


def _build_subset(values, index, chosen, result):
    if index == len(values):
        # 끝까지 다 봤으니, 지금까지 고른 부분집합 하나를 답에 담는다.
        result.append(chosen[:])
        return

    chosen.append(values[index])
    _build_subset(values, index + 1, chosen, result)
    chosen.pop()  # 되돌리기: 넣었던 값을 뺀다.

    _build_subset(values, index + 1, chosen, result)
