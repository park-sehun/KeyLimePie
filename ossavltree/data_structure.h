#include <node.h>

class Ds
{
public:
};

class Int_ds : public Ds
{
public:
    virtual void Minimum(int x) = 0; // key값이 x인 노드가 루트인 서브트리에서의 최소 key를 가지는 노드의 key와 depth를 출력하는 함수
    virtual void Maximum(int x) = 0; // key값이 x인 노드가 루트인 서브트리에서의 최대 key를 가지는 노드의 key와 depth를 출력하는 함수
    virtual void Find(int x) = 0;    // key 값이 x인 노드의 depth를 출력하는 함수
    virtual void Insert(int x) = 0;  // key값이 x인 노드를 삽입하는 함수
    virtual void Rank(int x) = 0;    // key값이 x인 노드의 depth와 rank를 출력하는 함수
    virtual void Erase(int x) = 0;   // key값이 x인 노드를 삭제하는 함수
    virtual void Empty() = 0;        // tree가 비어있는지를 출력하는 함수
    virtual void Size() = 0;         // tree의 크기를 출력하는 함수
};

class Int_AVLtree : public Int_ds
{
public:
    Int_AVLtree();                                           // 생성자 함수
    int getHeight(Int_AVLtree_Node *current_node);           // height를 구하는 함수
    int getBalanceFactor(Int_AVLtree_Node *current_node);    // balanceFactor를 구하는 함수
    int getDepth(Int_AVLtree_Node *current_node);            // depth를 구하는 함수
    void balance(Int_AVLtree_Node *current_node, bool flag); // root가 currnt_node인 서브트리에서 균형을 맞추는 함수
    void rightRotate(Int_AVLtree_Node *node_z);              // rightRotate를 실행하는 함수
    void leftRotate(Int_AVLtree_Node *node_z);               // leftRotate를 실행하는 함수
    void setHeight(Int_AVLtree_Node *current_node);
    int getRank(Int_AVLtree_Node *current_node, int x);                   // key값이 x보다 작은 노드의 개수를 재귀적으로 구하는 함수
    void UpdateLeftSubtreeSize(Int_AVLtree_Node *current_node, int flag); // insert 직후 left_subtree_size를 업데이트 하는 함수
    Int_AVLtree_Node *search(Int_AVLtree_Node *current_node, int x);      // root가 currnt_node인 서브트리에서 key값이 x인 노드를 찾는 함수

    void Minimum(int x) override; // key값이 x인 노드가 루트인 서브트리에서의 최소 key를 가지는 노드의 key와 depth를 출력하는 함수
    void Maximum(int x) override; // key값이 x인 노드가 루트인 서브트리에서의 최대 key를 가지는 노드의 key와 depth를 출력하는 함수
    void Find(int x) override;    // key 값이 x인 노드의 depth를 출력하는 함수
    void Insert(int x) override;  // key값이 x인 노드를 삽입하는 함수
    void Rank(int x) override;    // key값이 x인 노드의 depth와 rank를 출력하는 함수
    void Erase(int x) override;   // key값이 x인 노드를 삭제하는 함수
    void Empty() override;        // tree가 비어있는지를 출력하는 함수
    void Size() override;         // tree의 크기를 출력하는 함수

    Int_AVLtree_Node *root; // tree의 루트 노드
    int node_num;           // tree의 저장된 노드의 숫자
};

// 팩토리 메서드 적용, 단일 책임원칙을 위해 객체 생성역할 분리, 팩토리 메서드에 위임
class ds_creator
{
public:
    Ds *Create_Ds()
    {
        return Get_Ds();
    }
    virtual Ds *Get_Ds() = 0;
};

class Int_ds_creator : public ds_creator
{
public:
    virtual Int_ds *Get_Ds() override = 0;
};

class AVLtree_Int_ds_creator : public Int_ds_creator
{
public:
    Int_AVLtree *Get_Ds()
    {
        return new Int_AVLtree();
    }
};

Int_AVLtree::Int_AVLtree() : root(NULL), node_num(0) {} // 생성자 함수

int Int_AVLtree::getHeight(Int_AVLtree_Node *current_node) // height를 구하는 함수
{
    if (current_node == NULL) // 입력된 노드가 존재하지 않을 경우
    {
        return -1;
    }
    else
    {
        return current_node->height;
    }
}

int Int_AVLtree::getBalanceFactor(Int_AVLtree_Node *current_node) // balanceFactor를 구하는 함수
{
    if (current_node == NULL) // 입력된 노드가 존재하지 않을 경우
    {
        return 0;
    }
    int left_height = 0, right_height = 0;
    if (current_node->left_child != NULL)
    {
        left_height = current_node->left_child->height + 1;
    }
    if (current_node->right_child != NULL)
    {
        right_height = current_node->right_child->height + 1;
    }
    int balance_factor = left_height - right_height;
    return balance_factor;
}

