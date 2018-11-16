// 2014004739 신정식
#include <stdio.h>
#include <stdlib.h>

//typedef int int;
struct RedBlackNode;
typedef struct RedBlackNode *Position;
typedef struct RedBlackNode *RBTree;

RBTree Find(int X, RBTree T);
RBTree Insert(int X, int ival, RBTree T);

typedef enum Color {
    Red, Black
} Color;

struct RedBlackNode {
    int Key;
    int Value;
    RBTree Left;
    RBTree Right;
    Color Color;
};

Position NullNode = NULL; /* Needs initialization */
FILE * close;
int depth, dupli, pre_value, notfound;

/* END */

RBTree Find(int ikey, RBTree T) {
    if(ikey==NULL){printf("check input\n"); return T;}
    if (T == NullNode) {
        notfound = 1;
        return T;
    }
    if (T->Key > ikey) {
        depth++;
        return Find(ikey, T->Left);
    }
    else if (T->Key < ikey) {
        depth++;
        return Find(ikey, T->Right);
    }
    else
        return T;
}

void PrintTree(RBTree T, FILE * close) {
    if (T != NullNode) {
        fprintf(close,"(%d,%d) ",T->Key,T->Value);
        PrintTree(T->Left,close);
        PrintTree(T->Right,close);
    }
}

/* This function can be called only if K2 has a left child */
/* Perform a rotate between a node (K2) and its left child */

/* Update heights, then return new root */

Position SingleRotateWithLeft(Position K2) {
    Position K1;

    K1 = K2->Left;
    K2->Left = K1->Right;
    K1->Right = K2;

    return K1; /* New root */
}

/* This function can be called only if K1 has a right child */
/* Perform a rotate between a node (K1) and its right child */

/* Update heights, then return new root */

Position SingleRotateWithRight(Position K1) {
    Position K2;

    K2 = K1->Right;
    K1->Right = K2->Left;
    K2->Left = K1;

    return K2; /* New root */
}

/* Perform a rotation at node X */
/* (whose parent is passed as a parameter) */

/* The child is deduced by examining ikey */

Position Rotate(int ikey, Position Parent) {

    if (ikey < Parent->Key)
        return Parent->Left = ikey < Parent->Left->Key ?
                              SingleRotateWithLeft(Parent->Left) :
                              SingleRotateWithRight(Parent->Left);
    else
        return Parent->Right = ikey < Parent->Right->Key ?
                               SingleRotateWithLeft(Parent->Right) :
                               SingleRotateWithRight(Parent->Right);
}

Position X, P, GP, GGP;

RBTree Insert(int ikey, int ival, RBTree T) {
    X = P = GP = T;
    NullNode->Key = ikey;
    NullNode->Value = ival;
    if(NullNode->Key==NULL){printf("check input\n"); return T;}
    while (X->Key != ikey) /* Descend down the tree */ {
        GGP = GP;
        GP = P;
        P = X;
        if (ikey < X->Key)
            X = X->Left;
        else
            X = X->Right;
        if (X->Left->Color == Red && X->Right->Color == Red){
            // HandleReorient(ikey, T);
            X->Color = Red; /* Do the color flip */
            X->Left->Color = Black;
            X->Right->Color = Black;

            if (P->Color == Red) /* Have to rotate */ {
                GP->Color = Red;
                if ((ikey < GP->Key) != (ikey < P->Key))
                    P = Rotate(ikey, GP); /* Start double rotate */
                X = Rotate(ikey, GGP);
                X->Color = Black;
            }
            T->Right->Color = Black; /* Make root black */
        }
    }

    if (X != NullNode) {
        dupli=1;
        pre_value = X->Value; // duplicate
    }

    X = malloc(sizeof ( struct RedBlackNode));
    if (X == NULL)
        printf("Out of space!!!");
    X->Key = ikey;
    X->Value = ival;
    X->Left = X->Right = NullNode;

    if (ikey < P->Key) /* Attach to its parent */
        P->Left = X;
    else
        P->Right = X;
    X->Color = Red; /* Do the color flip */
    X->Left->Color = Black;
    X->Right->Color = Black;

    if (P->Color == Red) /* Have to rotate */ {
        GP->Color = Red;
        if ((ikey < GP->Key) != (ikey < P->Key))
            P = Rotate(ikey, GP); /* Start double rotate */
        X = Rotate(ikey, GGP);
        X->Color = Black;
    }
    T->Right->Color = Black; /* Make root black */

    return T;
}


int main(int argc, char * argv[]) {
    RBTree T, F;

    if (NullNode == NULL) {
        NullNode = malloc(sizeof ( struct RedBlackNode));
        if (NullNode == NULL)
            printf("Out of space!!!");
        NullNode->Left = NullNode->Right = NullNode;
        NullNode->Color = Black;
    }

    /* Create the header node */
    T = malloc(sizeof(struct RedBlackNode));
    if (T == NULL)
        printf("Out of space!!!");
    T->Left = T->Right = NullNode;
    T->Color = Black;

    printf("Inserts are complete\n");
    //PrintTree(T->Right); // header skip

    char infi; // insert, find
    int key,val;
    FILE *open = fopen("input.txt","r");
    close = fopen("output.txt","w");
    if(open==NULL){puts("err : file read"); return 0;}

    while(fscanf(open,"%c",&infi) != EOF)
    {
        switch(infi) {
            case 'I':
                fscanf(open, "%d%d", &key, &val);
                T = Insert(key, val, T);
                if(dupli==1){
                    fprintf(close,"Found (%d,%d) update v=%d\n",key,pre_value,val);
                    dupli=0;
                }
                else fprintf(close,"Inserted (%d,%d)\n",key,val);
                break;
            case 'F':
                fscanf(open, "%d", &key);
                F = Find(key,T);
                if(notfound==1){
                    fprintf(close,"Not Found\n");
                    notfound=0;
                    depth=0;
                }
                else{
                    fprintf(close,"Found (%d,%d) on d=%d with c=",F->Key,F->Value,depth-1); // because header
                    if(F->Color==Red)fprintf(close,"red\n");
                    else fprintf(close,"black\n");
                    depth=0;
                }
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