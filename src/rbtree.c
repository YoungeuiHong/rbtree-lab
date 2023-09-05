#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

void left_rotate(rbtree *t, node_t *x);
void right_rotate(rbtree *t, node_t *x);
void rb_insert_fixup(rbtree *t, node_t *node);
void transplant(rbtree *t, node_t *from, node_t *to);
node_t *tree_minimum(rbtree *t, node_t *root);
void delete_fixup(rbtree *t, node_t *x);
void delete_node(rbtree *t, node_t *node);
void inorder(const rbtree *t, key_t *arr, node_t *node, const size_t n, int *order);

/*
🔴⚫️ RB 트리 구조체 생성 함수
*/
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

  // rbtree 포인터 반환
  return p;
}

/*
🔴⚫️ RB 트리의 모든 노드의 메모리를 해제하는 함수
*/
void delete_node(rbtree *t, node_t *node)
{
  if (node == t->nil)
  {
    return;
  }
  delete_node(t, node->left);  // 왼쪽 노드 탐색
  delete_node(t, node->right); // 오른쪽 노드 탐색
  free(node);                  // 메모리 해제
}

/*
🔴⚫️ RB tree 구조체가 사용했던 메모리를 모두 반환하는 함수
*/
void delete_rbtree(rbtree *t)
{
  delete_node(t, t->root); // 루트 노드를 포함한 모든 노드의 메모리 해제
  free(t->nil);            // nil 노드 메모리 해제
  free(t);                 // RB Tree 메모리 해제
}

/*
🔴⚫️ 주어진 노드를 기준으로 RB 트리를 왼쪽으로 회전시키는 함수
*/
void left_rotate(rbtree *t, node_t *x)
{
  // x는 아래로 내려가는 노드, y는 위로 올라가는 노드
  node_t *y = x->right;
  // y의 왼쪽 서브트리를 x의 오른쪽 서브트리로 옮김
  x->right = y->left;
  if (y->left != t->nil)
  {
    y->left->parent = x;
  }
  y->parent = x->parent;
  // 만약 x가 루트 노드였다면 트리의 루트 노드를 y로 변경
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

/*
🔴⚫️ 주어진 노드를 기준으로 RB 트리를 오른쪽으로 회전시키는 함수
*/
void right_rotate(rbtree *t, node_t *x)
{
  // x는 아래로 내려가는 노드, y는 위로 올라가는 노드
  node_t *y = x->left;
  x->left = y->right;
  if (y->right != t->nil)
  {
    y->right->parent = x;
  }
  y->parent = x->parent;
  // 만약 x가 루트 노드였다면 트리의 루트 노드를 y로 변경
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

/*
🔴⚫️ RB 트리에 노드를 삽입한 후 RB 트리의 속성을 충족할 수 있도록 재조정하는 함수
*/
void rb_insert_fixup(rbtree *t, node_t *node)
{
  // 새로 추가하는 노드의 부모 노드의 색깔이 빨간색일 때까지 반복문 진행
  while (node != t->root && node->parent->color == RBTREE_RED)
  {
    if (node->parent == node->parent->parent->left)
    { // 부모 노드가 할아버지 노드의 왼쪽 자식인 경우
      node_t *uncle = node->parent->parent->right;

      // Case 1. 부모 노드와 삼촌 노드의 색깔이 빨간색인 경우
      if (uncle != NULL && uncle->color == RBTREE_RED)
      {
        node->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;

        node->parent->parent->color = RBTREE_RED;

        node = node->parent->parent; // 할아버지 노드로 포커싱 이동
      }

      else
      {
        // Case 2. 삼촌 노드는 검은색이고 새로운 노드가 오른쪽 자식으로 추가된 경우
        if (node == node->parent->right)
        {
          node = node->parent;
          left_rotate(t, node);
        }

        // Case 3. 삼촌 노드는 검은색이고 새로운 노드가 왼쪽 자식으로 추가된 경우
        else
        {
          node->parent->color = RBTREE_BLACK;
          node->parent->parent->color = RBTREE_RED;
          right_rotate(t, node->parent->parent);
        }
      }
    }
    // 부모 노드가 할아버지 노드의 오른쪽 자식인 경우
    else
    {
      node_t *uncle = node->parent->parent->left;

      // Case 1. 부모 노드와 삼촌 노드의 색깔이 빨간색인 경우
      if (uncle != NULL && uncle->color == RBTREE_RED)
      {
        node->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        node->parent->parent->color = RBTREE_RED;
        node = node->parent->parent; // 할아버지 노드로 포커싱 이동
      }
      else
      {
        // Case 2. 삼촌 노드는 검은색이고 새로운 노드가 오른쪽 자식으로 추가된 경우
        if (node == node->parent->left)
        {
          node = node->parent;
          right_rotate(t, node);
        }

        // Case 3. 삼촌 노드는 검은색이고 새로운 노드가 왼쪽 자식으로 추가된 경우
        else
        {
          node->parent->color = RBTREE_BLACK;
          node->parent->parent->color = RBTREE_RED;
          left_rotate(t, node->parent->parent);
        }
      }
    }
  }

  t->root->color = RBTREE_BLACK;
}

/*
🔴⚫️ RB 트리에 새로운 노드를 삽입하는 함수
*/
node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // 새로 추가할 노드 메모리 할당하기
  struct node_t *new_node = calloc(1, sizeof(struct node_t));

  // 메모리 할당에 실패한 경우 NULL 리턴
  if (new_node == NULL)
  {
    return NULL;
  }

  // 새로 추가할 노드 값 초기화
  new_node->color = RBTREE_RED;
  new_node->key = key;
  new_node->parent = t->nil;
  new_node->left = t->nil;
  new_node->right = t->nil;

  // 만약 트리가 비어있는 상태라면 루트 노드를 추가하고 리턴하기
  if (t->root == t->nil)
  {
    t->root = new_node;
    t->root->color = RBTREE_BLACK; // 루트노드는 검은색
    return new_node;
  }

  struct node_t *curr = t->root; // 새로 추가할 노드와 비교할 노드
  struct node_t *prev = t->nil;  // 새로 추가할 노드의 부모가 될 노드

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
  new_node->parent = prev;

  // 부모 노드의 왼쪽 자식 또는 오른쪽 자식으로 추가
  if (new_node->key < prev->key)
  {
    prev->left = new_node;
  }
  else
  {
    prev->right = new_node;
  }

  rb_insert_fixup(t, new_node);

  return new_node;
}

/*
🔴⚫️ 주어진 key에 해당되는 노드의 포인터를 반환하는 함수
*/
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

  // 주어진 key에 해당되는 노드가 없는 경우
  if (curr == t->nil)
  {
    return NULL;
  }

  return curr;
}

