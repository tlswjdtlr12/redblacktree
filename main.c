// 2014004739 신정식
#include <stdio.h>
#include <stdlib.h>

typedef int ElementType;
struct RedBlackNode;
typedef struct RedBlackNode *Position;
typedef struct RedBlackNode *RedBlackTree;

Position Find(ElementType X, RedBlackTree T);
RedBlackTree Insert(ElementType X, ElementType ival, RedBlackTree T);

typedef enum ColorType {
    Red, Black
} ColorType;

struct RedBlackNode {
    ElementType Element;
    ElementType Value;
    RedBlackTree Left;
    RedBlackTree Right;
    ColorType Color;
};

Position NullNode = NULL; /* Needs initialization */
FILE * close;

/* END */

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

static RedBlackTree
MakeEmptyRec(RedBlackTree T) {
    if (T != NullNode) {
        MakeEmptyRec(T->Left);
        MakeEmptyRec(T->Right);
        free(T);
    }
    return NullNode;
}

void
PrintTree(RedBlackTree T, FILE * close) {
    if (T != NullNode) {
        fprintf(close,"%d ",T->Element);
        PrintTree(T->Left,close);
        PrintTree(T->Right,close);
    }
}

/* This function can be called only if K2 has a left child */
/* Perform a rotate between a node (K2) and its left child */

/* Update heights, then return new root */

Position
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

Position
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

Position
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

Position X, P, GP, GGP;

RedBlackTree
Insert(ElementType Item, ElementType ival, RedBlackTree T) {
    X = P = GP = T;
    NullNode->Element = Item;
    if(NullNode->Element==NULL){printf("check input\n"); return T;}
    while (X->Element != Item) /* Descend down the tree */ {
        GGP = GP;
        GP = P;
        P = X;
        if (Item < X->Element)
            X = X->Left;
        else
            X = X->Right;
        if (X->Left->Color == Red && X->Right->Color == Red){
            // HandleReorient(Item, T);
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

    return T;
}


int main(int argc, char * argv[]) {
    RedBlackTree T;

    if (NullNode == NULL) {
        NullNode = malloc(sizeof ( struct RedBlackNode));
        if (NullNode == NULL)
            printf("Out of space!!!");
        NullNode->Left = NullNode->Right = NullNode;
        NullNode->Color = Black;
    }

    /* Create the header node */
    T = malloc(sizeof ( struct RedBlackNode));
    if (T == NULL)
        printf("Out of space!!!");
    //T->Element = -10;
    T->Left = T->Right = NullNode;
    T->Color = Black;

    T->Right = MakeEmptyRec(T->Right);

    printf("Inserts are complete\n");
    //PrintTree(T->Right); // header skip



    char infi; // insert, find
    ElementType key,val;
    FILE *open = fopen("input.txt","r");
    close = fopen("output.txt","w");
    if(open==NULL){puts("err : file read"); return 0;}

    while(fscanf(open,"%c",&infi) != EOF)
    {
        switch(infi) {
            case 'I':
                fscanf(open, "%d%d", &key, &val);
                T = Insert(key, val, T);
                //  printf("%lld", T->Right->Element);
//                if(dupli==1){
//                    fprintf(close,"Found (%lld,%lld) update v=%lld\n",key,pre_value,val);
//                    dupli=0;
//                }
//                else fprintf(close,"Inserted (%lld,%lld)\n",key,val);
                break;
            case 'F':
                fscanf(open, "%lld", &key);
//                F = Find(key,T);
//                if(notfound==1){
//                    fprintf(close,"Not Found\n");
//                    notfound=0;
//                    depth=0;
//                }
//                else{
//                    fprintf(close,"Found (%lld,%lld) on d=%d with h=%d\n",F->Key,F->Value,depth,getHeight(F));
//                    depth=0;
//
//                }
                break;
            case 'P':
                PrintTree(T->Right, close);
                fprintf(close, "\n");
            case 'Q':
                // end of file
                break;
        }
    }
    return 0;
}