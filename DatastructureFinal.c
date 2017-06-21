#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <stdlib.h>
enum { RED, BLACK, NONE };
typedef struct Node Node;
typedef Node* NodePtr;
struct Node
{
	int key;
	NodePtr parent;
	NodePtr left;
	NodePtr right;
	int color;
};

typedef struct RBT RBT;
typedef RBT* RBTPtr;
struct RBT
{
	NodePtr root;
};


struct inform
{
	int ins_count;
	int del_count;
	int mis_count;
	int nb;
	int bh;
}information;
typedef struct inform inform;
typedef inform* informPtr;

void inform_init(informPtr inform)
{
	inform->ins_count = 0;
	inform->del_count = 0;
	inform->mis_count = 0;
	inform->nb = 0;
	inform->bh = 1;
}


NodePtr Tnull_alloc()
{
	NodePtr Tnull = (NodePtr)malloc(sizeof(Node));
	Tnull->key = NULL;
	Tnull->parent = NULL;
	Tnull->left = NULL;
	Tnull->right = NULL;
	Tnull->color = BLACK;
	return Tnull;
}

NodePtr node_alloc(int k, NodePtr Tnull)
{
	NodePtr self = (NodePtr)malloc(sizeof(Node));
	self->key = k;
	self->parent = Tnull;
	self->left = Tnull;
	self->right = Tnull;
	self->color = NONE;
	return self;
}

RBTPtr rbt_alloc(NodePtr Tnull)
{
	RBTPtr self = (RBTPtr)malloc(sizeof(RBT));
	self->root = Tnull;
	return self;
}



Right_rotate(RBTPtr rbt, NodePtr tree, NodePtr x, NodePtr Tnull)
{
	NodePtr z = (NodePtr)malloc(sizeof(Node));
	z = x->left;
	x->left = z->right;
	if (z->right != Tnull)
		z->right->parent = x;
	z->parent = x->parent;
	if (x->parent == Tnull)
		rbt->root = z;
	else if (x == x->parent->left)
		x->parent->left = z;
	else
		x->parent->right = z;
	z->right = x;
	x->parent = z;
}

Left_rotate(RBTPtr rbt, NodePtr tree, NodePtr x, NodePtr Tnull)
{
	NodePtr z = (NodePtr)malloc(sizeof(Node));
	z = x->right;
	x->right = z->left;
	if (z->left != Tnull)
		z->left->parent = x;
	z->parent = x->parent;
	if (x->parent == Tnull)
		rbt->root = z;
	else if (x == x->parent->left)
		x->parent->left = z;
	else
		x->parent->right = z;
	z->left = x;
	x->parent = z;
}

void rbt_insert_fixup(RBTPtr rbt, NodePtr tree, NodePtr n, NodePtr Tnull)
{
	NodePtr y = (NodePtr)malloc(sizeof(Node));
	while (n->parent->color == RED)
	{
		if (n->parent == n->parent->parent->left)
		{
			y = n->parent->parent->right;
			if (y->color == RED)
			{
				n->parent->color = BLACK;
				y->color = BLACK;
				n->parent->parent->color = RED;
				n = n->parent->parent;
			}
			else if (n == n->parent->left)
			{
				n->parent->color = BLACK;
				n->parent->parent->color = RED;
				Right_rotate(rbt, rbt->root, n->parent->parent, Tnull);
			}
			else if (n == n->parent->right)
			{
				n = n->parent;
				Left_rotate(rbt, rbt->root, n, Tnull);
				n->parent->color = BLACK;
				n->parent->parent->color = RED;
				Right_rotate(rbt, rbt->root, n->parent->parent, Tnull);
			}
		}
		else
		{
			y = n->parent->parent->left;
			if (y->color == RED)
			{
				n->parent->color = BLACK;
				y->color = BLACK;
				n->parent->parent->color = RED;
				n = n->parent->parent;
			}
			else if (n == n->parent->right)
			{
				n->parent->color = BLACK;
				n->parent->parent->color = RED;
				Left_rotate(rbt, rbt->root, n->parent->parent, Tnull);
			}
			else if (n == n->parent->left)
			{
				n = n->parent;
				Right_rotate(rbt, rbt->root, n, Tnull);
				n->parent->color = BLACK;
				n->parent->parent->color = RED;
				Left_rotate(rbt, rbt->root, n->parent->parent, Tnull);
			}
		}
	}
	rbt->root->color = BLACK;
}

