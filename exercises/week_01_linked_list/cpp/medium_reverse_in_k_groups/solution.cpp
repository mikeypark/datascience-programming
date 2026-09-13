// Problem statement: problem.md

struct Node {  // given node type
    int value;
    Node *next;
};

// Build a list from values, reverse every group of k nodes and write the result to out.
// Returns the number of items written (n), or -1 when k < 1 or out_capacity is too small.
extern "C" int reverse_groups(const int *values, int n, int k, int *out, int out_capacity)
{
    if (k < 1 || out_capacity < n) {
        return -1;
    }

    // 1. values를 순서대로 이어 붙여서 리스트를 만든다.
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

    // 2. 더미 헤드를 두면 첫 묶음도 "이전 묶음 뒤에 잇는" 처리와 똑같이 다룰 수 있다.
    //    prevTail은 항상 "이미 뒤집힌 부분의 마지막 노드"를 가리킨다.
    Node dummy{0, head};
    Node *prevTail = &dummy;

    while (true) {
        // 2-1. 뒤집기 전에 남은 노드가 k개 이상인지 먼저 확인한다.
        Node *probe = prevTail->next;
        int count = 0;
        while (probe != nullptr && count < k) {
            probe = probe->next;
            count++;
        }
        if (count < k) {
            break;   // 남은 게 k개 미만이면 그대로 둔다.
        }

        // 2-2. 이 묶음(k개)만 링크 방향을 뒤집는다.
        Node *groupStart = prevTail->next;   // 뒤집고 나면 이 묶음의 꼬리가 된다.
        Node *prev = nullptr;
        Node *cur = groupStart;
        for (int i = 0; i < k; i++) {
            Node *next = cur->next;
            cur->next = prev;
            prev = cur;
            cur = next;
        }

        // 2-3. 이전 묶음의 꼬리를 새 묶음의 머리(prev)에 연결하고,
        //      뒤집힌 묶음의 꼬리(groupStart)를 다음 묶음의 시작(cur)에 연결한다.
        prevTail->next = prev;
        groupStart->next = cur;
        prevTail = groupStart;
    }

    head = dummy.next;

    // 3. 결과를 out에 쓴다 (capacity는 이미 앞에서 확인했다).
    int i = 0;
    for (Node *node = head; node != nullptr; node = node->next) {
        out[i++] = node->value;
    }

    // 4. 만든 노드는 반환 전에 전부 delete한다.
    while (head != nullptr) {
        Node *next = head->next;
        delete head;
        head = next;
    }

    return i;
}
