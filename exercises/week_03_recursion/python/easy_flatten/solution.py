"""Problem statement: problem.md"""

from __future__ import annotations


def flatten(nested: list) -> list[int]:
    """Every integer in order, with all nesting removed."""
    result = []
    for item in nested:
        if isinstance(item, list):
            # item 도 리스트니까, 똑같이 펼쳐서 하나씩 담는다.
            inner = flatten(item)
            for value in inner:
                result.append(value)
        else:
            result.append(item)
    return result


def depth(nested: list) -> int:
    """How many list layers deep the structure goes at its deepest point."""
    deepest = 0
    for item in nested:
        if isinstance(item, list):
            item_depth = depth(item)
            if item_depth > deepest:
                deepest = item_depth
    return 1 + deepest