void rbt_insert(RBTPtr rbt, NodePtr tree, NodePtr n, NodePtr Tnull)
{
	NodePtr z = Tnull;
	NodePtr x = (NodePtr)malloc(sizeof(Node));
	x = rbt->root;
	while (x != Tnull)
	{
		z = x;
		if (n->key < x->key)
			x = x->left;
		else
			x = x->right;
	}
	n->parent = z;
	if (z == Tnull)
		rbt->root = n;
	else if (n->key < z->key)
		z->left = n;
	else
		z->right = n;
	n->right = Tnull;
	n->left = Tnull;
	n->color = RED;
	rbt_insert_fixup(rbt, tree, n, Tnull);
}

void rbt_print(RBTPtr rbt, NodePtr tree, NodePtr Tnull, int level)
{
	if (tree->right != Tnull)
		rbt_print(rbt, tree->right, Tnull, level + 1);
	for (int i = 0; i < level; i++)
	{
		printf("	");
	}
	if (tree->color == RED)
		printf("%d(red)\n", tree->key);
	else
		printf("%d(black)\n", tree->key);
	if (tree->left != Tnull)
		rbt_print(rbt, tree->left, Tnull, level + 1);

	printf("\n");
}

void rb_transplant(RBTPtr rbt, NodePtr tree, NodePtr u, NodePtr v, NodePtr Tnull)
{
	if (u->parent == Tnull)
		rbt->root = v;
	else if (u == u->parent->left)
		u->parent->left = v;
	else
		u->parent->right = v;
	v->parent = u->parent;
}

NodePtr rb_search(RBTPtr rbt, NodePtr tree, int k, NodePtr Tnull)
{
	NodePtr x = tree;
	while (x->key != k && x != Tnull)
	{
		if (k < x->key)
			x = x->left;
		else
			x = x->right;
	}
	return x;
}

NodePtr rb_maximum(NodePtr z, NodePtr Tnull)
{
	NodePtr x = z;
	while (x->right != Tnull)
		x = x->right;
	return x;
}

NodePtr rb_minimum(NodePtr z, NodePtr Tnull)
{
	NodePtr x = z;
	while (x->left != Tnull)
		x = x->left;
	return x;
}


void rb_delete_fixup(RBTPtr rbt, NodePtr tree, NodePtr x, NodePtr Tnull)
{
	NodePtr w;
	while (x != rbt->root && x->color == BLACK)
	{
		if (x == x->parent->left)
		{
			w = x->parent->right;
			if (w->color == RED)
			{
				w->color = BLACK;
				x->parent->color = RED;
				Left_rotate(rbt, tree, x->parent, Tnull);
				w = x->parent->right;
			}
			if (w->left->color == BLACK && w->right->color == BLACK)
			{
				w->color = RED;
				x = x->parent;
			}
			else if (w->left->color == BLACK)
			{
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->right->color = BLACK;
				Left_rotate(rbt, tree, x->parent, Tnull);
				x = rbt->root;
			}
			else
			{

				w->left->color = BLACK;
				w->color = RED;
				Right_rotate(rbt, tree, w, Tnull);
				w = x->parent->right;
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->right->color = BLACK;
				Left_rotate(rbt, tree, x->parent, Tnull);
				x = rbt->root;
			}
		}
		else
		{
			w = x->parent->left;
			if (w->color == RED)
			{
				w->color = BLACK;
				x->parent->color = RED;
				Right_rotate(rbt, tree, x->parent, Tnull);
				w = x->parent->left;
			}
			if (w->right->color == BLACK && w->left->color == BLACK)
			{
				w->color = RED;
				x = x->parent;
			}
			else if (w->right->color == BLACK)
			{
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->left->color = BLACK;
				Right_rotate(rbt, tree, x->parent, Tnull);
				x = rbt->root;
			}
			else
			{

				w->right->color = BLACK;
				w->color = RED;
				Left_rotate(rbt, tree, w, Tnull);
				w = x->parent->left;
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->left->color = BLACK;
				Right_rotate(rbt, tree, x->parent, Tnull);
				x = rbt->root;
			}
		}
	}
	x->color = BLACK;
}


