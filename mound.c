#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#define MAX_CAPACITY 4194303 // 2^22 - 1
#define THRESHOLD 10

// Efficient function to find powers of 2
int alog(int x)
{
    return 32 - __builtin_clz(x) - 1;
}

// List's node structure definition and pointer declaration
typedef struct LNode *LNODE;
struct LNode
{
    int value;
    LNODE next;
};

// Mound's node structure definition and pointer declaration
typedef struct MNode *MNODE;
struct MNode
{
    int c;
    bool dirty;
    LNODE list;
    MNODE left;
    MNODE right;
};

// Mounds' structure definition and pointer declaration
typedef struct mound *MOUND;
struct mound
{
    MNODE root;
    MNODE *indexes;
    unsigned int numberOfNodes;
};

struct MNode defaultMNode = {0, false, NULL, NULL, NULL};

//Function defined to return random integer between the range
int randiom(int lower, int upper)
{
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}


// bool isPowerOf2(int n)
// {
//     if (n <= 0)
//     {
//         return false;
//     }
//     return (n & (n - 1)) == 0;
// }

// int binary_search(int arr[], int left, int right, int x)
// {
//     while (left <= right)
//     {
//         int mid = left + (right - left) / 2;
//         if (arr[mid] == x)
//         {
//             return mid;
//         }
//         else if (arr[mid] < x)
//         {
//             left = mid + 1;
//         }
//         else
//         {
//             right = mid - 1;
//         }
//     }
//     return -1;
// }


// Returns the Mound Node's value
int getMNodeValue(MNODE n)
{
    if (n->c == 0)
        return INT_MAX;
    return n->list->value;
}

// Function to create a whole new Mound structure
MOUND createNewMound()
{
    MOUND m = (MOUND)malloc(sizeof(struct mound));
    m->root = NULL;
    m->numberOfNodes = 0;
    return m;
}

// Function to create a single new node in the Mound
MNODE createNewMNode()
{
    MNODE n = (MNODE)malloc(sizeof(struct MNode));
    n->c = 0;
    n->dirty = false;
    n->list = NULL;
    n->left = NULL;
    n->right = NULL;     //initialisation of MNode
    return n;
}


// Function to create a single new node in the List
LNODE createNewLNode(int val)
{
    LNODE l = (LNODE)malloc(sizeof(struct LNode));
    l->next = NULL;
    l->value = val;       //initialisation of LNode
    return l;
}

// Checks if Mound's node is dirty or not
bool checkDirty(MNODE n)
{
    return n->dirty;
}

// Function used to set Mound's node dirty if the child parent condition of mound is not satisfied
void setMNodeDirty(MNODE n, bool newDirty)
{
    n->dirty = newDirty;
}


// TO initialise mound data structure with memory allocation and tree formation using indices
void intialiseMound(MOUND m)
{
    m->indexes = (MNODE *)malloc(sizeof(MNODE) * MAX_CAPACITY);
    MNODE *indexes = m->indexes;
    for (int i = 0; i <= MAX_CAPACITY; i++)
    {
        indexes[i] = createNewMNode();   // initialising MNodes for every every node
    }
    m->root = indexes[0];
    for (int i = 1; i <= (MAX_CAPACITY - 1) / 2; i++)
    {
        indexes[i - 1]->left = indexes[(i * 2) - 1];     // to create relations of left
        indexes[i - 1]->right = indexes[(i * 2)];        //   and right child to parent node using indexes
    }
}


// Function used to swap parent node with left child using pointers swapping
void swapLeft(MNODE n)
{
    int leftc = n->left->c;
    LNODE temp = n->left->list;        // creating a temporary list node to exchange list pointers
    n->left->list = n->list;
    n->list = temp;
    n->left->c = n->c;
    n->c = leftc;
    setMNodeDirty(n, false);
    int val = getMNodeValue(n->left);
    int left = getMNodeValue(n->left->left);
    int right = getMNodeValue(n->left->right);
    if (val > left || val > right)
        setMNodeDirty(n->left, true);    // setting new parent node dirty if value greater than any child
}

// Same function for right child as was for left child above
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
        unsigned int big = x;
        int bigPower = alog(big);
        unsigned int small = 1;
        int smallPower = alog(small);
        child = indexes[x - 1];
        parent = indexes[(x / 2) - 1];
        do
        {
            // binary search of x;
            int power = alog(x);
            if (value <= getMNodeValue(child))
            {
                big = x;
                bigPower = alog(big);
                power = (smallPower + bigPower) / 2;
                int toMinus = bigPower - power;
                int divideBy = pow(2, toMinus);
                x /= divideBy;
            }
            else
            {
                small = x;
                smallPower = alog(small) + 1;
                power = (bigPower + smallPower) / 2;
                int toAdd = bigPower - power;
                int divideBy = pow(2, toAdd);
                x = big / divideBy;
                // if (isPowerOf2(x)) power
            }
            if (x == 0)
            {
                x = 1;
            }
            child = indexes[x - 1];
            if (x != 1)
                parent = indexes[(x / 2) - 1];
            else
            {
                parent = dummy;
            }
        } while (!((value <= getMNodeValue(child)) && ((value > getMNodeValue(parent)))) && x != 1 && bigPower > smallPower);
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

// Corrects the mound structure when disturbed
void moundify(MNODE n)
{
    if (!checkDirty(n))
        return;

    moundify(n->left);      // recursive call to check each and 
    moundify(n->right);     //    every node and swap it with correct positions

    (getMNodeValue(n->left) > getMNodeValue(n->right)) ? swapRight(n) : swapLeft(n);

    moundify(n->left);      // recursive call to check each and every node and swap it with correct positions
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
    // printMound(m);
    // printf("initialised\n");
    FILE *fp;
    int number;
    fp = fopen("data.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening the file.\n");
        return 1;
    }
    while (fscanf(fp, "%d", &number) != EOF)
    {
        // printf("%d\n", number);
        insert(m, number);
    }
    fclose(fp);
    // print2D(m);
    int t;
    do
    {
        t = extractMin(m);
    } while (t != -1);

    return 0;
}
