/* This file is intentionally empty.  You should fill it in with your
   solution to the programming exercise. */
#include <string.h>
#include <stdio.h>

#include "util.h"

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

int main()
{
    T_tree root = NULL;
    root = insert("t", root);
    root = insert("s", root);
    root = insert("p", root);
    root = insert("i", root);  // 不会重复插入

    printTree(root);

	return 0;
}