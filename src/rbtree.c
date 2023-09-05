#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

void left_rotate(rbtree *t, node_t *x);
void right_rotate(rbtree *t, node_t *x);
void transplant(rbtree *t, node_t *from, node_t *to);
node_t *tree_minimum(rbtree *t, node_t *root);
void delete_fixup(rbtree *t, node_t *x);

rbtree *new_rbtree(void)
{
  // RB Tree 포인터
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  if (p == NULL)
  {
    return NULL;
  }

  // NIL 노드 포인터
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  if (nil == NULL)
  {
    free(p);
    return NULL;
  }

  // NIL 노드 값 초기화
  nil->color = RBTREE_BLACK;
  nil->key = 0;
  nil->parent = NULL;
  nil->left = NULL;
  nil->right = NULL;

  // RB Tree 필드 값 초기화
  p->root = nil;
  p->nil = nil;

  // rbtree 구조체를 가리키는 포인터 p를 반환
  return p;
}

void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  free(t);
}

// left-rotate
void left_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->right;
  x->right = y->left;
  if (y->left != t->nil)
  {
    y->left->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil)
  {
    t->root = y;
  }
  else if (x == x->parent->left)
  {
    x->parent->left = y;
  }
  else
  {
    x->parent->right = y;
  }

  y->left = x;
  x->parent = y;
}

// right-rotate
void right_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->left;
  x->left = y->right;
  if (y->right != t->nil)
  {
    y->right->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil)
  {
    t->root = y;
  }
  else if (x == x->parent->left)
  {
    x->parent->left = y;
  }
  else
  {
    x->parent->right = y;
  }

  y->right = x;
  x->parent = y;
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // 새로 추가할 노드 메모리 할당하기
  struct node_t *newNode = calloc(1, sizeof(struct node_t));

  // 메모리 할당에 실패한 경우 NULL 리턴
  if (newNode == NULL)
  {
    return NULL;
  }

  // 새로 추가할 노드 값 초기화
  newNode->color = RBTREE_RED;
  newNode->key = key;
  newNode->parent = t->nil;
  newNode->left = t->nil;
  newNode->right = t->nil;

  // 만약 트리가 비어있는 상태라면 루트 노드를 추가하고 리턴하기
  if (t->root == t->nil)
  {
    t->root = newNode;
    t->root->color = RBTREE_BLACK; // 루트노드는 검은색
    return newNode;
  }

  struct node_t *curr = t->root; // 새로 추가할 노드와 비교할 노드
  struct node_t *prev = NULL;    // 새로 추가할 노드의 부모가 될 노드

  // sentinel 노드에 이를 때까지 내려가기
  while (curr != t->nil)
  {
    prev = curr;
    if (key < curr->key)
    {                    // 넣으려는 값이 현재 노드의 값보다 작은 경우
      curr = curr->left; // 왼쪽 노드로 포커스 이동
    }
    else
    {                     // 넣으려는 값이 현재 노드의 값보다 크거나 같은 경우
      curr = curr->right; // 오른쪽 노드로 포커스 이동
    }
  }

  // 노드를 추가할 위치를 찾은 경우 새로 추가할 노드의 부모 노드 설정
  newNode->parent = prev;

  // 부모 노드의 왼쪽 자식 또는 오른쪽 자식으로 추가
  if (newNode->key < prev->key)
  {
    prev->left = newNode;
  }
  else
  {
    prev->right = newNode;
  }

  // 새로 추가하는 노드의 부모 노드의 색깔이 빨간색일 때까지 반복문 진행
  while (newNode != t->root && newNode->parent->color == RBTREE_RED)
  {
    if (newNode->parent == newNode->parent->parent->left)
    { // 부모 노드가 할아버지 노드의 왼쪽 자식인 경우
      node_t *uncle = newNode->parent->parent->right;

      // Case 1. 부모 노드와 삼촌 노드의 색깔이 빨간색인 경우
      if (uncle != NULL && uncle->color == RBTREE_RED)
      {
        newNode->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;

        newNode->parent->parent->color = RBTREE_RED;

        newNode = newNode->parent->parent; // 할아버지 노드로 포커싱 이동
      }

      else
      {
        // Case 2. 삼촌 노드는 검은색이고 새로운 노드가 오른쪽 자식으로 추가된 경우
        if (newNode == newNode->parent->right)
        {
          newNode = newNode->parent;
          left_rotate(t, newNode);
        }

        // Case 3. 삼촌 노드는 검은색이고 새로운 노드가 왼쪽 자식으로 추가된 경우
        else
        {
          newNode->parent->color = RBTREE_BLACK;
          newNode->parent->parent->color = RBTREE_RED;
          right_rotate(t, newNode->parent->parent);
        }
      }
    }
    // 부모 노드가 할아버지 노드의 오른쪽 자식인 경우
    else
    {
      node_t *uncle = newNode->parent->parent->left;

      // Case 1. 부모 노드와 삼촌 노드의 색깔이 빨간색인 경우
      if (uncle != NULL && uncle->color == RBTREE_RED)
      {
        newNode->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        newNode->parent->parent->color = RBTREE_RED;
        newNode = newNode->parent->parent; // 할아버지 노드로 포커싱 이동
      }
      else
      {
        // Case 2. 삼촌 노드는 검은색이고 새로운 노드가 오른쪽 자식으로 추가된 경우
        if (newNode == newNode->parent->left)
        {
          newNode = newNode->parent;
          right_rotate(t, newNode);
        }

        // Case 3. 삼촌 노드는 검은색이고 새로운 노드가 왼쪽 자식으로 추가된 경우
        else
        {
          newNode->parent->color = RBTREE_BLACK;
          newNode->parent->parent->color = RBTREE_RED;
          left_rotate(t, newNode->parent->parent);
        }
      }
    }
  }

  t->root->color = RBTREE_BLACK;

  return newNode;
}

