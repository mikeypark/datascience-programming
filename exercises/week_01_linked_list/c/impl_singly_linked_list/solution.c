/* Problem statement: problem.md / function contract: list.h */

#include <stdlib.h>

#include "list.h"

/* Delete this define and every `NOT_IMPLEMENTED` below once you start writing.
 * The grader reads them as "not written yet"; anything else counts as a real answer.
 * list_create returning NULL carries the same meaning. */
#define NOT_IMPLEMENTED (-1000)

typedef struct Node {
    int value;
    struct Node *next;
} Node;

struct List {
    Node *head;
    Node *tail;
    int size;
    /* Feel free to add fields such as tail or size. */
};

List *list_create(void)
{
    List *list = malloc(sizeof(List));
    if (list == NULL) {
        return NULL; // malloc 실패
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;
}

void list_destroy(List *list)
{
    if (list == NULL) {
        return; // list_destroy(NULL) 은 아무 일도 하지 않고 그냥 넘어가야 합니다
    }

    Node *n = list->head;
    while (n != NULL) {
        Node *next = n->next;
        free(n);
        n = next;
    }

    free(list);
}

int list_size(const List *list)
{
    return list->size;
}

int list_push_front(List *list, int value)
{
    Node *newNode = malloc(sizeof(Node));
    if (newNode == NULL) {
        return -1;
    }
    newNode->value = value;
    newNode->next = list->head;

    list->head = newNode;
    if (list->size == 0) {
        list->tail = newNode; // 리스트가 비어있었다면 tail도 새 노드로
    }
    list->size++; // 리스트 사이즈 증가시킴.

    return 0;
}

int list_push_back(List *list, int value)
{
    Node *newNode = malloc(sizeof(Node));
    if (newNode == NULL) {
        return -1;
    }
    newNode->value = value;
    newNode->next = NULL;

    if (list->size == 0) {
        list->head = newNode;
    } else {
        list->tail->next = newNode;
    }
    list->tail = newNode;
    list->size++;

    return 0;
}

int list_insert(List *list, int index, int value)
{
    if (index < 0 || index > list->size) {
        return -1;   // 범위를 벗어나면 거절
    }

    Node *newNode = malloc(sizeof(Node));
    if (newNode == NULL) {
        return -1;
    }
    newNode->value = value;

    if (index == 0) {
        // 맨 앞에 넣는 경우: push_front와 똑같음
        newNode->next = list->head;
        list->head = newNode;
        if (list->size == 0) {
            list->tail = newNode;   // 원래 빈 리스트였다면 tail도 새 노드
        }
    } else {
        // index-1 번째 노드(prev)를 찾아서, prev와 prev->next 사이에 끼움
        Node *prev = list->head;
        for (int i = 0; i < index - 1; i++) {
            prev = prev->next;
        }
        newNode->next = prev->next;
        prev->next = newNode;
        if (newNode->next == NULL) {
            list->tail = newNode;   // 맨 뒤에 넣은 경우(index == size)라면 tail 갱신
        }
    }

    list->size++;
    return 0;
}

int list_remove(List *list, int index, int *out)
{
    if (index < 0 || index >= list->size) {
        return -1;   // remove는 index == size까지 허용 안 함 (insert와 다른 점!)
    }

    Node *target;

    if (index == 0) {
        // 맨 앞을 지우는 경우: head 자체를 바꿔야 함
        target = list->head;
        list->head = target->next;
        if (list->head == NULL) {
            list->tail = NULL;   // 이 노드가 유일한 노드였다면 리스트가 완전히 빈다
        }
    } else {
        // index-1 번째 노드(prev)를 찾아서, prev와 target을 건너뛰어 연결
        Node *prev = list->head;
        for (int i = 0; i < index - 1; i++) {
            prev = prev->next;
        }
        target = prev->next;
        prev->next = target->next;
        if (target->next == NULL) {
            list->tail = prev;   // 지운 노드가 tail이었다면 tail을 prev로 갱신
        }
    }

    if (out != NULL) {
        *out = target->value;   // out이 NULL이 아니면 지우기 전 값을 챙겨줌
    }
    free(target);
    list->size--;

    return 0;
}

int list_get(const List *list, int index, int *out)
{
    if (index < 0 || index >= list->size) {
        return -1;
    }

    Node *n = list->head;
    for (int i = 0; i < index; i++) {
        n = n->next;
    }

    *out = n->value;
    return 0;
}

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

int list_to_array(const List *list, int *out, int capacity)
{
    if (capacity < list->size) {
        return -1;   // 용량 부족: 아무것도 쓰지 않고 -1
    }

    int i = 0;
    Node *n = list->head;
    while (n != NULL) {
        out[i] = n->value;
        i++;
        n = n->next;
    }

    return i;   // == list->size
}
