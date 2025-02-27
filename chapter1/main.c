/* This file is intentionally empty.  You should fill it in with your
   solution to the programming exercise. */
#include <string.h>
#include <stdio.h>

#include "util.h"
#include "prog1.h"
#include "slp.h"

// 求一个表达式中 print 语句最大参数 
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
        return count1 > count2 ? count1 : count2;
    }
    case A_assignStm:
    {
        if (A_eseqExp == stm->u.assign.exp->kind) 
        {
            return maxargs(stm->u.assign.exp->u.eseq.stm);
        }
        else return 0;
    }
    }
}

// 变量表
typedef struct table *Table_;
struct table { string key; int value; Table_ tail; };
Table_ Table(string key, int value, Table_ tail)
{
    Table_ t = malloc(sizeof(*t));
    t->key = key; t->value = value; t->tail = tail;
    return t;
}
// 链表查询
int lookup(Table_ t, string key)
{
    while (t)
    {
        if (strcmp(key, t->key) == 0) return t->value;

        t = t->tail;
    }
    // 如果未找到，直接报错并终止程序
    fprintf(stderr, "Error: Key '%s' not found in the table.\n", key);
    exit(1);
}
// 链表更新
Table_ update(Table_ t, string key, int value)
{
    if (t == NULL) return Table(key, value, NULL);

    Table_ head = t;
    Table_ prev = NULL;

    while (t)
    {
        if (strcmp(key, t->key) == 0)
        {
            t->value = value;
            return head;
        }

        prev = t;
        t = t->tail;
    }

    Table_ new_t = Table(key, value, NULL);
    prev->tail = new_t;
    return head;
}
// 计算 opExp
int computeOpExp(int left, int right, A_binop op_type)
{
    int result;
    switch (op_type)
    {
    case A_plus:
        result = left + right;
        break;
    case A_minus:
        result = left - right;
        break;
    case A_times:
        result = left * right;
        break;
    case A_div:
        result = left / right;
        break;
    }

    return result;
}

// 相互递归要提前声明一下
Table_ interpStm(A_stm s, Table_ t);
// 解释表达式，将表达式的结果，与变量表一起返回
struct IntAndTable { int i; Table_ t; };
struct IntAndTable interpExp(A_exp e, Table_ t)
{
    struct IntAndTable result;
    switch (e->kind)
    {
    case A_idExp:
    {
        result.i = lookup(t, e->u.id);
        result.t = t;
        break;
    }
    case A_numExp:
    {
        result.i = e->u.num;
        result.t = t;
        break;
    }
    case A_opExp:
    {
        struct IntAndTable left = interpExp(e->u.op.left, t);
        struct IntAndTable right = interpExp(e->u.op.right, left.t);

        result.i = computeOpExp(left.i, right.i, e->u.op.oper);
        result.t = right.t;
        break;
    }
    case A_eseqExp:
    {
        Table_ new_t = interpStm(e->u.eseq.stm, t);
        result = interpExp(e->u.eseq.exp, new_t);
        break;
    }
    }
    return result;
}
// 解释语句
Table_ interpStm(A_stm stm, Table_ t)
{
    switch (stm->kind)
    {
    case A_compoundStm:
    {
        Table_ t1 = interpStm(stm->u.compound.stm1, t);
        Table_ t2 = interpStm(stm->u.compound.stm2, t1);
        return t2;
    }
    case A_assignStm:
    {
        struct IntAndTable exp_result = interpExp(stm->u.assign.exp, t);
        string key = stm->u.assign.id;
        return update(exp_result.t, key, exp_result.i);
    }
    case A_printStm:
    {
        A_expList exp_list = stm->u.print.exps;
        while (exp_list->kind == A_pairExpList)
        {
            struct IntAndTable exp_result = interpExp(exp_list->u.pair.head, t);
            t = exp_result.t;
    
            exp_list = exp_list->u.pair.tail;
        }
        struct IntAndTable exp_result = interpExp(exp_list->u.last, t);
        printf("%d\n", exp_result.i);
        return exp_result.t;
    }
    }
}
// 打印链表
void printTable(Table_ t)
{
    while (t)
    {
        printf("{%s: %d}, ", t->key, t->value);
        t = t->tail;
    }
    printf("\n");
}

typedef struct tree *T_tree;
struct tree { T_tree left; string key; T_tree right; };

T_tree Tree(T_tree l, string k, T_tree r)
{
		T_tree t = checked_malloc(sizeof(*t));
		t->left = l; t->key = k; t->right = r;
		return t;
}
// 在树中插入
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
// 搜索树的查找
int member(string key, T_tree t)
{
    if (NULL == t) return FALSE;
	else if (strcmp(key, t->key) < 0) return member(key, t->left);
	else if (strcmp(key, t->key) > 0) return member(key, t->right);
	else return TRUE;
}

int main()
{
    // 程序设计 1
    int args_count = maxargs(prog());
    printf("print 语句参数最多为：%d\n", args_count);
    // 程序设计 2
    Table_ table = NULL;
    Table_ result = interpStm(prog(), table);
    printTable(result);

    // 习题 a
    T_tree tree = NULL;
    tree = insert("a", tree);
    tree = insert("b", tree);
    tree = insert("c", tree);
    tree = insert("d", tree);
    printf("%d\n", member("b", tree));
    printf("%d\n", member("e", tree));
    // 剩下习题不想写


	return 0;
}