/*
🔴⚫️ RB 트리 내에서 최소값을 가진 노드의 포인터를 반환하는 함수
*/
node_t *rbtree_min(const rbtree *t)
{
  node_t *curr = t->root;
  while (curr != t->nil && curr->left != t->nil)
  {
    curr = curr->left;
  }
  return curr;
}

/*
🔴⚫️ RB 트리 내에서 최대값을 가진 노드의 포인터를 반환하는 함수
*/
node_t *rbtree_max(const rbtree *t)
{
  node_t *curr = t->root;
  while (curr != t->nil && curr->right != t->nil)
  {
    curr = curr->right;
  }
  return curr;
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

/*
🔴⚫️ 주어진 노드의 오른쪽 서브트리에서 최소값을 가진 노드를 찾는 함수
*/
node_t *tree_minimum(rbtree *t, node_t *root)
{
  node_t *curr = root;
  while (curr != t->nil && curr->left != t->nil)
  {
    curr = curr->left;
  }
  return curr;
}

/*
🔴⚫️ 노드 삭제 후 RB 트리의 속성을 충족할 수 있도록 재조정하는 함수
*/
void delete_fixup(rbtree *t, node_t *x)
{
  // while문에서 x는 항상 non-root doubly black node
  while (x != t->root && x->color == RBTREE_BLACK)
  {
    if (x == x->parent->left)
    {
      node_t *w = x->parent->right;
      // Case 1. x의 형제 w가 빨간색 노드일 때
      if (w->color == RBTREE_RED)
      {
        // w의 색상과 x->parent의 색상을 교환
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
        // => 이 과정을 마치면 Case 2, 3, 4 중 하나에 해당하게 됨
      }

      // Case 2. x의 형제 w가 검은색 노드이고, w의 자식들이 모두 검은색 노드일 때
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else
      {
        // Case 3. x의 형제 w가 검은색 노드이고, w의 왼쪽 자식은 빨간색 노드, w의 오른쪽 자식은 검은색 노드일 때
        if (w->right->color == RBTREE_BLACK)
        {
          // w의 색상과 w->left의 색상을 교환한 다음 오른쪽 회전
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
          // => 이 과정을 마치면 Case 4.로 가게 됨
        }

        // Case 4. x의 형제 w가 검은색 노드이고, w의 오른쪽 자식이 빨간색 노드일 때
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        // x->parent를 기준으로 왼쪽으로 회전하여 x에 있는 extra black 제거
        left_rotate(t, x->parent);
        // x를 루트로 변경하여 while문 종료
        x = t->root;
      }
    }
    else
    {
      node_t *w = x->parent->left;
      // Case 1. x의 형제 w가 빨간색 노드일 때
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }

      // Case 2. x의 형제 w가 검은색 노드이고, w의 자식들이 모두 검은색 노드일 때
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK)
      {
        // doubly black이었던 x를 검은색 노드 하나만 가지고 있게 하고, w는 빨간색 노드로 변경함
        w->color = RBTREE_RED;
        // x와 w가 검은색 노드를 읽은 것을 보상하기 위해 x->parent에 extra black을 더해줌
        // case 1.을 거쳐 case 2.로 온 경우 x->parent는 빨간색이었기 때문에 새로운 x는 red and black
        // x는 결국 빨간색 노드가 되어 다음 while문에서 종료됨. 그리고 마지막에 x를 검정색 노드로 변경해줌
        x = x->parent;
      }
      else
      {
        // Case 3. x의 형제 w가 검은색 노드이고, w의 왼쪽 자식은 검은색 노드, w의 오른쪽 자식은 빨간색 노드일 때
        if (w->left->color == RBTREE_BLACK)
        {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }

        // Case 4. x의 형제 w가 검은색 노드이고, w의 왼쪽 자식이 빨간색 노드일 때
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

/*
🔴⚫️ RB 트리에서 인자로 주어진 노드를 삭제하고 메모리를 반환하는 함수
*/
int rbtree_erase(rbtree *t, node_t *p)
{
  node_t *del = p;                     // 삭제할 노드 y
  color_t original_color = del->color; // 삭제할 노드의 원래 색상
  node_t *base;                        // 트리 재조정의 기준점이 될 노드 x

  if (p->left == t->nil)
  {
    base = p->right;
    // p를 p의 오른쪽 자식 노드로 대체
    transplant(t, p, p->right);
  }
  else if (p->right == t->nil)
  {
    base = p->left;
    // p를 p의 왼쪽 자식 노드로 대체
    transplant(t, p, p->left);
  }
  else
  {
    del = tree_minimum(t, p->right); // successor 찾기
    original_color = del->color;
    base = del->right;
    // 만약 successor가 p의 오른쪽 자식 노드가 아닌 경우
    if (del != p->right)
    {
      // successor을 successor의 오른쪽 sub tree로 교체
      transplant(t, del, del->right);
      del->right = p->right;
      del->right->parent = del;
    }
    else
    {
      // base(successor의 오른쪽 자식 노드)가 nil 노드인 경우를 위해 parent 값 설정해주기
      base->parent = del;
    }
    transplant(t, p, del);
    del->left = p->left;
    del->left->parent = del;
    del->color = p->color;
  }

  // 삭제하려는 노드의 메모리 해제하기
  free(p);

  // 검은색 노드를 삭제한 경우 RB 트리 속성이 깨질 수 있으므로 재조정 작업하기
  if (original_color == RBTREE_BLACK)
  {
    delete_fixup(t, base);
  }

  return 0;
}

/*
🔴⚫️ RB 트리 내에서 최소값을 가진 노드를 찾는 함수
*/
void inorder(const rbtree *t, key_t *arr, node_t *node, const size_t n, int *order)
{
  // n개까지만 배열로 변환
  if (node == t->nil || *order > n)
  {
    return;
  }

  inorder(t, arr, node->left, n, order);

  arr[*order] = node->key;
  (*order)++;

  inorder(t, arr, node->right, n, order);
}

/*
🔴⚫️ RB 트리를 key를 기준으로 오름차순으로 정렬된 배열로 변환하는 함수
array의 크기는 n으로 주어지며 tree의 크기가 n 보다 큰 경우에는 순서대로 n개 까지만 변환
*/
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  int order = 0;
  inorder(t, arr, t->root, n, &order);
  return 0;
}
