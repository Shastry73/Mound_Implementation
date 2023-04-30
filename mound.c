#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#define MAX_CAPACITY 127
#define THRESHOLD 1

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
    MNODE *indexes;
    unsigned int numberOfNodes;
};

struct MNode defaultMNode = {0, false, NULL, NULL, NULL};

int randiom(int lower, int upper)
{
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}
bool isPowerOf2(int n)
{
    if (n <= 0)
    {
        return false;
    }
    return (n & (n - 1)) == 0;
}

int binary_search(int arr[], int left, int right, int x)
{
    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        if (arr[mid] == x)
        {
            return mid;
        }
        else if (arr[mid] < x)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    return -1;
}

// returns the MNode value
int getMNodeValue(MNODE n)
{
    if (n->c == 0)
        return INT_MAX;
    return n->list->value;
}

MOUND createNewMound()
{
    MOUND m = (MOUND)malloc(sizeof(struct mound));
    m->root = NULL;
    m->numberOfNodes = 0;
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

void intialiseMound(MOUND m)
{
    m->indexes = (MNODE *)malloc(sizeof(MNODE) * MAX_CAPACITY);
    MNODE *indexes = m->indexes;
    for (int i = 0; i <= MAX_CAPACITY; i++)
    {
        indexes[i] = createNewMNode();
    }
    m->root = indexes[0];
    for (int i = 1; i <= (MAX_CAPACITY - 1) / 2; i++)
    {
        indexes[i - 1]->left = indexes[(i * 2) - 1];
        indexes[i - 1]->right = indexes[(i * 2)];
    }
}

void swapLeft(MNODE n)
{
    int leftc = n->left->c;
    LNODE temp = n->left->list;
    n->left->list = n->list;
    n->list = temp;
    n->left->c = n->c;
    n->c = leftc;
    setMNodeDirty(n, false);
    int val = getMNodeValue(n->left);
    int left = getMNodeValue(n->left->left);
    int right = getMNodeValue(n->left->right);
    if (val > left || val > right)
        setMNodeDirty(n->left, true);
}
void swapRight(MNODE n)
{
    int rightc = n->right->c;
    LNODE temp = n->right->list;
    n->right->list = n->list;
    n->list = temp;
    n->right->c = n->c;
    n->c = rightc;
    setMNodeDirty(n, false);
    int val = getMNodeValue(n->right);
    int left = getMNodeValue(n->right->left);
    int right = getMNodeValue(n->right->right);
    if (val > left || val > right)
        setMNodeDirty(n->right, true);
}

void insert(MOUND m, int value)
{
    MNODE dummy = createNewMNode();
    LNODE dummyData = createNewLNode(INT_MIN);
    dummy->list = dummyData;
    MNODE *indexes = m->indexes;
    MNODE child;
    MNODE parent;
    int i = 0;
    do
    {
        unsigned int x = randiom((MAX_CAPACITY + 1) / 2, MAX_CAPACITY); // select randomly between numberOfNodes and MAX_CAPACITY
        unsigned int intitial = x;
        int intialPower = (int)log2(intitial);
        child = indexes[x - 1];
        parent = indexes[(x / 2) - 1];
        do
        {
            // binary search of x;
            int power = (int)log2(x);
            if (value <= getMNodeValue(child))
            {
                power /= 2;
                if (power == 0)
                    power = 1;
                int divideBy = pow(2, power);
                x /= divideBy;
            }
            else
            {
                if (power / 2 == 0)
                    power = power + 1;
                else if (power % 2 == 0)
                {
                    power = power + ((power - 1) / 2);
                }
                else
                {
                    power = power + (power / 2);
                }
                // if (isPowerOf2(x)) power
                int powerMult = intialPower - power;
                int divideBy = pow(2, powerMult);
                x = intitial / divideBy;
            }
            child = indexes[x - 1];
            if (x != 1)
                parent = indexes[(x / 2) - 1];
            else
            {
                parent = dummy;
            }
        } while (!((value <= getMNodeValue(child)) && ((value > getMNodeValue(parent)))) && x != 1);
        i++;
    } while (getMNodeValue(child) == INT_MAX && i < THRESHOLD);
    LNODE temp = createNewLNode(value);
    if (child->list == NULL)
        m->numberOfNodes++;
    temp->next = child->list;
    child->list = temp;
    child->c += 1;
    free(dummy);
}

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

void printLNodes(LNODE list)
{
    LNODE temp = list;
    while (temp != NULL)
    {
        printf("%d ", temp->value);
        temp = temp->next;
    }
    printf("\n");
}

void printInOrderMNode(MNODE n)
{
    if (n->list == NULL)
        return;
    printInOrderMNode(n->left);
    printLNodes(n->list);
    printInOrderMNode(n->right);
}

void printMound(MOUND m)
{
    printInOrderMNode(m->root);
    printf("\n");
}

void print2DUtil(MNODE n, int space)
{
    int i;
    if (n->list == NULL)
        return;
    space += 5;
    print2DUtil(n->right, space);
    printf("\n");
    for (i = 5; i < space; i++)
        printf(" ");
    printf("%d\n", getMNodeValue(n));
    print2DUtil(n->left, space);
}

void print2D(MOUND m)
{
    print2DUtil(m->root, 0);
    printf("\n\n\n\n\n");
}

int extractMin(MOUND m)
{
    if (m->root->list == NULL)
    {
        printf("Mound empty\n");
        return -1;
    }
    int min = getMNodeValue(m->root); // first element of the root node is the minimum
    printf("Minimum element: %d\n\n", min);
    LNODE temp;
    temp = m->root->list;
    m->root->list = m->root->list->next;
    m->root->c -= 1;
    if (m->root->c == 0)
        m->numberOfNodes -= 1;
    free(temp);
    int val = getMNodeValue(m->root);
    int left = getMNodeValue(m->root->left);
    int right = getMNodeValue(m->root->right);
    if (val > left || val > right)
        setMNodeDirty(m->root, true);
    moundify(m->root);
    // printMound(m);
    // print2D(m);
    return min;
}

int main(int argc, char const *argv[])
{
    srand(time(0));
    MOUND m = createNewMound();
    intialiseMound(m);
    printMound(m);
        FILE *fp;
    int number;
    fp = fopen("data.txt", "r");
    if (fp == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }
    while (fscanf(fp, "%d", &number) != EOF) {
        insert(m, number);
    }
    fclose(fp);
    extractMin(m);  

    return 0;
}
