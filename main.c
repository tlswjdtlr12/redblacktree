// 2014004739 신정식

#include <stdio.h>
#include <stdlib.h>

struct RedBlackNode;
typedef struct RedBlackNode *RBTree;

// P is parent, GP is Grand parent
RBTree Position, P, GP, TEMP;
RBTree Find(int ikey, RBTree T);
RBTree Insert(int ikey, int ival, RBTree T);
RBTree NN = NULL;
FILE * close;
int depth, dupli, pre_value, notfound;


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

RBTree Find(int ikey, RBTree T) {
    if(ikey==NULL){printf("check input\n"); return T;}
    if (T == NN) {
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
    if (T != NN) {
        fprintf(close,"(%d,%d) ",T->Key,T->Value);
        PrintTree(T->Left,close);
        PrintTree(T->Right,close);
    }
}

RBTree SingleLeft(RBTree K2) {
    RBTree K1;

    K1 = K2->Left;
    K2->Left = K1->Right;
    K1->Right = K2;

    return K1;
}

RBTree SingleRight(RBTree K1) {
    RBTree K2;

    K2 = K1->Right;
    K1->Right = K2->Left;
    K2->Left = K1;

    return K2;
}

RBTree DoubleRotate(int ikey, RBTree P) { // P is Parent

    if(ikey < P->Key){
        if(ikey < P->Left->Key) P->Left = SingleLeft(P->Left);
        else P->Left = SingleRight(P->Left);
        return P->Left;
    }
    else {
        if (ikey < P->Right->Key) P->Right = SingleLeft(P->Right);
        else P->Right = SingleRight(P->Right);
        return P->Right;
    }
}

RBTree Insert(int ikey, int ival, RBTree T) {
    Position = P = GP = T;
    NN->Key = ikey;
    NN->Value = ival;
    if(NN->Key==NULL){printf("check input\n"); return T;}
    while (Position->Key != ikey) {

        // descending
        TEMP = GP;
        GP = P;
        P = Position;

        // child's position
        Position = ikey < Position->Key ? Position->Left : Position->Right;

        // Both left and right are Red => color change
        if (Position->Left->Color == Red && Position->Right->Color == Red){
            Position->Color = Red;
            Position->Left->Color = Black;
            Position->Right->Color = Black;

            // after color change, check parent's color is red
            if (P->Color == Red){
                GP->Color = Red;
                if ((ikey < GP->Key) != (ikey < P->Key))
                    P = DoubleRotate(ikey, GP);
                Position = DoubleRotate(ikey, TEMP);
                Position->Color = Black;
            }
            // root color. T is header, T->Right is Root
            T->Right->Color = Black;
        }
    }

    // Until this line, value finding confirmation

    if (Position != NN) { // duplicate
        dupli=1;
        pre_value = Position->Value;
    }

    // Insert
    Position = malloc(sizeof ( struct RedBlackNode));
    if (Position == NULL)
        printf("Out of space!!!");
    Position->Key = ikey;
    Position->Value = ival;
    Position->Left = Position->Right = NN;

    // attach parent
    if (ikey < P->Key)
        P->Left = Position;
    else
        P->Right = Position;

    // color change
    Position->Color = Red;
    Position->Left->Color = Black;
    Position->Right->Color = Black;

    // after color change, check parent's color is red
    if (P->Color == Red){
        GP->Color = Red;
        if ((ikey < GP->Key) != (ikey < P->Key))
            P = DoubleRotate(ikey, GP);
        Position = DoubleRotate(ikey, TEMP);
        Position->Color = Black;
    }

    // root's color is black. T is header, T->Right is Root
    T->Right->Color = Black;

    return T;
}


int main(int argc, char * argv[]) {
    RBTree T, F;

    // NN is node with null
    if (NN == NULL) {
        NN = malloc(sizeof ( struct RedBlackNode));
        if (NN == NULL) printf("error");
        NN->Left = NN->Right = NN;
        NN->Color = Black;
    }

    // T is header node. T->Right is root
    T = malloc(sizeof(struct RedBlackNode));
    if (T == NULL) printf("error\n");
    T->Left = T->Right = NN;
    T->Color = Black;

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
                    fprintf(close,"Found (%d,%d) on d=%d with c=",F->Key,F->Value,depth-1); // depth-1 : because header
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