#include "classes.h"

using namespace std;

//Class for optimal binary search tree node
template<typename T>
struct BSTNode
{
    BSTNode* left;
    BSTNode* right;
    BSTNode* parent;
    T data;
    explicit BSTNode(const T& dataArg) : left(nullptr), right(nullptr), parent(nullptr), data(dataArg) {};
};

//Ñlass for optimal binary search tree
template<typename T>
class OptimalBST
{
private:
    //Minimal element in left subtree
    OptimalBST<T>* SubtreeMin(BSTNode<T>* localRoot)
    {
        auto ptr = localRoot;
        while (ptr->left)
            ptr = ptr->left;
        return ptr;
    };

    //Maximal element in right subtree
    BSTNode<T>* SubtreeMax(BSTNode<T>* localRoot)
    {
        auto ptr = localRoot;
        while (ptr->right)
            ptr = ptr->right;

        return ptr;
    };

    //Maximal element in both subtrees
    BSTNode<T>* Successor(BSTNode<T>* node)
    {
        if (node->right)
            return SubtreeMax(node);
        auto ptr = node->parent;
        while (ptr && node == ptr->right)
        {
            node = ptr;
            ptr = ptr->parent;
        }
        return ptr;
    };

    //Minimal element in both subtrees
    BSTNode<T>* Predecessor(BSTNode<T>* node)
    {
        if (node->left)
            return SubtreeMin(node);
        auto ptr = node->parent;
        while (ptr && node == ptr->left)
        {
            node = ptr;
            ptr = ptr->parent;
        }
        return ptr;
    };

    //Search node by value
    BSTNode<T>* _search(T& key)
    {
        auto ptr = root;
        while (ptr)
        {
            if (key == ptr->data)
                return ptr;
            if (key < ptr->data)
                ptr = ptr->left;
            else if (key > ptr->data)
                ptr = ptr->right;
        }
        return nullptr;
    };

    //AVL balancing
    void Transplant(BSTNode<T>* where, BSTNode<T>* what)
    {
        if (!where->parent)
            root = what;
        else if (where == where->parent->left)
            where->parent->left = what;
        else where->parent->right = what;

        if (what)
            what->parent = where->parent;
    }

    //Function for interactive finding elements
    BSTNode<T>* ChooseNode(BSTNode<T>* current_node) const
    {
        int position = 0;
        bool end = false;

        while (!end)
        {
            system("cls");
            cout << "Parent Node: ";
            if (current_node->parent)
            {
                cout << current_node->parent->data;
            }
            cout << endl;
            if (position == 0)
                cout << "-> Current Node: " << current_node->data << endl;
            else cout << "   Current Node: " << current_node->data << endl;
            if (position == 1)
            {
                if (current_node->left)
                    cout << "-> Left Node " << current_node->left->data << endl;
                else
                    cout << "-> NULL" << endl;
            }
            else
            {
                if (current_node->left)
                    cout << "   Left Node " << current_node->left->data << endl;
                else
                    cout << "   NULL" << endl;
            }
            if (position == 2)
            {
                if (current_node->right)
                    cout << "-> Right Node " << current_node->right->data << endl;
                else
                    cout << "-> NULL" << endl;
            }
            else
            {
                if (current_node->right)
                    cout << "   Right Node " << current_node->right->data << endl;
                else
                    cout << "   NULL" << endl;
            }
            if (position == 3)
                cout << "-> Back;" << endl;
            else cout << "   Back;" << endl;

            char key;
            key = _getch();
            if ((int)key != 13)
            {
                if ((int)key == -32)
                {
                    key = _getch();
                    if ((int)key == 72)
                    {
                        if (position > 0)
                            position--;
                        else position = 3;
                    }

                    if ((int)key == 80)
                    {
                        if (position < 3)
                            position++;
                        else position = 0;
                    }
                }
            }
            else
            {
                switch (position)
                {
                case 0:
                    return current_node;
                case 1:
                    if (current_node->left)
                        return ChooseNode(current_node->left);
                    break;
                case 2:
                    if (current_node->right)
                        return ChooseNode(current_node->right);
                    break;
                default:
                    if (current_node->parent)
                        return ChooseNode(current_node->parent);
                    break;
                }
            }
        }
    }
public:
    BSTNode<T>* root;

    //Constructor
    OptimalBST() : root(nullptr) {};

