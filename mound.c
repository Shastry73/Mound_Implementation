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

void printLNodes(LNODE list)
{
    LNODE temp = list;
    while (temp != NULL)
    {
        printf("%d ", temp);
        temp = temp->next;
    }
    prnitf("\n");
}

void printInOrderMNode(MNODE n)
{
    if (n == NULL)
        return;
    printInOrderMNode(n->left);
    printLNodes(n);
    printInOrderMNode(n->right);
}

void printMound(MOUND m)
{
    printInOrderMNode(m->root);
}