# Week 01 연결 리스트 — 상세 풀이 노트

C / C++ / Python 세 언어로 푼 5개 과제를 다시 뜯어봅니다.
정답을 외우는 게 아니라, 포인터가 매 줄마다 어디를 가리키는지 직접 따라가며 이해하는 게 목표입니다.

- 5개 과제, 3개 언어 (C · C++ · Python)
- 공통 주제: 링크를 직접 끊고 잇기
- 공통 함정: free/delete 순서, null 체크

## 목차

1. [언어별 차이 & 복잡도](#언어별-차이--복잡도)
2. [C · 단일 연결 리스트](#c--단일-연결-리스트)
3. [C++ · 중복 제거 (쉬움)](#c--정렬된-리스트에서-중복-제거)
4. [C++ · k개씩 묶어 뒤집기 (중간)](#c--k개씩-묶어서-뒤집기)
5. [Python · 리스트 뒤집기 (쉬움)](#python--리스트-뒤집기와-변환)
6. [Python · 사이클 탐지 (중간)](#python--사이클-찾기--플로이드의-토끼와-거북이)

---

## 언어별 차이 & 복잡도

다섯 문제 전부 "노드를 직접 이어 붙이고, 링크를 다시 연결한다"는 같은 동작을 합니다.
언어가 달라지는 건 그 동작을 **누가 책임지느냐**입니다.

| | 메모리 관리 | 핵심 차이 |
|---|---|---|
| **C** | `malloc`으로 만든 노드는 `free`로 내가 직접 지워야 합니다 | head/tail 포인터도 손으로 갱신하지 않으면 리스트가 깨집니다 |
| **C++** | `new`/`delete` | 이번 과제는 일부러 `Node*`를 손으로 조작합니다. 채점기가 부르는 함수만 `extern "C"`로 노출하면 됩니다 |
| **Python** | GC가 자동 수거 | 노드를 따로 `free`할 필요는 없지만, 재귀는 `RecursionError`가 나서 전부 반복문으로 풉니다 |

### 로제타 스톤 — 같은 동작, 다른 문법

언어 3개가 섞이면 헷갈리는 게 당연합니다. 사실 알고리즘은 다섯 문제 전부 동일합니다
(3-포인터로 방향 뒤집기, 링크 끊고 잇기, 두 포인터가 같은 노드인지 보기).
달라지는 건 딱 세 가지 — **①메모리를 누가 정리하는지, ②포인터/참조 문법, ③함수를 채점기에 어떻게 노출하는지** — 뿐입니다.

| 동작 | C | C++ | Python |
|---|---|---|---|
| 노드 타입 정의 | `struct Node { int value; struct Node *next; };` | `struct Node { int value; Node *next; };` (`struct Node`를 안 써도 그냥 `Node`라고 적을 수 있음) | `class Node:`<br>`  def __init__(self, value, next=None):`<br>`    self.value = value`<br>`    self.next = next` |
| 빈 리스트 / 리스트 끝 | `NULL` | `nullptr` | `None` |
| 노드 하나 새로 만들기 | `Node *n = malloc(sizeof(Node));`<br>`n->value = v; n->next = NULL;` (malloc 실패 시 NULL — 직접 체크 필요) | `Node *n = new Node{v, nullptr};` (실패하면 예외를 던짐) | `n = Node(v)` (실패 걱정 없음) |
| 다음 노드로 이동 | `n = n->next;` | `n = n->next;` | `n = n.next` (포인터가 아니라 참조라 화살표 없음) |
| 두 변수가 "같은 노드"인지 비교 | `if (n1 == n2)` | `if (n1 == n2)` | `if n1 is n2:` (`==`는 값 비교라 커스텀될 수 있음. "같은 객체인지"는 반드시 `is`) |
| 다 쓴 노드 정리 | `free(n);` (안 하면 메모리 누수) | `delete n;` (안 하면 메모리 누수, new/delete 짝을 맞출 것) | 필요 없음 — 참조 없는 노드는 GC가 자동 회수 |
| 채점기에 함수 노출 | `int f(...) { ... }` | `extern "C" int f(...) { ... }` (C++는 name mangling을 하므로 ctypes가 찾을 수 있게 C 이름 규칙 강제) | `def f(...): ...` (ctypes 안 거치고 파이썬이 직접 호출) |

### 복잡도 요약

| 과제 | 언어 | 시간 | 추가 공간 | 핵심 기법 |
|---|---|---|---|---|
| list_index_of / list_reverse | C | O(n) | O(1) | 순차 탐색 · 3-포인터 뒤집기 |
| 정렬된 리스트 중복 제거 | C++ | O(n) | O(1) | 인접 노드 비교 후 링크 끊기 |
| k개씩 묶어 뒤집기 | C++ | O(n) | O(1) | 더미 헤드 + 그룹별 3-포인터 뒤집기 |
| reverse (리스트 뒤집기) | Python | O(n) | O(1) | 3-포인터 반복 뒤집기 |
| detect_cycle / cycle_length | Python | O(n) | O(1) | Floyd 토끼와 거북이 |

---

## C · 단일 연결 리스트

`list_create`부터 `list_to_array`까지 전체 API를 직접 만드는 과제였습니다.
여기서는 새로 채워 넣은 두 함수, **순차 탐색**과 **제자리 뒤집기**를 집중해서 봅니다.

```
head → [3|next] → [7|next] → [1|next] → NULL
```

`List` 구조체가 `head`와 `tail` 포인터를 둘 다 들고 있어서, `push_back`과 `list_size`를 O(1)로 처리합니다.

### list_index_of — 값이 있는 위치 찾기

```c
int list_index_of(const List *list, int value)
{
    Node *n = list->head;
    int i = 0;
    while (n != NULL) {
        if (n->value == value) {
            return i;
        }
        n = n->next;
        i++;
    }

    return -1;
}
```

1. head부터 시작해서 `n`이 NULL이 될 때까지, 즉 리스트 끝에 닿을 때까지 순회합니다.
2. 매 노드에서 값이 먼저 일치하는지 확인하고, 일치하면 그 자리에서 바로 인덱스 `i`를 반환합니다 — "첫 번째" 일치를 찾는 문제라 더 갈 필요가 없습니다.
3. 일치하지 않으면 `n`을 다음 노드로, `i`를 하나 증가시키고 계속합니다.
4. 루프가 끝날 때까지 못 찾았다면 `-1`. 값이 존재하지 않는다는 뜻입니다.

### list_reverse — 링크 방향을 그 자리에서 뒤집기

```c
void list_reverse(List *list)
{
    Node *prev = NULL;
    Node *curr = list->head;

    list->tail = list->head;
    while (curr != NULL) {
        Node *next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    list->head = prev;
}
```

```
before:  head → [3] → [7] → [1] → NULL            (tail)
after :  NULL ← [3] ← [7] ← [1]  (tail)   (head)
```

1. `prev = NULL, curr = head`로 시작합니다. prev는 "이미 뒤집은 부분의 새 머리", curr는 "지금 뒤집을 노드"입니다.
2. curr을 고치기 **전에** `next = curr->next`로 다음 노드를 먼저 저장합니다. 안 그러면 링크를 끊는 순간 나머지 리스트로 가는 길을 잃습니다.
3. `curr->next = prev`로 화살표를 뒤로 돌립니다.
4. prev와 curr을 한 칸씩 앞으로 민 뒤(`prev = curr; curr = next;`), curr이 NULL이 될 때까지 반복합니다.
5. 루프가 끝나면 `prev`가 새 head입니다. 원래 head였던 노드는 이제 맨 끝이므로 `list->tail`로 미리 챙겨 둡니다 (루프 시작 전에).

> **흔한 실수**
> - `curr->next`를 먼저 바꿔버리고 나서 다음 노드를 찾으려 하면, 그 순간 원래의 다음 노드로 가는 유일한 길이 사라집니다. 항상 next부터 저장하세요.
> - `list->tail` 갱신을 빼먹기 쉽습니다. 뒤집기 전 head가 뒤집은 후엔 tail이 된다는 걸 기억하세요.

<details>
<summary>생각해 볼 것 — push_back을 매번 끝까지 순회해서 구현하면?</summary>

노드를 n개 넣을 때 매번 tail을 처음부터 찾으면 1+2+...+n = O(n²)입니다. tail 포인터를 따로 들고 있으면 매 삽입이 O(1)로 줄어서 전체가 O(n)이 됩니다. 이 프로젝트의 `List` 구조체가 `tail` 필드를 갖는 이유입니다.
</details>

<details>
<summary>생각해 볼 것 — list_remove에서 지울 노드의 이전 노드는 어떻게 잡나요?</summary>

index-1까지 순회해서 prev를 찾고, `prev->next = target->next`로 target을 건너뜁니다. index가 0(head 삭제)이면 prev가 없으므로 따로 분기해서 head 자체를 옮겨야 합니다 — 더미 헤드를 쓰면 이 분기를 없앨 수 있는데, k개씩 뒤집기 문제에서 바로 그 방법을 씁니다.
</details>

---

## C++ · 정렬된 리스트에서 중복 제거

정렬된 배열을 리스트로 만든 뒤, 이어진 중복 구간을 하나만 남기고 링크를 끊어 지웁니다.
정렬돼 있다는 조건 덕분에 같은 값은 항상 옆에 붙어 있다는 게 핵심입니다.

```
입력: [1, 1, 2, 3, 3, 3]  →  결과: [1, 2, 3]
```

```cpp
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
```

1. `cur->next`가 `cur`과 값이 같으면, 그 노드는 지워야 할 중복입니다.
2. 지우기 전에 **`dup->next`를 `cur->next`에 먼저 옮겨서** 링크를 건너뛰게 만듭니다. 이 대입이 끝나야 `dup`이 리스트에서 완전히 분리됩니다.
3. 그 다음에야 `delete dup`이 안전합니다. 순서를 반대로 하면 delete된 메모리의 `next`를 읽는 use-after-free가 됩니다.
4. 값이 다를 때만 `cur`을 앞으로 옮깁니다 — 그래서 같은 값이 3개, 4개로 이어져도 `cur`은 제자리에서 계속 다음 중복을 끊어낼 수 있습니다.

> **왜 정렬 조건이 필요한가**
> - 이 방법은 `cur`과 바로 다음 노드만 비교합니다. 정렬 안 된 `[1, 2, 1]`이면 두 1이 붙어있지 않아서 놓칩니다.
> - 정렬 안 된 입력의 중복 제거는 이미 본 값을 `set`에 저장해야 해서 O(n) 추가 메모리가 필요해집니다.

<details>
<summary>생각해 볼 것 — capacity가 부족하면 왜 개수부터 세야 하나요?</summary>

"부족하면 아무것도 쓰지 않는다"는 조건 때문에, out에 쓰기 시작한 뒤에 capacity 초과를 발견하면 이미 일부를 써버린 뒤라 되돌릴 방법이 없습니다. 그래서 먼저 전체 개수를 세고 capacity와 비교한 다음에 쓰기 시작해야 합니다.
</details>

---

## C++ · k개씩 묶어서 뒤집기

앞에서부터 k개씩 묶어 각 묶음만 뒤집고, 묶음의 순서는 그대로 유지합니다.
마지막에 k개가 안 되는 묶음은 손대지 않습니다.

```
[1,2,3,4,5], k=2

before : [1 → 2] → [3 → 4] → [5]
after  : [2 → 1] → [4 → 3] → [5]
```

```cpp
Node dummy{0, head};
Node *prevTail = &dummy;   // "이미 뒤집힌 부분의 마지막 노드"

while (true) {
    // 뒤집기 전에 남은 노드가 k개 이상인지 먼저 확인
    Node *probe = prevTail->next;
    int count = 0;
    while (probe != nullptr && count < k) {
        probe = probe->next;
        count++;
    }
    if (count < k) break;   // 남은 게 k개 미만이면 그대로 둔다

    Node *groupStart = prevTail->next;   // 뒤집고 나면 이 묶음의 꼬리가 된다
    Node *prev = nullptr;
    Node *cur = groupStart;
    for (int i = 0; i < k; i++) {
        Node *next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }

    prevTail->next = prev;      // 이전 묶음 꼬리 → 새 묶음 머리
    groupStart->next = cur;     // 뒤집힌 묶음 꼬리 → 다음 묶음 시작
    prevTail = groupStart;
}
head = dummy.next;
```

1. **더미 헤드**(`dummy`)를 진짜 head 앞에 가짜로 하나 둡니다. 그러면 "첫 묶음을 뒤집을 때 이전 묶음이 없다"는 특수 케이스가 사라집니다 — `prevTail`이 항상 존재하니까요.
2. **뒤집기 전에 먼저 k개가 남았는지 센다** (문제의 힌트 그대로). 안 세고 뒤집었다가 k개가 안 됐으면, 이미 방향이 바뀐 링크를 다시 원래대로 되돌려야 해서 훨씬 번거로워집니다.
3. k개가 확인되면, list_reverse와 똑같은 3-포인터 뒤집기를 이 구간에만 적용합니다.
4. 뒤집고 나면 `groupStart`(원래 시작 노드)가 이 묶음의 꼬리가 되므로, 두 군데를 이어 붙여야 합니다: 이전 묶음 꼬리 → 새 머리(`prev`), 그리고 이 묶음의 새 꼬리(`groupStart`) → 다음 묶음 시작(`cur`).
5. `prevTail`을 `groupStart`로 옮기고 다음 라운드로 — 이 라운드의 꼬리가 다음 라운드의 "이전 꼬리"가 됩니다.

<details>
<summary>생각해 볼 것 — 묶음 하나를 뒤집으면 머리와 꼬리가 바뀝니다. 포인터가 몇 개 필요한가요?</summary>

뒤집기 자체에 prev/cur/next 3개, 그리고 그 결과를 바깥과 잇기 위해 prevTail(이전 묶음 꼬리)과 groupStart(이번 묶음의 새 꼬리) 2개가 더 필요합니다. 총 5개의 포인터가 동시에 의미를 가집니다.
</details>

<details>
<summary>생각해 볼 것 — 추가 메모리를 상수로 끝낼 수 있나요?</summary>

네. 위 코드가 쓰는 건 dummy, prevTail, probe, groupStart, prev, cur, next 뿐이고 전부 노드 개수와 무관하게 고정된 개수입니다. 배열에 값을 복사해서 뒤집는 방식은 O(n) 추가 메모리가 필요하지만, 링크만 다시 잇는 이 방식은 O(1)입니다.
</details>

---

## Python · 리스트 뒤집기와 변환

파이썬 리스트 ↔ 연결 리스트 변환 두 개와, C에서 했던 것과 같은 제자리 뒤집기입니다.
다른 점은 딱 하나 — 새 노드를 만들지 않고 기존 노드의 `next`만 다시 잇습니다.

```python
def reverse(head: Node | None) -> Node | None:
    prevNode = None
    currNode = head
    nextNode = None

    while currNode is not None:
        nextNode = currNode.next
        currNode.next = prevNode

        prevNode = currNode
        currNode = nextNode

    return prevNode
```

구조는 C 버전과 완전히 동일합니다 — `nextNode`를 먼저 저장하고, `currNode.next`를 뒤로 돌리고, 두 포인터를 한 칸씩 밉니다.
차이는 `free`가 없다는 것뿐입니다: 파이썬은 더 이상 참조되지 않는 노드를 가비지 컬렉터가 알아서 정리합니다.

```python
def from_iterable(values: Iterable[int]) -> Node | None:
    head = None
    tail = None
    for value in values:
        new_node = Node(value)
        if head is None:
            head = new_node
        else:
            tail.next = new_node
        tail = new_node
    return head

def to_list(head: Node | None) -> list[int]:
    result = []
    node = head
    while node is not None:
        result.append(node.value)
        node = node.next
    return result
```

`from_iterable`은 C의 `list_push_back`을 반복 호출하는 것과 같은 모양입니다 (tail을 들고 있어서 매번 끝까지 순회하지 않음).
`to_list`는 재귀로도 짤 수 있지만, 문제 조건대로 반복문을 쓰면 입력이 10만 개여도 `RecursionError` 걱정이 없습니다.

<details>
<summary>생각해 볼 것 — to_list를 재귀로 짜면 몇 개짜리 입력에서 터질까요?</summary>

파이썬의 기본 재귀 한도는 약 1000입니다. 재귀로 `to_list`를 짜면 리스트 길이가 그 근처만 되어도 `RecursionError`가 납니다. 그래서 이 문제가 "재귀 대신 반복문"을 조건으로 못박은 것입니다.
</details>

---

## Python · 사이클 찾기 — 플로이드의 토끼와 거북이

마지막 노드가 앞쪽 노드를 다시 가리키면 사이클이 생깁니다. `set`으로 방문 기록을 남기면 쉽지만
추가 메모리가 O(n)이라 금지 — 대신 느린 포인터와 빠른 포인터만으로 O(1) 메모리에 풉니다.

```
1 → 2 → 3 → 4
        ^       |
        +-------+
```

slow는 한 칸씩, fast는 두 칸씩 갑니다. 사이클이 없으면 fast가 먼저 NULL에 닿아 끝납니다.
사이클이 있으면 둘 다 결국 고리 안에 갇히고, fast가 매 스텝 slow와의 거리를 1씩 좁히므로
언젠가 정확히 같은 노드에서 만납니다.

### 1단계 — 만나는 지점 찾기

```python
slow = head
fast = head

found = False
while fast.next is not None and fast.next.next is not None:
    slow = slow.next
    fast = fast.next.next
    if slow is fast:
        found = True
        break

if not found:
    return None
```

`fast.next`와 `fast.next.next`를 둘 다 확인하는 이유는, fast가 두 칸을 가야 하니 그 두 칸이 모두 존재하는지 먼저 봐야 하기 때문입니다. 둘 중 하나라도 `None`이면 사이클 없이 끝난 것입니다.

### 2단계 — 시작 노드까지 찾기 (수학이 필요한 부분)

```python
pt1 = head
pt2 = slow      # 1단계에서 만난 지점
while pt1 is not pt2:
    pt1 = pt1.next
    pt2 = pt2.next

return pt1
```

head에서 사이클 시작까지 거리를 **a**, 시작에서 만난 지점까지를 **b**, 사이클 전체 길이를 **L**이라 하면:

- slow가 이동한 거리: `a + b`
- fast는 slow의 두 배를 이동했고, 사이클을 정수 번 더 돈 상태: `2(a+b) = a + b + kL`
- 정리하면 `a = kL - b`, 즉 `a`는 "사이클을 몇 바퀴 돈 길이에서 b를 뺀 것"과 같습니다.
- 그래서 한 포인터를 head로 되돌리고 둘 다 한 칸씩 걸으면, **정확히 a칸 뒤에** 사이클 시작점에서 다시 만납니다.

### cycle_length — detect_cycle을 다시 안 부르고 재사용하기

```python
def cycle_length(head: Node | None) -> int:
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
```

시작 노드를 한 번 찾으면, 그 노드부터 다시 자기 자신으로 돌아올 때까지 한 칸씩 세기만 하면 됩니다.
내부에서 `detect_cycle`을 한 번만 호출하고 fast/slow 탐색을 다시 하지 않습니다.

<details>
<summary>생각해 볼 것 — 노드 하나가 자기 자신을 가리키면 어떻게 되나요?</summary>

그 노드가 곧 사이클 시작점이고 길이는 1입니다. slow와 fast 둘 다 그 노드에 도달하는 순간 `fast.next.next`가 다시 자기 자신이 되어 바로 `slow is fast`가 성립합니다. 2단계에서도 `a`가 그대로 그 노드를 가리키므로 잘 동작합니다.
</details>

<details>
<summary>생각해 볼 것 — 사이클이 리스트 전체를 감싸면 (head가 곧 시작점)?</summary>

a = 0인 경우입니다. 2단계에서 pt1과 pt2가 이미 처음부터 같은 지점(만난 지점 = head 자체)일 수도 있고, 아니라면 정확히 사이클을 한 바퀴 돌아 다시 head에서 만납니다. 공식 `a = kL - b`가 a=0일 때도 그대로 성립하기 때문입니다.
</details>

---

*Week 01 · linked_list — 다시 볼 때는 각 섹션의 "생각해 볼 것"부터 스스로 답해보고, 막히면 그 위의 설명을 순서대로 따라가 보세요.*
