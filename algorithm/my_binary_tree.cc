#include <iostream>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& values) {
  out << '[';
  for (auto iter = values.begin(); iter != values.end(); ++iter) {
    if (iter == values.end() - 1) {
      out << *iter;
    } else {
      out << *iter << ',';
    }
  }
  out << ']';
  return out;
}

template <typename T>
class MyBinaryTree {
 public:
  struct TreeNode {
    TreeNode(const T& v)
        : value(v), left(nullptr), right(nullptr), parent(nullptr) {}
    T value;
    TreeNode* left;
    TreeNode* right;
    TreeNode* parent;
  };

  // 1. 构建二叉树 && 展示二叉树
  explicit MyBinaryTree(const std::vector<T>& values) : root_(nullptr) {
    if (values.empty()) return;

    root_ = new TreeNode(values[0]);
    std::queue<TreeNode*> node_queue;
    node_queue.push(root_);
    for (size_t i = 1; i < values.size(); ++i) {
      TreeNode* curr = new TreeNode(values[i]);
      if (i % 2 == 1) {
        node_queue.front()->left = curr;
      } else {
        node_queue.front()->right = curr;
        node_queue.pop();
      }
      node_queue.push(curr);
    }
  }

  // 中序后续构建二叉树
  TreeNode* BuildBinaryTreeImpl(const std::unordered_map<T, int>& value_index,
                                const std::vector<T>& inorder,
                                int inorder_begin, int inorder_end,
                                const std::vector<T>& postorder,
                                int postorder_begin, int postorder_end) {
    if (postorder_begin == postorder_end) return nullptr;

    TreeNode* curr = new TreeNode(postorder[postorder_end - 1]);
    if (postorder_end - postorder_begin == 1) return curr;

    int delimiter_index = value_index.at(postorder[postorder_end - 1]);
    curr->left = BuildBinaryTreeImpl(
        value_index, inorder, inorder_begin, delimiter_index, postorder,
        postorder_begin, postorder_begin + delimiter_index - inorder_begin);
    curr->right = BuildBinaryTreeImpl(
        value_index, inorder, delimiter_index + 1, inorder_end, postorder,
        postorder_begin + delimiter_index - inorder_begin, postorder_end - 1);
    return curr;
  }
  MyBinaryTree(const std::vector<T>& inorder, const std::vector<T>& postorder)
      : root_(nullptr) {
    if (inorder.size() == 0 || postorder.size() == 0) return;
    if (std::unordered_set<T>(inorder.begin(), inorder.end()) !=
        std::unordered_set<T>(postorder.begin(), postorder.end())) {
      return;
    }

    std::unordered_map<T, int> value_index;
    for (size_t i = 0; i < inorder.size(); ++i) {
      value_index[inorder[i]] = i;
    }
    if (value_index.size() != inorder.size()) return;

    root_ = BuildBinaryTreeImpl(value_index, inorder, 0, inorder.size(),
                                postorder, 0, postorder.size());
  }

  static void DisplayImp(TreeNode* node, int level) {
    if (!node) return;

    DisplayImp(node->right, level + 1);
    for (int i = 0; i < level; i++) {
      std::cout << "   ";
    }
    std::cout << node->value << std::endl;
    DisplayImp(node->left, level + 1);
  }
  void Display() {
    std::cout << "=====================" << std::endl;
    if (!root_) {
      std::cout << "null binary tree" << std::endl;
    } else {
      DisplayImp(root_, 0);
    }
    std::cout << "=====================" << std::endl;
  }

  // 2. 先序、中序、后序、层序、翻转二叉树
  std::vector<T> Preorder() const {
    if (!root_) return {};

    std::stack<TreeNode*> node_stack;
    node_stack.push(root_);

    std::vector<T> ret;
    while (!node_stack.empty()) {
      auto curr = node_stack.top();
      node_stack.pop();

      ret.emplace_back(curr->value);
      if (curr->right) node_stack.push(curr->right);
      if (curr->left) node_stack.push(curr->left);
    }
    return ret;
  }

  std::vector<T> Inorder() const {
    std::stack<TreeNode*> node_stack;
    TreeNode* curr = root_;

    std::vector<T> ret;
    while (!node_stack.empty() || curr) {
      while (curr) {
        node_stack.push(curr);
        curr = curr->left;
      }
      curr = node_stack.top();
      node_stack.pop();

      ret.emplace_back(curr->value);
      curr = curr->right;
    }
    return ret;
  }

  std::vector<T> Postorder() const {
    std::stack<TreeNode*> node_stack;
    TreeNode *curr = root_, *visit = nullptr;

    std::vector<T> ret;
    while (!node_stack.empty() || curr) {
      while (curr) {
        node_stack.push(curr);
        curr = curr->left;
      }
      curr = node_stack.top();

      if (curr->right == nullptr || curr->right == visit) {
        node_stack.pop();
        ret.emplace_back(curr->value);
        visit = curr;
        curr = nullptr;
      } else {
        curr = curr->right;
      }
    }
    return ret;
  }

 private:
  TreeNode* root_;
};

// 1. 测试构建二叉树
void TestBuildBinaryTree() {
  std::cout << "TestBuildBinaryTree" << std::endl;
  MyBinaryTree<int> tree1({1, 2, 3, 4, 5});
  tree1.Display();
  MyBinaryTree<int> tree2({6, 4, 7, 2, 5, 1, 3}, {6, 7, 4, 5, 2, 3, 1});
  tree2.Display();
  MyBinaryTree<int> tree3({6, 4, 7, 2, 5, 1}, {6, 7, 4, 5, 2, 3, 1});
  tree3.Display();
}

// 2. 测试先序、中序、后序、层序、翻转二叉树
void TestOrder() {
  std::cout << "TestOrder" << std::endl;
  MyBinaryTree<int> tree1({6, 4, 7, 2, 5, 1, 3}, {6, 7, 4, 5, 2, 3, 1});
  std::cout << tree1.Preorder() << std::endl;
  std::cout << tree1.Inorder() << std::endl;
  std::cout << tree1.Postorder() << std::endl;
}

int main() {
  TestBuildBinaryTree();
  TestOrder();
  return 0;
}
