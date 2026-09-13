// Problem statement: problem.md

struct Node {  // given node type
    int value;
    Node *next;
};

// Build a list from the sorted values, drop the duplicates and write the result to out.
// Returns the number of items written, or -1 when out_capacity is too small.
extern "C" int dedup_sorted(const int *values, int n, int *out, int out_capacity)
{
    // 1. values를 순서대로 이어 붙여서 연결 리스트를 만든다 (push_back 방식).
    Node *head = nullptr;
    Node *tail = nullptr;
    for (int i = 0; i < n; i++) {
        Node *node = new Node{values[i], nullptr};
        if (head == nullptr) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }

    // 2. 정렬돼 있으므로 같은 값은 항상 붙어 있다.
    //    cur->next가 cur과 같은 값이면 링크에서 끊어서 지우고,
    //    값이 달라질 때만 cur을 다음으로 옮긴다.
    Node *cur = head;
    while (cur != nullptr && cur->next != nullptr) {
        if (cur->next->value == cur->value) {
            Node *dup = cur->next;
            cur->next = dup->next;   // delete 전에 next를 먼저 옮겨 담아 링크를 끊는다
            delete dup;
        } else {
            cur = cur->next;
        }
    }

    // 3. 중복 제거 후 남은 노드 개수를 센다.
    int count = 0;
    for (Node *node = head; node != nullptr; node = node->next) {
        count++;
    }

    // 4. capacity가 부족하면 out에 아무것도 쓰지 않고 -1만 반환한다.
    int result = count;
    if (count > out_capacity) {
        result = -1;
    } else {
        int i = 0;
        for (Node *node = head; node != nullptr; node = node->next) {
            out[i++] = node->value;
        }
    }

    // 5. 만든 노드는 반환 전에 전부 delete한다.
    while (head != nullptr) {
        Node *next = head->next;
        delete head;
        head = next;
    }

    return result;
}
