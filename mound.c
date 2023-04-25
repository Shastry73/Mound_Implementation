#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct LNode *LNODE;
struct LNode
{
    int value;
    LNODE next;
};
typedef struct MNode *MNODE;
struct MNode
{
    int c;
    bool dirty;
    LNODE list;
    MNODE left;
    MNODE right;
};
typedef struct mound *MOUND;
struct mound
{
    MNODE root;
    int count;
};

// returns the MNode value
int getMNodeValue(MNODE n)
{
    if (n->c == 0)
        return __INT_MAX__;
    return n->list->value;
}

void insert(MOUND m, int value);
MOUND createNewMound()
{
    MOUND m = (MOUND)malloc(sizeof(struct mound));
    m->count = 0;
    m->root = NULL;
    return m;
}

MNODE createNewMNode()
{
    MNODE n = (MNODE)malloc(sizeof(struct MNode));
    n->c = 0;
    n->dirty = false;
    n->list = NULL;
    n->left = NULL;
    n->right = NULL;
    return n;
}

LNODE createNewLNode(int val)
{
    LNODE l = (LNODE)malloc(sizeof(struct LNode));
    l->next = NULL;
    l->value = val;
    return l;
}

bool checkDirty(MNODE n)
{
    return n->dirty;
}

void setMNodeDirty(MNODE n, bool newDirty)
{
    n->dirty = newDirty;
}

void swapLeft(MNODE n)
{
    LNODE temp = n->left->list;
    n->left->list = n->list;
    n->list = temp;
    setMNodeDirty(n, false);
    int val = getMNodeValue(n->left);
    int left = getMNodeValue(n->left->left);
    int right = getMNodeValue(n->left->right);
    if (val > left || val > right)
        setMNodeDirty(n->left, true);
}
void swapRight(MNODE n)
{
    LNODE temp = n->right->list;
    n->right->list = n->list;
    n->list = temp;
    setMNodeDirty(n, false);
    int val = getMNodeValue(n->right);
    int left = getMNodeValue(n->right->left);
    int right = getMNodeValue(n->right->right);
    if (val > left || val > right)
        setMNodeDirty(n->right, true);
}

void insert(MOUND m, int value);

void moundify(MNODE n)
{
    if (!checkDirty(n))
        return;

    moundify(n->left);
    moundify(n->right);

    (getMNodeValue(n->left) > getMNodeValue(n->right)) ? swapRight(n) : swapLeft(n);
    moundify(n->left);
    moundify(n->right);
}

int extractMin(MOUND m)
{
    int min = getMNodeValue(m->root); // first element of the root node is the minimum
    LNODE temp;
    temp = m->root->list;
    m->root->list = m->root->list->next;
    free(temp);
    setMNodeDirty(m->root, true);
    moundify(m);
    return min;
}
// LIST createNewList()
// {
//     LIST myList;
//     myList = (LIST)malloc(sizeof(struct linked_list));
//     myList->count = 0;
//     myList->head = NULL;
//     return myList;
// }
// NODE createNewNode(int value)
// {
//     NODE myNode;
//     myNode = (NODE)malloc(sizeof(struct node));
//     myNode->ele = value;
//     myNode->next = NULL;
//     return myNode;
// }
// void insertAfter(int searchEle, NODE n1, LIST l1)
// {
//     if (l1->head == NULL)
//     {
//         l1->head = n1;
//         n1->next = NULL;
//         l1->count++;
//     }
//     else
//     {
//         NODE temp = l1->head;
//         NODE prev = temp;
//         while (temp != NULL)
//         {
//             if (temp->ele == searchEle)
//                 break;
//             prev = temp;
//             temp = temp->next;
//         }
//         if (temp == NULL)
//         {
//             printf("Element not found\n");
//             return;
//         }
//         else
//         {
//             if (temp->next == NULL)
//             {
//                 temp->next = n1;
//                 n1->next = NULL;
//                 l1->count++;
//             }
//             else
//             {
//                 prev = temp;
//                 temp = temp->next;
//                 prev->next = n1;
//                 n1->next = temp;
//                 l1->count++;
//             }
//             return;
//         }
//     }
//     return;
// }
// void printList(LIST l1)
// {
//     NODE temp = l1->head;
//     printf("[HEAD] ->");
//     while (temp != NULL)
//     {
//         printf(" %d ->", temp->ele);
//         temp = temp->next;
//     }
//     printf(" [NULL]\n");
// }

