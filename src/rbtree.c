#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void)
{
  // RB Tree 포인터
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  if (p == NULL)
  {
    return NULL;
  }

  /*
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
  */

  // RB Tree 필드 값 초기화
  p->root = NULL;
  p->nil = NULL;

  // rbtree 구조체를 가리키는 포인터 p를 반환
  return p;
}

void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // 새로 추가할 노드를 메모리 할당하고 초기화하기
  struct node_t *newNode = calloc(1, sizeof(struct node_t));
  newNode->color = RBTREE_RED;
  newNode->key = key;
  newNode->parent = NULL;
  newNode->left = NULL;
  newNode->right = NULL;

  // 만약 트리가 비어있는 상태라면 추가하고 리턴하기
  if (t->root == NULL)
  {
    t->root = newNode;
    return newNode;
  }

  struct node_t *curr = t->root;

  while (curr != NULL)
  {
    // 넣으려는 값에 해당되는 노드가 이미 존재하는 경우 삽입하지 않고 NULL 리턴
    if (key == curr->key)
    {
      free(newNode);
      return NULL;
    }
    // 넣으려는 값이 현재 노드의 값보다 작은 경우
    if (key < curr->key)
    {
      // 현재 노드의 왼쪽 자식 노드가 없으면 왼쪽 자식 노드로 추가하고 반복문 종료
      if (curr->left == NULL)
      {
        newNode->parent = curr;
        curr->left = newNode;
        break;
      }
      // 왼쪽 노드로 포커스 이동
      curr = curr->left;
    }
    // 넣으려는 값이 현재 노드의 값보다 큰 경우
    if (key > curr->key)
    {
      // 현재 노드의 오른쪽 자식 노드가 없으면 오른쪽 자식 노드로 추가하고 반복문 종료
      if (curr->right == NULL)
      {
        newNode->parent = curr;
        curr->right = newNode;
        break;
      }
      // 오른쪽 노드로 포커스 이동
      curr = curr->right;
    }
  }

  return t->root;
}

// const 키워드가 붙어있으면 변경 불가능
node_t *rbtree_find(const rbtree *t, const key_t key)
{
  node_t *root = t->root;
  while (root != NULL && root->key != key)
  {
    if (root->key < key)
    {
      root = root->right;
    }
    else
    {
      root = root->left;
    }
  }
  return root;
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

int rbtree_erase(rbtree *t, node_t *p)
{
  // 만약 삭제하려는 노드가 루트 노드인 경우
  if (p == t->root)
  {
    free(p);
    t->root = NULL;
    return 0;
  }

  // 삭제하려는 노드 찾기
  node_t *parent = NULL;
  node_t *target = t->root;

  while (target != p)
  {
    parent = target;
    if (target->key < p->key)
    {
      target = target->right;
    }
    else
    {
      target = target->left;
    }
  }

  // 1) 자식 노드가 없는 노드를 삭제하는 경우
  if (target->left == NULL && target->right == NULL)
  {
    free(target);
    return 0;
  }

  // 2-1) 자식 노드가 한 개인 노드를 삭제하는 경우: 왼쪽 노드만 있는 경우
  else if (target->left != NULL && target->right == NULL)
  {
    parent->left = target->left;
    free(target);
    return 0;
  }

  // 2-1) 자식 노드가 한 개인 노드를 삭제하는 경우: 오른쪽 노드만 있는 경우
  else if (target->left == NULL && target->right != NULL)
  {
    parent->right = target->right;
    free(target);
    return 0;
  }

  // 3) 자식 노드가 두 개인 경우
  else
  {
    // 더 이상 왼쪽 자식이 없을 때까지 타고 내려가서 successor(오른쪽 서브트리에서 제일 작은 값을 가진 노드) 찾기
    node_t *parent = target;
    node_t *successor = target->left;
    while (successor->left != NULL)
    {
      parent = successor;
      successor = successor->left;
    }
    // 삭제하려는 노드의 값을 successor 노드의 값으로 바꾸기
    target->key = successor->key;
    // successor 노드의 오른쪽 자식이 있다면 successor의 부모 노드의 왼쪽 자식으로 붙여주기
    parent->left = successor->right;
    // successor 노드 메모리 해제
    free(successor);
  }

  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  return 0;
}