void rb_delete(RBTPtr rbt, NodePtr tree, int k, NodePtr Tnull)
{
	NodePtr z = rb_search(rbt, tree, k, Tnull);
	NodePtr y;
	y = z;
	int y_original_color = y->color;
	NodePtr x;
	if (z->left == Tnull)
	{
		x = z->right;
		rb_transplant(rbt, tree, z, z->right, Tnull);
	}
	else if (z->right == Tnull)
	{
		x = z->left;
		rb_transplant(rbt, tree, z, z->left, Tnull);
	}
	else
	{
		y = rb_minimum(z->right, Tnull);
		y_original_color = y->color;
		x = y->right;
		if (y->parent == z)
			x->parent = y;
		else
		{
			rb_transplant(rbt, tree, y, y->right, Tnull);
			y->right = z->right;
			y->right->parent = y;
		}
		rb_transplant(rbt, tree, z, y, Tnull);
		y->left = z->left;
		y->left->parent = y;
		y->color = z->color;
	}
	if (y_original_color == BLACK)
		rb_delete_fixup(rbt, tree, x, Tnull);
}


void rb_inorder_traversal(RBTPtr rbt, NodePtr tree, NodePtr Tnull)
{
	NodePtr x = tree;
	if (x == Tnull)
	{
		return;
	}
	else
	{
		rb_inorder_traversal(rbt, x->left, Tnull);
		if (x->color == RED)
			printf("%d R\n", x->key);
		else
			printf("%d B\n", x->key);
		rb_inorder_traversal(rbt, x->right, Tnull);
	}
}



