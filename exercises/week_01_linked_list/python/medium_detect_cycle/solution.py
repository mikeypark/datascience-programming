"""Problem statement: problem.md"""

from __future__ import annotations


class Node:
    """Given node type - do not modify."""

    __slots__ = ("value", "next")

    def __init__(self, value: int, next: Node | None = None) -> None:
        self.value = value
        self.next = next

    def __repr__(self) -> str:
        # Never print next, so that a cyclic list stays safe to repr.
        return f"Node({self.value})"


def detect_cycle(head: Node | None) -> Node | None:
    """Return the node where the cycle starts, or None when there is no cycle."""

    # 사이클 시작 노드, 없으면 None
    if head is None:
        return None

    slow = head
    fast = head

    # 사이클이 있는지 확인
    found = False
    while fast.next is not None and fast.next.next is not None:
        slow = slow.next
        fast = fast.next.next
        if slow is fast:
            found = True
            break

    # 사이클 아님.
    if not found:
        return None

    # 사이클이 있다면 하나씩 전진하면서 찾기
    pt1 = head
    pt2 = slow
    while pt1 is not pt2:
        pt1 = pt1.next
        pt2 = pt2.next

    return pt1


def cycle_length(head: Node | None) -> int:
    """Return the number of nodes in the cycle, or 0 when there is no cycle."""

    start = detect_cycle(head)
    if start is None:
        return 0

    count = 0
    newHead = start
    while start is not None:
        count += 1
        start = start.next
        if start is newHead:
            break

    return count