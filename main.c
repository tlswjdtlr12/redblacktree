#include <stdio.h>
#include <stdlib.h>

typedef int ElementType;
struct RedBlackNode;
typedef struct RedBlackNode *Position;
typedef struct RedBlackNode *RedBlackTree;

RedBlackTree MakeEmpty(RedBlackTree T);
Position Find(ElementType X, RedBlackTree T);
RedBlackTree Initialize(void);
RedBlackTree Insert(ElementType X, RedBlackTree T);

void PrintTree(RedBlackTree T);

typedef enum ColorType {
    Red, Black
} ColorType;

struct RedBlackNode {
    ElementType Element;
    RedBlackTree Left;
    RedBlackTree Right;
    ColorType Color;
};

static Position NullNode = NULL; /* Needs initialization */

/* Initialization procedure */
RedBlackTree
Initialize(void) {
    RedBlackTree T;

    if (NullNode == NULL) {
        NullNode = malloc(sizeof ( struct RedBlackNode));
        if (NullNode == NULL)
            printf("Out of space!!!");
        NullNode->Left = NullNode->Right = NullNode;
        NullNode->Color = Black;
        NullNode->Element = 12345;
    }

    /* Create the header node */
    T = malloc(sizeof ( struct RedBlackNode));
    if (T == NULL)
        printf("Out of space!!!");
    //T->Element = -10;
    T->Left = T->Right = NullNode;
    T->Color = Black;

    return T;
}


static void
DoPrint(RedBlackTree T) {
    if (T != NullNode) {
        //Output(T->Element);
        printf("%d ",T->Element);
        DoPrint(T->Left);
        DoPrint(T->Right);
    }
}


/* END */

static RedBlackTree
MakeEmptyRec(RedBlackTree T) {
    if (T != NullNode) {
        MakeEmptyRec(T->Left);
        MakeEmptyRec(T->Right);
        free(T);
    }
    return NullNode;
}

RedBlackTree
MakeEmpty(RedBlackTree T) {
    T->Right = MakeEmptyRec(T->Right);
    return T;
}

Position
Find(ElementType X, RedBlackTree T) {
    if (T == NullNode)
        return NullNode;
    if (X < T->Element)
        return Find(X, T->Left);
    else
    if (X > T->Element)
        return Find(X, T->Right);
    else
        return T;
}


/* This function can be called only if K2 has a left child */
/* Perform a rotate between a node (K2) and its left child */

/* Update heights, then return new root */

static Position
SingleRotateWithLeft(Position K2) {
    Position K1;

    K1 = K2->Left;
    K2->Left = K1->Right;
    K1->Right = K2;

    return K1; /* New root */
}

/* This function can be called only if K1 has a right child */
/* Perform a rotate between a node (K1) and its right child */

/* Update heights, then return new root */

static Position
SingleRotateWithRight(Position K1) {
    Position K2;

    K2 = K1->Right;
    K1->Right = K2->Left;
    K2->Left = K1;

    return K2; /* New root */
}

/* Perform a rotation at node X */
/* (whose parent is passed as a parameter) */

/* The child is deduced by examining Item */

static Position
Rotate(ElementType Item, Position Parent) {

    if (Item < Parent->Element)
        return Parent->Left = Item < Parent->Left->Element ?
                              SingleRotateWithLeft(Parent->Left) :
                              SingleRotateWithRight(Parent->Left);
    else
        return Parent->Right = Item < Parent->Right->Element ?
                               SingleRotateWithLeft(Parent->Right) :
                               SingleRotateWithRight(Parent->Right);
}

static Position X, P, GP, GGP;

static
void HandleReorient(ElementType Item, RedBlackTree T) {
    X->Color = Red; /* Do the color flip */
    X->Left->Color = Black;
    X->Right->Color = Black;

    if (P->Color == Red) /* Have to rotate */ {
        GP->Color = Red;
        if ((Item < GP->Element) != (Item < P->Element))
            P = Rotate(Item, GP); /* Start double rotate */
        X = Rotate(Item, GGP);
        X->Color = Black;
    }
    T->Right->Color = Black; /* Make root black */
}

RedBlackTree
Insert(ElementType Item, RedBlackTree T) {
    X = P = GP = T;
    NullNode->Element = Item;
    while (X->Element != Item) /* Descend down the tree */ {
        GGP = GP;
        GP = P;
        P = X;
        if (Item < X->Element)
            X = X->Left;
        else
            X = X->Right;
        if (X->Left->Color == Red && X->Right->Color == Red)
            HandleReorient(Item, T);
    }

    if (X != NullNode)
        return NullNode; /* Duplicate */

    X = malloc(sizeof ( struct RedBlackNode));
    if (X == NULL)
        printf("Out of space!!!");
    X->Element = Item;
    X->Left = X->Right = NullNode;

    if (Item < P->Element) /* Attach to its parent */
        P->Left = X;
    else
        P->Right = X;
    HandleReorient(Item, T); /* Color it red; maybe rotate */

    return T;
}


main() {
    RedBlackTree T;

    T = Initialize();
    T = MakeEmpty(T);

    //for (i = 0; i < 800; i++, j = (j + 7) % 800)
    //  T = Insert(j, T);
    T = Insert(1, T);
    T = Insert(3, T);
    T = Insert(2, T);
    T = Insert(4, T);
    T = Insert(5, T);
    T = Insert(7, T);
    T = Insert(6, T);
    T = Insert(9, T);
    T = Insert(8, T);
    T = Insert(10, T);

    printf("Inserts are complete\n");

    DoPrint(T->Right); // header skip
    return 0;
}