void push(NodePtr *stk, int *top, NodePtr x)
{
	*top = *top + 1;
	stk[*top] = x;
}
NodePtr pop(NodePtr *skt, int *top)
{
	NodePtr k;
	*top = *top - 1;
	k = skt[*top + 1];
	return k;
}
int empty(int *top)
{
	if (*top == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
#define STKSZ 200
int nb_count(RBTPtr rbt, NodePtr tree, NodePtr Tnull)
{
	int nb = 0;
	NodePtr x = tree;
	if (x == Tnull)
		return 0;
	NodePtr STK[STKSZ];
	int top = 0;
	while (1)
	{
		if (x != Tnull)
		{
			push(STK, &top, x);
			x = x->left;
		}
		else
		{
			if (empty(&top) == 1)
			{
				break;
			}
			x = pop(STK, &top);
			if (x->color == BLACK)
			{
				nb++;
			}
			x = x->right;
		}
	}
	return nb;
}

void Discriminate(RBTPtr rbt, NodePtr tree, informPtr information, int k, NodePtr Tnull)
{
	NodePtr x;
	x = tree;
	if (k > 0)
	{
		rbt_insert(rbt, tree, node_alloc(k, Tnull), Tnull);
		information->ins_count = information->ins_count + 1;
	}
	else if (k < 0)
	{
		k = -k;
		if (rb_search(rbt, tree, k, Tnull) != Tnull)
		{
			rb_delete(rbt, tree, k, Tnull);
			information->del_count = information->del_count + 1;
		}
		else
		{
			information->mis_count = information->mis_count + 1;
		}
	}
	else
	{
		//printf(" total = %d\n", information->ins_count - information->del_count);
		//printf(" insert = %d\n", information->ins_count);
		//printf(" deleted = %d\n", information->del_count);
		//printf(" miss = %d\n", information->mis_count);
		//printf(" nb = %d\n", nb_count(rbt, tree, Tnull));
		while (x != Tnull)
		{
			x = x->left;
			if (x->color == BLACK)
			{
				information->bh = information->bh + 1;
			}
		}
		//printf(" bh = %d\n", information->bh - 1);
		rb_inorder_traversal(rbt, tree, Tnull);



	}
}

char compare(char *str1, char *str2)
{
	while (*str1)
	{
		if (*str1 != *str2)
		{
			return 0;
		}
		str1++;
		str2++;
	}
	if (*str2 == '\0')
		return 1;
	return 0;
}
NodePtr rb_successor(RBTPtr rbt, NodePtr tree, NodePtr x, NodePtr Tnull)
{
	NodePtr y = x->parent;
	if (x->right != Tnull)
		return rb_minimum(x->right, Tnull);
	while (y != Tnull && x == y->right)
	{
		x = y;
		y = y->parent;
	}
	return y;
}

NodePtr rb_predecessor(RBTPtr rbt, NodePtr tree, NodePtr x, NodePtr Tnull)
{
	NodePtr y = x->parent;
	if (x->left != Tnull)
		return rb_maximum(x->left, Tnull);
	while (y != Tnull && x == y->left)
	{
		x = y;
		y = y->parent;
	}
	return y;
}

int main()
{
	WIN32_FIND_DATA ffd;
	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;

	FILE *fp;
	int data;
	NodePtr Tnull = Tnull_alloc();
	RBTPtr rbt = rbt_alloc(Tnull);
	informPtr information = (informPtr)malloc(sizeof(inform));
	inform_init(information);
	char filename[200];

	StringCchCopy(szDir, MAX_PATH, TEXT("./data/"));

	_tprintf(TEXT("\nTarget directory is %s\n\n"), szDir);

	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	hFind = FindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		printf("Error: FindFirstFile\n");
		return dwError;
	}


	FILE *fp2;
	fp2 = fopen("output.txt", "w");
	if (fp2 == NULL)
	{
		printf("Write Error!\n");
		return 0;
	}



	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
		}
		else
		{
			_tprintf(TEXT("  %s\n"), ffd.cFileName);

			int i = 0;
			while (ffd.cFileName[i] != '\0')
			{
				filename[i] = ffd.cFileName[i];
				i++;
			}
			filename[i] = '\0';
			//printf(" filename : %s\n", filename);
			char dir[200] = "./data/";
			strcat(dir, filename);

			fopen_s(&fp, dir, "r");

			char input_name[] = "./data/input.txt";
			char search_name[] = "./data/search.txt";


			if (compare(&dir, &input_name) == 1)
			{
				while (fscanf_s(fp, "%d", &data) != EOF)
				{
					Discriminate(rbt, rbt->root, information, data, Tnull);
				}
			}

			else if (compare(&dir, &search_name) == 1)
			{
				while (fscanf_s(fp, "%d", &data) != EOF)
				{


					if (data == 0)
						return 0;
					else if (rb_search(rbt, rbt->root, data, Tnull) != Tnull)
					{
						NodePtr x = rb_search(rbt, rbt->root, data, Tnull);
						if (rb_predecessor(rbt, rbt->root, x, Tnull) == Tnull)
						{
							printf(" NULL ");
							fputs(" NULL ", fp2);
						}

						else
						{
							printf(" %d ", rb_predecessor(rbt, rbt->root, x, Tnull)->key);
							fprintf(fp2, " %d ", rb_predecessor(rbt, rbt->root, x, Tnull)->key);
						}

						printf(" %d ", x->key);
						fprintf(fp2, " %d ", x->key);
						if (rb_successor(rbt, rbt->root, x, Tnull) == Tnull)
						{
							printf(" NULL \n");
							fputs(" NULL \n", fp2);
						}
						else
						{
							printf(" %d \n", rb_successor(rbt, rbt->root, x, Tnull)->key);
							fprintf(fp2, " %d \n", rb_successor(rbt, rbt->root, x, Tnull)->key);
						}


					}
					else
					{
						rbt_insert(rbt, rbt->root, node_alloc(data, Tnull), Tnull);
						NodePtr x = rb_search(rbt, rbt->root, data, Tnull);
						if (rb_predecessor(rbt, rbt->root, x, Tnull) == Tnull)
						{
							printf(" NULL ");
							fputs(" NULL ", fp2);
						}
						else
						{
							printf(" %d ", rb_predecessor(rbt, rbt->root, x, Tnull)->key);
							fprintf(fp2, " %d ", rb_predecessor(rbt, rbt->root, x, Tnull)->key);
						}
						printf(" NULL ");
						fputs(" NULL ", fp2);
						if (rb_successor(rbt, rbt->root, x, Tnull) == Tnull)
						{
							printf(" NULL \n");
							fputs(" NULL \n", fp2);
						}
						else
						{
							printf(" %d \n", rb_successor(rbt, rbt->root, x, Tnull)->key);
							fprintf(fp2, " %d \n", rb_successor(rbt, rbt->root, x, Tnull)->key);
						}

						rb_delete(rbt, rbt->root, data, Tnull);
					}


				}
				fclose(fp2);
			}

			//rbt_print(rbt, rbt->root, Tnull, 0);
			inform_init(information);
			//rbt = rbt_alloc(Tnull);
			printf("\n##############################\n");

			fclose(fp);

		}
	} while (FindNextFile(hFind, &ffd) != 0);



	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES)
	{
		printf("Error: FindFirstFile\n");
	}

	FindClose(hFind);
	return dwError;
}



