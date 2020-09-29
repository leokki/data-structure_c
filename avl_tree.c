#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int val;
    struct node *left;
    struct node *right;
    struct node *parent;
    int height;//avl树中结点的高度
    int bf;//avl树结点的平衡因子
} Avlnode, *Avltree;
static int Max(int a, int b) {return a>b?a:b;}
//计算avl树中该结点的高度
static int GetHeight(Avlnode *p) {
    if (p == NULL) {
        return 0;
    }
    if (p->left != NULL && p->right != NULL) {
        return Max(p->left->height, p->right->height)+1;
    }
    else if (p->left == NULL && p->right == NULL) {
        return 1;
    }
    else if (p->left == NULL) {
        return p->right->height+1;
    }
    else {
        return p->left->height+1;
    }
}
//对以root结点为根的avl树的结点a进行一次左旋,返回旋转子树的根结点
static Avlnode *LeftRotate(Avltree *root, Avlnode *a) {
    Avlnode *b = a->right;
    Avlnode *fa = a->parent;
    a->right = b->left;
    if (b->left != NULL) {
        b->left->parent = a;
    }
    b->left = a;
    a->parent = b;
    if (fa == NULL) {
        *root = b;
    }
    else if (a == fa->left) {
        fa->left = b;
    }
    else {
        fa->right = b;
    }
    b->parent = fa;
    Avlnode *p = a;
    while (p != NULL) {
        p->height = GetHeight(p);
        p = p->parent;
        //p->bf = GetHeight(p->left)-GetHeight(p->right);
    }
    return b;
}
//对以root结点为根的avl树的结点a进行一次右旋,返回旋转子树的根结点
static Avlnode *RightRotate(Avltree *root, Avlnode *a) {
    Avlnode *b = a->left;
    Avlnode *fa = a->parent;
    a->left = b->right;
    if (b->right != NULL) {
        b->right->parent = a;
    }
    b->right = a;
    a->parent = b;
    if (fa == NULL) {
        *root = b;
    }
    else if (a == fa->left) {
        fa->left = b;
    }
    else {
        fa->right = b;
    }
    b->parent = fa;
    Avlnode *p = a;
    while (p != NULL) {
        p->height = GetHeight(p);
        p = p->parent;
        //p->bf = GetHeight(p->left)-GetHeight(p->right);
    }
    return b;
}
//********无论是哪种调整方式,旋转子树的高度都会减一********
/*
向根为 root 的平衡二叉树插入新元素 val
成功后返回新平衡二叉树根结点,失败返回NULL
*/
Avltree AvlInsert(Avltree *root, int val) {
    Avlnode *s = (Avlnode*)malloc(sizeof(Avlnode));
    if (s == NULL) {
        return *root;
    }
    s->val = val;
    s->left = s->right = s->parent = NULL;
    s->height = 1;
    //s->bf = 0;

    if (*root == NULL) {
        *root = s;
        return *root;
    }
    //查找s的插入位置fp
    //记录距s插入位置最近且平衡因子不为零的结点
    Avlnode *A = *root;//可能的失衡结点
    Avlnode *p = *root;//用于查找插入位置的指针
    Avlnode *FA = NULL;//结点A的父亲结点
    Avlnode *fp = NULL;//结点p的父亲结点(指向p指针前面一个结点)
    while (p != NULL) {
        if (/*p->bf != 0*/GetHeight(p->left) != GetHeight(p->right)) {
            A = p; FA = fp;
        }
        fp = p;
        if (val < p->val) {
            p = p->left;
        }
        else if (val > p->val) {
            p = p->right;
        }
        else {
            free(s);
            return NULL;
        }
    }
    //插入s
    if (val < fp->val) {
        fp->left = s;
    }
    else {
        fp->right = s;
    }
    s->parent = fp;
    //确定结点B,并修改A的平衡因子
    Avlnode *B = NULL;
    if (val < A->val) {
        B = A->left;
        //A->bf++;
    }
    else {
        B = A->right;
        //A->bf++;
    }
    //修改B到s路径上各结点的平衡因子(初值均为0)
    /*
    p = B;
    while (p != s) {
        if (val < p->val) {
            p->bf++;
            p = p->left;
        }
        else {
            p->bf--;
            p = p->right;
        }
    }
    */
    //修改fp到root路径上各结点的高度
    p = fp;
    while (p != NULL) {
        p->height = GetHeight(p);
        p = p->parent;
    }
    //判断失衡类型并分类处理
    if (/*A->bf == 2 && B->bf == 1*/ GetHeight(A->left)-GetHeight(A->right)==2&&GetHeight(B->left)-GetHeight(B->right)==1) {
        RightRotate(root, A);
    }
    else if (GetHeight(A->left)-GetHeight(A->right)==2&&GetHeight(B->left)-GetHeight(B->right)==-1) {
        LeftRotate(root, B);
        RightRotate(root, A);
    }
    else if (GetHeight(A->left)-GetHeight(A->right)==-2&&GetHeight(B->left)-GetHeight(B->right)==1) {
        RightRotate(root, B);
        LeftRotate(root, A);
    }
    else if (GetHeight(A->left)-GetHeight(A->right)==-2&&GetHeight(B->left)-GetHeight(B->right)==-1) {
        LeftRotate(root, A);
    }
    return *root;
}
/*
在根为 root 的平衡二叉树中删除元素 val
成功后返回新平衡二叉树根节点,失败返回NULL
*/
Avltree AvlDelete(Avltree *root, int val) {
    if (*root == NULL) {
        return NULL;
    }
    Avlnode *p = *root;//用于查找删除位置的指针
    Avlnode *s = NULL;
    Avlnode *fs = NULL;//被删除结点的父结点指针
    while (p != NULL) {
        if (val < p->val) {
            p = p->left;
        }
        else if (val > p->val) {
            p = p->right;
        }
        else {
            break;
        }
    }
    if (p == NULL) {
        return NULL;
    }
    if (p == *root) {
        free(p);
        *root = NULL;
        return *root;
    }
    /*****
     * 分三种情况讨论
     * 该结点无孩子，则直接删除该结点
     * 该结点有且只有一个孩子，则连接之后再删除结点
     * 该结点既有左孩子又有右孩子，则再分为以下几种情况
     *    该结点左子树高度大于右子树，让左子树的最右结点代替当前结点
     *    该结点右子树高度大于左子树，让右子树的最右结点代替当前结点
     * 最后在层层回溯，逐层调整
    */
    Avlnode *fp = p->parent;
    fs = fp;
    if (p->left == NULL && p->right == NULL) {
        if (p == fp->left) {
            fp->left = NULL;
        }
        else {
            fp->right = NULL;
        }
        free(p);
    }
    else if (p->left == NULL) {
        if (p == fp->left) {
            fp->left = p->right;
        }
        else {
            fp->right = p->right;
        }
        p->right->parent = fp;
        free(p);
    }
    else if (p->right == NULL) {
        if (p == fp->left) {
            fp->left = p->left;
        }
        else {
            fp->right = p->left;
        }
        p->left->parent = fp;
        free(p);
    }
    else {
        if (GetHeight(p->left) > GetHeight(p->right)) {
            fs = p;
            s = p->left;
            while (s->right != NULL) {
                fs = s;
                s = s->right;
            }
            p->val = s->val;
            if (s == fs->left) {
                fs->left = s->left;
            }
            else {
                fs->right = s->left;
            }
            if (s->left != NULL) {
                s->left->parent = fs;
            }
            free(s);
        }
        else {
            fs = p;
            s = p->right;
            while (s->left != NULL) {
                fs = s;
                s = s->left;
            }
            p->val = s->val;
            if (s == fs->left) {
                fs->left = s->right;
            }
            else {
                fs->right = s->right;
            }
            if (s->right != NULL) {
                s->right->parent = fs;
            }
            free(s);
        }
    }
    //此时fs为被删除结点的父结点
    //层层回溯修改结点高度
    s = fs;
    while (s != NULL) {
        s->height = GetHeight(s);
        s = s->parent;
    }
    //层层回溯调整平衡
    s = fs;
    while (s != NULL) {
        int s_height = s->height;
        if (GetHeight(s->left) - GetHeight(s->right) == 2) {
            p = s->left;
            if (GetHeight(p->left) >= GetHeight(p->right)) {
                s = RightRotate(root, s);
            }
            else {
                LeftRotate(root, p);
                s = RightRotate(root, s);
            }
        }
        else if (GetHeight(s->left) - GetHeight(s->right) == -2) {
            p = s->right;
            if (GetHeight(p->right) >= GetHeight(p->left)) {
                s = LeftRotate(root, s);
            }
            else {
                RightRotate(root, p);
                s = LeftRotate(root, s);
            }
        }
        /*
        必须逐层循环直至根结点的原因:(纯数字意为高为x的任意平衡子树)
        node 20 ,height 5
          node 10 ,height 4
            node 5 ,height 3
              node 2 ,height 1
              node 7 ,height 2
                node 8 ,height 1
            node 15 ,height 3
              2
              3
          node 30 ,height 4
            4
            5
        上面这个平衡树删去结点8,每一层都需要调整

        node 20 ,height 6
          node 10 ,height 4
            node 5 ,height 3
              node 3 ,height 2
                node 2 ,height 1
              node 7 ,height 1
            2
          5
        上面这个平衡树删除结点7,调整一层后有一层不调整再后一层要调整
        */
        s = s->parent;
    }
    return *root;
}
/*
在根为 root 的平衡二叉树中查找元素 val
成功返回寻找的二叉树结点,失败返回 NULL
*/
Avlnode *AvlSearch(Avltree root, int val) {
    if (root == NULL) {
        return NULL;
    }
    else if (val < root->val) {
        return AvlSearch(root->left, val);
    }
    else if (val > root->val) {
        return AvlSearch(root->right, val);
    }
    else {
        return root;
    }
}