int Int_AVLtree::getDepth(Int_AVLtree_Node *current_node) // depth를 구하는 함수
{
    if (current_node == NULL)
    {
        return 0;
    }

    int depth = 0;
    while (current_node != root) // current_node가 root 노드면 탈출
    {
        current_node = current_node->parent_node;
        depth++;
    }

    return depth;
}

int Int_AVLtree::getRank(Int_AVLtree_Node *current_node, int x)
{                                // key값이 x보다 작은 노드의 개수를 재귀적으로 구하는 함수
    if (x == current_node->key_) // 현재 노드의 key가 x일 경우 해당 노드의 left_subtree_size값 리턴
    {
        return current_node->left_subtree_size;
    }
    else if (x < current_node->key_) // x값이 현재 노드의 key값보다 작을 경우
    {
        if (current_node->left_child == NULL) // 왼쪽 자식이 존재하지 않을 경우 해당 노드가 없다는 뜻이므로 탐색 실패
        {
            return -1;
        }
        else // 왼쪽 자식이 존재 할 경우 그곳으로 이동
            return getRank(current_node->left_child, x);
    }
    else // x값이 현재 노드의 key값보다 클 경우
    {
        if (current_node->right_child == NULL) // 오른쪽 자식이 존재하지 않을 경우 해당 노드가 없다는 뜻이므로 탐색 실패
        {
            return -1;
        }
        else // 오른쪽 자식이 존재하는 경우
        {
            int right_subtree_size = getRank(current_node->right_child, x); // right_size를 재귀적으로 구해옴.
            if (right_subtree_size == -1)                                   // x값을 가진 노드 탐색 실패 할 경우
                return -1;
            else                                                                 //
                return current_node->left_subtree_size + 1 + right_subtree_size; // 현재노드의 key값이 x보다 작으므로 현재노드의 왼쪽 서브트리 사이즈 + 1 + right size 리턴
        }
    }
}

void Int_AVLtree::UpdateLeftSubtreeSize(Int_AVLtree_Node *current_node, int flag)
{                                             // insert 직후, rotation전에 left_subtree_size를 업데이트 하는 함수
    if (current_node == root || root == NULL) // 루트,NULL 도달 시 종료
    {
        return;
    }
    else
    {
        if (current_node == current_node->parent_node->left_child) // current_node가 부모의 왼쪽 자식일 경우 부모의 left_subtree_size 1 증가
        {
            current_node->parent_node->left_subtree_size += flag;
        }
        return UpdateLeftSubtreeSize(current_node->parent_node, flag); // 재귀적으로 루트까지 업데이트
    }
}

Int_AVLtree_Node *Int_AVLtree::search(Int_AVLtree_Node *current_node, int x) // root가 current_node인 서브트리에서 key값이 x인 노드를 찾는 함수
{
    while (current_node != NULL)
    {
        if (x == current_node->key_)
        {
            return current_node;
        } // key값이 x인 노드 발견 성공시 해당 노드 포인터 리턴
        else if (x < current_node->key_)
        { // x값이 현재 노드의 key값보다 작을 경우 현재노드의 left_child로 이동
            current_node = current_node->left_child;
        }
        else
        { // x값이 현재 노드의 key값보다 클 경우 현재 노드의 right_child로 이동
            current_node = current_node->right_child;
        }
    }
    return NULL; // key가 x인 노드를 찾지 못하고 leaf 노드에 도달 한 경우 x값을 가진 노드가 없다는 뜻이므로 NULL리턴
}

void Int_AVLtree::setHeight(Int_AVLtree_Node *current_node) // 노드의 height 변수를 세팅해주는 함수
{
    if (current_node == NULL)
    {
        return;
    }
    if (current_node->left_child == NULL && current_node->right_child == NULL)
    { // 리프노드의 height는 0
        current_node->height = 0;
    }
    else
    { // 좌,우 자식의 height중 더 높은것 +1이 자신의 height가 됨.
        int left_height = -1, right_height = -1;
        if (current_node->left_child != NULL)
        {
            left_height = current_node->left_child->height;
        }
        if (current_node->right_child != NULL)
        {
            right_height = current_node->right_child->height;
        }
        current_node->height = std::max(left_height, right_height) + 1;
    }
    return setHeight(current_node->parent_node); // 재귀호출을 통해 추가된 리프노드부터 부모까지 height 업데이트
}

void Int_AVLtree::balance(Int_AVLtree_Node *current_node, bool flag) // root가 currnt_node인 서브트리에서 균형을 맞추는 함수 , flag가 true면 insert ,false면 delete
{
    if (current_node == NULL) // currenr_node가 없는 경우
    {
        return;
    }
    int current_node_balance_factor = getBalanceFactor(current_node); // currnet_node의 balance factor

    if (current_node_balance_factor <= 1 && current_node_balance_factor >= -1) // 높이차가 1이하인 경우(balancefactor가 -1에서 1사이인 경우)
    {
        return balance(current_node->parent_node, flag);
    }
    else if (current_node_balance_factor >= 1) // 왼쪽 자식 노드의 높이가 2이상으로 더 높은 경우
    {
        if (getBalanceFactor(current_node->left_child) >= 0) // LL 변환
        {
            rightRotate(current_node);
        }
        else // LR 변환
        {
            leftRotate(current_node->left_child);
            rightRotate(current_node);
        }
        if (flag)
        {
            return;
        }
    }
    else if (current_node_balance_factor <= -1) // 오른쪽 자식 노드의 높이가 2이상으로 더 높은 경우
    {
        if (getBalanceFactor(current_node->right_child) > 0) // RL 변환
        {
            rightRotate(current_node->right_child);
            leftRotate(current_node);
        }
        else // RR 변환
        {
            leftRotate(current_node);
        }
        if (flag)
        {
            return;
        }
    }
    return balance(current_node->parent_node, flag);
}