// const 키워드가 붙어있으면 변경 불가능
node_t *rbtree_find(const rbtree *t, const key_t key)
{
  node_t *curr = t->root;
  while (curr != t->nil && curr->key != key)
  {
    if (curr->key < key)
    {
      curr = curr->right;
    }
    else
    {
      curr = curr->left;
    }
  }

  // key에 해당되는 노드가 없는 경우
  if (curr == t->nil) {
    return NULL;
  }

  return curr;
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  return t->root;
}

//
void transplant(rbtree *t, node_t *u, node_t *v)
{
  if (u->parent == t->nil)
    t->root = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else
    u->parent->right = v;

  v->parent = u->parent;
}

node_t *tree_minimum(rbtree *t, node_t *root)
{
  node_t *curr = root;
  while (curr != t->nil && curr->left != t->nil)
  {
    curr = curr->left;
  }
  return curr;
}

void delete_fixup(rbtree *t, node_t *x)
{
  while (x != t->root && x->color == RBTREE_BLACK)
  {
    if (x == x->parent->left)
    {
      node_t *w = x->parent->right;
      // Case 1
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }

      // Case 2
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else
      {
        // Case 3
        if (w->right->color == RBTREE_BLACK)
        {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }
        // Case 4
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    }
    else
    {
      node_t *w = x->parent->left;
      // Case 1
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }

      // Case 2
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else
      {
        // Case 3
        if (w->left->color == RBTREE_BLACK)
        {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }
        // Case 4
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p)
{
  node_t *del = p;                     // 삭제할 노드 y
  color_t original_color = del->color; // 삭제할 노드의 원래 색상
  node_t *base;                        // 트리 재조정의 기준점이 될 노드 x

  if (p->left == t->nil)
  {
    base = p->right;
    transplant(t, p, p->right);
  }
  else if (p->right == t->nil)
  {
    base = p->left;
    transplant(t, p, p->left);
  }
  else
  {
    del = tree_minimum(t, p->right); // successor 찾기
    original_color = del->color;
    base = del->right;
    if (del != p->right)
    {
      transplant(t, del, del->right);
      del->right = p->right;
      del->right->parent = del;
    }
    else
    {
      base->parent = del;
    }
    transplant(t, p, del);
    del->left = p->left;
    del->left->parent = del;
    del->color = p->color;
  }

  free(p);

  if (original_color == RBTREE_BLACK)
  {
    delete_fixup(t, base);
  }

  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  return 0;
}
