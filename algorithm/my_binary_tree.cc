#include <deque>
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
struct Preorder {
  Preorder(const std::vector<T>& init_data) : data(init_data) {}
  size_t size() const { return data.size(); }
  std::vector<T> data;
};

template <typename T>
struct Inorder {
  Inorder(const std::vector<T>& init_data) : data(init_data) {}
  size_t size() const { return data.size(); }
  std::vector<T> data;
};

template <typename T>
struct Postorder {
  Postorder(const std::vector<T>& init_data) : data(init_data) {}
  size_t size() const { return data.size(); }
  std::vector<T> data;
};

template <typename T>
struct TreeNode {
  explicit TreeNode(const T& v)
      : value(v), left(nullptr), right(nullptr), parent(nullptr) {}
  T value;
  TreeNode* left;
  TreeNode* right;
  TreeNode* parent;
};

template <typename T>
class MyBinaryTree {
 public:
  MyBinaryTree() : root_(nullptr) {}

  // 1. 构建二叉树 && 展示二叉树
  explicit MyBinaryTree(const std::vector<T>& values) : root_(nullptr) {
    if (values.empty()) return;

    root_ = new TreeNode<T>(values[0]);
    std::queue<TreeNode<T>*> node_queue;
    node_queue.push(root_);
    for (size_t i = 1; i < values.size(); ++i) {
      TreeNode<T>* curr = new TreeNode<T>(values[i]);
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
  TreeNode<T>* BuildBinaryTreeImpl(
      const std::unordered_map<T, int>& value_index,
      const std::vector<T>& inorder, int inorder_begin, int inorder_end,
      const std::vector<T>& postorder, int postorder_begin, int postorder_end) {
    if (postorder_begin == postorder_end) return nullptr;

    TreeNode<T>* curr = new TreeNode<T>(postorder[postorder_end - 1]);
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
  MyBinaryTree(const Inorder<T>& inorder, const Postorder<T>& postorder)
      : root_(nullptr) {
    if (inorder.size() == 0 || postorder.size() == 0) return;
    if (std::unordered_set<T>(inorder.data.begin(), inorder.data.end()) !=
        std::unordered_set<T>(postorder.data.begin(), postorder.data.end())) {
      return;
    }

    std::unordered_map<T, int> value_index;
    for (size_t i = 0; i < inorder.size(); ++i) {
      value_index[inorder.data[i]] = i;
    }
    if (value_index.size() != inorder.size()) return;

    root_ = BuildBinaryTreeImpl(value_index, inorder.data, 0, inorder.size(),
                                postorder.data, 0, postorder.size());
  }

  void Clear() {
    if (!root_) return;

    std::stack<TreeNode<T>*> node_stack;
    node_stack.push(root_);
    TreeNode<T>* curr = nullptr;
    while (!node_stack.empty()) {
      curr = node_stack.top();
      node_stack.pop();

      if (curr->right) node_stack.push(curr->right);
      if (curr->left) node_stack.push(curr->left);

      curr->right = nullptr;
      curr->left = nullptr;
      delete curr;
    }
    root_ = nullptr;
  }

  ~MyBinaryTree() { Clear(); }

  static TreeNode<T>* DeepCopy(TreeNode<T>* node) {
    if (!node) return nullptr;
    auto temp = new TreeNode<T>(node->value);
    temp->left = DeepCopy(node->left);
    temp->right = DeepCopy(node->right);
    return temp;
  }

  static void DisplayImp(TreeNode<T>* node, int level) {
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

    std::stack<TreeNode<T>*> node_stack;
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
    std::stack<TreeNode<T>*> node_stack;
    TreeNode<T>* curr = root_;

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
    std::stack<TreeNode<T>*> node_stack;
    TreeNode<T>*curr = root_, *visit = nullptr;

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

  // 3. 左叶子的和
  T SumOfLeftLeaves() {
    if (!root_) return {};

    std::stack<TreeNode<T>*> node_stack;
    node_stack.push(root_);
    T ret{};
    while (!node_stack.empty()) {
      auto curr = node_stack.top();
      node_stack.pop();

      if (curr->left && !curr->left->left && !curr->left->right) {
        ret += curr->left->value;
      }
      if (curr->left) node_stack.push(curr->left);
      if (curr->right) node_stack.push(curr->right);
    }
    return ret;
  }

  // 4. 左下角的值
  T BottomLeftValue() {
    if (!root_) return {};

    std::queue<TreeNode<T>*> node_queue;
    node_queue.push(root_);
    TreeNode<T>* target = nullptr;
    while (!node_queue.empty()) {
      auto level_size = node_queue.size();
      for (size_t i = 0; i < level_size; ++i) {
        auto curr = node_queue.front();
        node_queue.pop();

        if (i == 0) target = curr;
        if (curr->left) node_queue.push(curr->left);
        if (curr->right) node_queue.push(curr->right);
      }
    }
    return target->value;
  }

  // 5.  从根节点到叶子节点的路径和等于target
  std::vector<std::vector<T>> PathSum(const T& target) {
    std::vector<std::vector<T>> ret;

    TreeNode<T>*curr = root_, *pre = nullptr;
    std::deque<TreeNode<T>*> node_deque;
    T value_tmp{};
    while (curr || !node_deque.empty()) {
      while (curr) {
        value_tmp += curr->value;
        node_deque.push_back(curr);
        curr = curr->left;
      }
      curr = node_deque.back();
      if (!curr->left && !curr->right && value_tmp == target) {
        std::vector<T> tmp;
        for (const auto& iter : node_deque) {
          tmp.emplace_back(iter->value);
        }
        ret.emplace_back(tmp);
      }
      if (!curr->right || curr->right == pre) {
        value_tmp -= curr->value;
        node_deque.pop_back();
        pre = curr;
        curr = nullptr;
      } else {
        curr = curr->right;
      }
    }
    return ret;
  }

  // 7. 合并二叉树
  void Merge(const MyBinaryTree& rhf) {
    if (!root_) {
      root_ = DeepCopy(rhf.root_);
      return;
    }
    if (!rhf.root_) return;

    std::queue<TreeNode<T>*> node_queue;
    node_queue.push(root_);
    node_queue.push(rhf.root_);
    while (!node_queue.empty()) {
      auto node1 = node_queue.front();
      node_queue.pop();
      auto node2 = node_queue.front();
      node_queue.pop();
      node1->value += node2->value;

      if (node1->left && node2->left) {
        node_queue.push(node1->left);
        node_queue.push(node2->left);
      }
      if (node1->right && node2->right) {
        node_queue.push(node1->right);
        node_queue.push(node2->right);
      }
      if (!node1->left && node2->left) {
        node1->left = DeepCopy(node2->left);
      }
      if (!node1->right && node2->right) {
        node1->right = DeepCopy(node2->right);
      }
    }
  }

 protected:
  TreeNode<T>* root_;
};

// 6. 最大二叉树，给定一个数组，用最大值分左右两边，构建二叉树。
template <typename T>
class MaxBinaryTree : public MyBinaryTree<T> {
 public:
  explicit MaxBinaryTree(const std::vector<T>& nums) {
    this->root_ = BuildImpl(nums, 0, nums.size());
  }

 private:
  TreeNode<T>* BuildImpl(const std::vector<T>& nums, size_t left,
                         size_t right) {
    if (left >= right) return nullptr;

    size_t max_value_index = left;
    for (size_t i = left; i < right; ++i) {
      if (nums[i] > nums[max_value_index]) max_value_index = i;
    }
    TreeNode<T>* node = new TreeNode<T>(nums[max_value_index]);
    node->left = BuildImpl(nums, left, max_value_index);
    node->right = BuildImpl(nums, max_value_index + 1, right);
    return node;
  }
};

// 1. 测试构建二叉树
void TestBuildBinaryTree() {
  std::cout << "TestBuildBinaryTree" << std::endl;
  MyBinaryTree<int> tree1({1, 2, 3, 4, 5});
  tree1.Display();
  MyBinaryTree<int> tree2(Inorder<int>({6, 4, 7, 2, 5, 1, 3}),
                          Postorder<int>({6, 7, 4, 5, 2, 3, 1}));
  tree2.Display();
  MyBinaryTree<int> tree3(Inorder<int>({6, 4, 7, 2, 5, 1}),
                          Postorder<int>({6, 7, 4, 5, 2, 3, 1}));
  tree3.Display();
}

// 2. 测试先序、中序、后序、层序、翻转二叉树
void TestOrder() {
  std::cout << "TestOrder" << std::endl;
  MyBinaryTree<int> tree1(Inorder<int>({6, 4, 7, 2, 5, 1, 3}),
                          Postorder<int>({6, 7, 4, 5, 2, 3, 1}));
  std::cout << tree1.Preorder() << std::endl;
  std::cout << tree1.Inorder() << std::endl;
  std::cout << tree1.Postorder() << std::endl;
}

// 3. 测试左叶子之和
void TestSumOfLeftLeaves() {
  std::cout << "TestSumOfLeftLeaves" << std::endl;
  MyBinaryTree<int> tree1(Inorder<int>({6, 4, 7, 2, 5, 1, 3}),
                          Postorder<int>({6, 7, 4, 5, 2, 3, 1}));
  std::cout << tree1.SumOfLeftLeaves() << std::endl;
}

// 4. 测试左下角的值
void TestBottomLeftValue() {
  std::cout << "TestBottomLeftValue" << std::endl;
  MyBinaryTree<int> tree1(Inorder<int>({6, 4, 7, 2, 5, 1, 3}),
                          Postorder<int>({6, 7, 4, 5, 2, 3, 1}));
  std::cout << tree1.BottomLeftValue() << std::endl;
}

// 5. 测试路径和
void TestPathSum() {
  std::cout << "TestPathSum" << std::endl;
  MyBinaryTree<int> tree1(Inorder<int>({7, 11, 2, 4, 5, 13, 8, 6, 3, 1}),
                          Postorder<int>({7, 2, 11, 4, 13, 6, 1, 3, 8, 5}));
  tree1.Display();
  for (const auto& e : tree1.PathSum(22)) {
    std::cout << e << std::endl;
  }
}

// 6. 测试最大二叉树
void TestMaxBinaryTree() {
  std::cout << "TestMaxBinaryTree" << std::endl;
  MaxBinaryTree<int> tree1({3, 2, 1, 6, 0, 5});
  tree1.Display();
}

// 7. 测试合并二叉树
void TestMerge() {
  std::cout << "TestMerge" << std::endl;
  MyBinaryTree<int> tree1(Inorder<int>({7, 11, 2, 4, 5, 13, 8, 6, 3, 1}),
                          Postorder<int>({7, 2, 11, 4, 13, 6, 1, 3, 8, 5}));
  tree1.Display();
  MaxBinaryTree<int> tree2({3, 2, 1, 6, 0, 5});
  tree2.Display();
  tree1.Merge(tree2);
  tree1.Display();
}

int main() {
  TestBuildBinaryTree();
  TestOrder();
  TestSumOfLeftLeaves();
  TestBottomLeftValue();
  TestPathSum();
  TestMaxBinaryTree();
  TestMerge();
  return 0;
}