    //Insertion
    void Insert(const T& dataArg)
    {
        BSTNode<T>* prev = nullptr;
        BSTNode<T>* ptr = root;
        while (ptr)
        {
            prev = ptr;
            if (dataArg > ptr->data)
                ptr = ptr->right;
            else ptr = ptr->left;
        }

        auto* newNode = new BSTNode<T>(dataArg);
        newNode->left = newNode->right = nullptr;
        newNode->parent = prev;

        if (!prev)
            root = newNode;
        else if (dataArg > prev->data)
            prev->right = newNode;
        else
            prev->left = newNode;
    };

    //Remove element
    void remove(T& key)
    {
        auto forDelete = _search(key);
        if (forDelete)
        {
            if (!forDelete->right)
                Transplant(forDelete, forDelete->left);
            else if (!forDelete->left)
                Transplant(forDelete, forDelete->right);
            else
            {
                auto ptr = SubtreeMin(forDelete->right);
                if (ptr->parent != forDelete)
                {
                    Transplant(ptr, ptr->right);
                    ptr->right = forDelete->right;
                    ptr->parent = forDelete->left;
                }
                Transplant(forDelete, ptr);
                ptr->left = forDelete->left;
                ptr->left->parent = ptr;
            }
            delete forDelete;
        }
    };

    //Check element in tree
    bool At(const T& key) {
        return _search(key) != nullptr;
    };

    //Interactive search
    void Print()
    {
        if (root)
            BSTNode<T>* p = ChooseNode(root);
        else cout << "Nothing" << endl;
    };
};

//Function for creating table of mathimetical expectations
int** optimalBSTRootMatrix(vector<double>& p, vector<double>& q)
{
    int n = p.size();
    double** subtree_cost = new double* [n + 2];
    double** subtree_exp = new double* [n + 2];
    int** root = new int* [n + 1];
    for (int i = 1; i <= n + 1; i++)
    {
        subtree_cost[i] = new double[n + 1];
        subtree_exp[i] = new double[n + 1];
        root[i] = new int[n + 1];
    }
    for (int i = 1; i <= n + 1; i++)
    {
        subtree_cost[i][i - 1] = q[i - 1];
        subtree_exp[i][i - 1] = q[i - 1];
    }

    for (int length = 1; length <= n; length++)
    {
        for (int i = 1; i <= n - length + 1; i++)
        {
            int j = i + length - 1;
            subtree_exp[i][j] = INT_MAX;
            subtree_cost[i][j] = subtree_cost[i][j - 1] + p[j - 1] + q[j];
            for (int r = i; r <= j; r++)
            {
                double tmp = subtree_exp[i][r - 1] + subtree_exp[r + 1][j] + subtree_cost[i][j];
                if (tmp < subtree_exp[i][j])
                {
                    subtree_exp[i][j] = tmp;
                    root[i][j] = r;
                }
            }
        }
    }
    return root;
}

//Creating optimal binary tree
template<typename T>
BSTNode<T>* CreateOptimalBST(std::vector<T>& data, int** rootMatrix, int indexI, int indexJ, BSTNode<T>* parent)
{
    if (indexI < 1 || indexJ > static_cast<int>(data.size()) || indexJ < indexI)
        return nullptr;
    if (indexI == indexJ)
    {
        auto node = new BSTNode<T>(data[indexI - 1]);
        node->parent = parent;
        node->left = node->right = nullptr;
        return node;
    }
    else
    {
        int index = rootMatrix[indexI][indexJ];
        auto node = new BSTNode<T>(data[index - 1]);
        node->parent = parent;
        node->left = CreateOptimalBST(data, rootMatrix, indexI, index - 1, node);
        node->right = CreateOptimalBST(data, rootMatrix, index + 1, indexJ, node);
        return node;
    }
}

//Function that find root of optimal binary tree
template<typename T>
OptimalBST<T> optimalBSTCreateRoot(vector<T>& data, vector<double>& probability, vector<double>& dummy_probability)
{
    auto rootMatrix = optimalBSTRootMatrix(probability, dummy_probability);
    OptimalBST<T> tree;
    tree.root = CreateOptimalBST<T>(data, rootMatrix, 1, data.size(), nullptr);
    return tree;
}

int main()
{
    vector<double> p = { 15, 10, 5, 10, 20 };
    vector<double> q = { 5, 10, 5, 5, 5, 10 };

    Group Man1("A");
    Group Man2("B");
    Group Man3("C");
    Group Man4("D");
    Group Man5("E");

    vector<Group> groups = { Man1, Man2, Man3, Man4, Man5 };
    optimalBSTCreateRoot(groups, p, q).Print();
    return 0;
}