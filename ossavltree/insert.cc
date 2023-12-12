#include "set.h"

void Int_AVLtree::Insert(int x) // key값이 x인 노드를 삽입하는 함수
{
    AVLtree_Int_node_creator node_creator;
    Int_AVLtree_Node *new_node = (Int_AVLtree_Node *)node_creator.CreateNode(x); // 새로운 노드 생성
    if (getNode_num() == 0)                                                      // tree가 비어있을 때
    {
        setRoot(new_node);
        setNode_num(getNode_num() + 1);
        std::cout << getDepth(new_node) << "\n";
        return;
    }

    Int_AVLtree_Node *current_node = getRoot(); // new_node가 삽일될 노드
    Int_AVLtree_Node *parent_node = NULL;       // 부모 노드가 될 노드
    int child_factor = 0;                       // 왼쪽 자식 노드인지 오른쪽 자식 노드인지 결정하는 인자

    while (current_node != NULL)
    {
        parent_node = current_node;
        if (current_node->getKey_() > x) // current_node key가 x보다 클 때
        {
            current_node = current_node->getLeft_child();
            child_factor = -1;
        }
        else if (current_node->getKey_() < x) // current_node key가 x보다 작을 때
        {
            current_node = current_node->getRight_child();
            child_factor = 1;
        }
        else if (current_node->getKey_() == x) // x를 key값으로 가지는 노드가 이미 존재할 때
        {
            std::cout << "이미 존재하는 값입니다./insert"
                      << "\n";
            return;
        }
    }

    new_node->setParent_node(parent_node);

    if (child_factor == -1) // 왼쪽 자식 노드일 경우
    {
        parent_node->setLeft_child(new_node);
    }
    else if (child_factor == 1) // 오른쪽 자식 노드일 경우
    {
        parent_node->setRight_child(new_node);
    }
    else // 에러가 일어났을 경우
    {
        std::cout << "잘못된 child_factor 값입니다./insert"
                  << "\n";
        return;
    }
    setNode_num(getNode_num() + 1);
    UpdateLeftSubtreeSize(new_node, 1);
    setHeight(new_node);
    balance(new_node, true);
    std::cout << getDepth(new_node) << "\n";
}