void Int_AVLtree::rightRotate(Int_AVLtree_Node *node_z) // rightRotate를 실행하는 함수
{
    Int_AVLtree_Node *node_y = node_z->left_child;
    Int_AVLtree_Node *T2_root = node_y->right_child;

    node_y->right_child = node_z;
    node_z->left_child = T2_root;

    if (node_z->parent_node == NULL) // node_z가 루트노드였을 경우
    {
        root = node_y; // node_y를 루트노트로 설정
        node_y->parent_node = NULL;
    }
    else if (node_z->parent_node->left_child == node_z) // node_z가 부모노드의 왼쪽 자식 노드였을 경우
    {
        node_z->parent_node->left_child = node_y;
    }
    else // node_z가 부모노드의 오른쪽 자식 노드였을 경우
    {
        node_z->parent_node->right_child = node_y;
    }
    node_y->parent_node = node_z->parent_node;
    node_z->parent_node = node_y;
    if (T2_root != NULL)
    {
        T2_root->parent_node = node_z;
    }

    node_z->left_subtree_size = node_z->left_subtree_size - (node_y->left_subtree_size + 1); // node_z의 왼쪽 서브트리의 사이즈 변화
    setHeight(node_z);
    return;
}

void Int_AVLtree::leftRotate(Int_AVLtree_Node *node_z) // leftRotate를 실행하는 함수
{
    Int_AVLtree_Node *node_y = node_z->right_child;
    Int_AVLtree_Node *T2_root = node_y->left_child;

    node_y->left_child = node_z;
    node_z->right_child = T2_root;

    if (node_z->parent_node == NULL) // node_z가 루트노드였을 경우
    {
        root = node_y; // node_y를 루트노트로 설정
        node_y->parent_node = NULL;
    }
    else if (node_z->parent_node->left_child == node_z) // node_z가 부모노드의 왼쪽 자식 노드였을 경우
    {
        node_z->parent_node->left_child = node_y;
    }
    else // node_z가 부모노드의 왼쪽 자식 노드였을 경우
    {
        node_z->parent_node->right_child = node_y;
    }
    node_y->parent_node = node_z->parent_node;
    node_z->parent_node = node_y;
    if (T2_root != NULL)
    {
        T2_root->parent_node = node_z;
    }
    node_y->left_subtree_size = node_y->left_subtree_size + node_z->left_subtree_size + 1; // node_y의 왼쪽 서브트리의 사이즈 변화
    setHeight(node_z);
    return;
}

void Int_AVLtree::Minimum(int x) // key값이 x인 노드가 루트인 서브트리에서의 최소 key를 가지는 노드의 key와 depth를 출력하는 함수
{
    Int_AVLtree_Node *minimum_node = search(root, x); // x를 key값으로 가지는 노드
    while (minimum_node->left_child != NULL)          // 노드가 존재할때 까지 실행
    {
        minimum_node = minimum_node->left_child; // minimum_node를 minimum_node의 왼쪽 자식 노드로 바꿈
    }
    int minimum_node_depth = getDepth(minimum_node); // minimum_node의 depth를 구함
    std::cout << minimum_node->key_ << " " << minimum_node_depth << "\n";
}

void Int_AVLtree::Maximum(int x) // key값이 x인 노드가 루트인 서브트리에서의 최대 key를 가지는 노드의 key와 depth를 출력하는 함수
{
    Int_AVLtree_Node *root_subtree = search(root, x); // x를 key값으로 가지는 노드
    Int_AVLtree_Node *maximum_node = root_subtree;
    while (maximum_node->right_child != NULL) // 노드가 존재할때 까지 실행
    {
        maximum_node = maximum_node->right_child; // maximum_node를 minimum_node의 오른쪽 자식 노드로 바꿈
    }
    int maximum_node_depth = getDepth(maximum_node); ////maximum_node의 depth를 구함
    std::cout << maximum_node->key_ << " " << maximum_node_depth << "\n";
}

void Int_AVLtree::Empty() // tree가 비어있는지를 출력하는 함수
{
    if (node_num == 0) // tree가 비었을때
    {
        std::cout << 1 << "\n";
        return;
    }
    else // tree가 비어있지 않을 때
    {
        std::cout << 0 << "\n";
        return;
    }
}

void Int_AVLtree::Size() // tree의 크기를 출력하는 함수
{
    std::cout << node_num << "\n";
    return;
}