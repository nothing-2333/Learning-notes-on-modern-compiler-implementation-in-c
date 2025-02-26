/* This file is intentionally empty.  You should fill it in with your
   solution to the programming exercise. */
#include <string.h>
#include <stdio.h>

#include "util.h"
#include "prog1.h"
#include "slp.h"

typedef struct tree *T_tree;
struct tree { T_tree left; string key; T_tree right; };

T_tree Tree(T_tree l, string k, T_tree r)
{
		T_tree t = checked_malloc(sizeof(*t));
		t->left = l; t->key = k; t->right = r;
		return t;
}

T_tree insert(string key, T_tree t)
{
	if (t == NULL) return Tree(NULL, key, NULL);
	else if (strcmp(key, t->key) < 0) return Tree(insert(key, t->left), t->key, t->right);
	else if (strcmp(key, t->key) > 0) return Tree(t->left, t->key, insert(key, t->right));
	else return Tree(t->left, t->key, t->right);
}

// 打印树的函数（中序遍历）
void printTree(T_tree t) {
    if (t == NULL) return;
    printTree(t->left);
    printf("%s\n", t->key);
    printTree(t->right);
}

int maxargs(A_stm stm)
{
    switch (stm->kind)
    {
    case A_printStm:
    {
        A_expList exp_list = stm->u.print.exps;
        int count = 0;
        while (exp_list->kind == A_pairExpList)
        {
            exp_list = exp_list->u.pair.tail;
            count++;
        }
        count++;

        return count;
    }
    case A_compoundStm:
    {
        int count1 = maxargs(stm->u.compound.stm1);
        int count2 = maxargs(stm->u.compound.stm2);
        printf("%d %d\n", count1, count2);
        return count1 > count2 ? count1 : count2;
    }
    
    default:
        return 0;
    }
}

int main()
{
    int args_count = maxargs(prog());
    printf("print 语句参数最多为：%d\n", args_count);

	return 0;
}