// void deleteAt(LIST l1, int position)
// {
//     if (position < 1 || position > l1->count || l1->count == 0)
//         return;
//     else if (position == 1)
//     {
//         NODE temp = l1->head;
//         l1->head = temp->next;
//         free(temp);
//     }
//     else if (position == l1->count)
//     {
//         NODE prev = l1->head;
//         for (int i = 1; i < position - 1; i++)
//         {
//             prev = prev->next;
//         }
//         NODE temp = prev->next;
//         prev->next = NULL;
//         free(temp);
//     }
//     else
//     {
//         NODE prev = l1->head;
//         for (int i = 1; i < position - 1; i++)
//         {
//             prev = prev->next;
//         }
//         NODE temp = prev->next;
//         prev->next = temp->next;
//         free(temp);
//     }
//     l1->count -= 1;
// }
// void insertFirst(LIST l1, NODE node)
// {
//     node->next = l1->head;
//     l1->head = node;
//     l1->count += 1;
// }

// void deleteFirst(LIST l1)
// {
//     deleteAt(l1, 1);
// }

// int search(LIST l1, int ele)
// {
//     NODE temp = l1->head;
//     for (int i = 1; i = l1->count; i++)
//     {
//         if (temp->ele == ele)
//             return i;
//         temp = temp->next;
//     }
//     return -1;
// }

// void rotateLeft(LIST l1, int k)
// {
//     k = k % l1->count;
//     if (k == 0)
//         return;
//     NODE temp = l1->head;
//     for (int i = 0; i < l1->count - 1; i++)
//     {
//         temp = temp->next;
//     }
//     temp->next = l1->head;
//     NODE to = l1->head;
//     for (int i = 0; i < k - 1; i++)
//     {
//         to = to->next;
//     }
//     l1->head = to->next;
//     to->next = NULL;
// }

// void reverse(LIST l1)
// {
//     NODE prev = NULL;
//     NODE temp = l1->head->next;
//     // prev = l1->head;
//     // l1->head = l1->head->next;
//     // temp = l1->head->next;
//     // prev->next = NULL;
//     // for (int i = 1; i < l1->count; i++)
//     // {
//     //     l1->head->next = prev;
//     //     prev = l1->head;
//     //     l1->head = temp;
//     //     temp = temp->next;
//     // }
//     while (temp != NULL)
//     {
//         l1->head->next = prev;
//         prev = l1->head;
//         l1->head = temp;
//         temp = temp->next;
//     }
// }

// LIST number(char *number)
// {
//     LIST l1 = createNewList();
//     int length = strlen(number) - 1;
//     for (int i = 0; i < length; i++)
//     {
//         NODE n1 = createNewNode((int)number[i] - 48);
//         insertFirst(l1, n1);
//     }
//     return l1;
// }

// void insertLast(LIST l1, NODE node)
// {
//     NODE temp = l1->head;
//     if (l1->count == 0)
//     {
//         l1->head = node;
//         l1->count += 1;
//         return;
//     }
//     while (temp->next != NULL)
//         temp = temp->next;
//     temp->next = node;
//     node->next = NULL;
//     l1->count += 1;
// }

// LIST add(LIST l1, LIST l2)
// {
//     int carry = 0;
//     LIST l3 = createNewList();
//     NODE temp1 = l1->head, temp2 = l2->head;
//     while (temp1 == NULL && temp2 == NULL)
//     {
//         int sum = 0;
//         if (temp1 != NULL)
//             sum += temp1->ele;
//         if (temp2 != NULL)
//             sum += temp2->ele;
//         sum += carry;
//         carry = 0;
//         if (sum > 9)
//         {
//             sum -= 10;
//             carry = 1;
//         }
//         insertLast(l3, createNewNode(sum));
//         if (temp1 != NULL)
//             temp1 = temp1->next;
//         if (temp2 != NULL)
//             temp2 = temp2->next;
//     }
//     if (carry == 1)
//         insertLast(l3, createNewNode(1));

//     return l3;
// }