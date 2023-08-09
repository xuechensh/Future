#include <iostream>
#include <vector>

template <typename T>
class MyList {
 public:
  struct ListNode {
    explicit ListNode(const T& v = {}, ListNode* n = nullptr)
        : value(v), next(n) {}
    ~ListNode() { next = nullptr; }

    T value;
    ListNode* next;
  };

  explicit MyList(const std::vector<T>& values = {}) : head_(nullptr) {
    ListNode* pre = nullptr;
    for (const auto& v : values) {
      ListNode* curr = new ListNode(v);
      if (pre) {
        pre->next = curr;
        pre = pre->next;
      } else {
        head_ = curr;
        pre = head_;
      }
    }
  }

  ~MyList() {
    while (head_) {
      ListNode* next = head_->next;
      delete head_;
      head_ = next;
    }
  }

  friend std::ostream& operator<<(std::ostream& out, const MyList<T>& list) {
    ListNode* curr = list.head_;
    while (curr) {
      out << curr->value << "->";
      curr = curr->next;
    }
    out << "nullptr";
    return out;
  }

  int size() const {
    ListNode* curr = head_;
    int ret = 0;

    while (curr) {
      ret += 1;
      curr = curr->next;
    }
    return ret;
  }

  ListNode* GetNthFromHead(int n) const {
    if (!head_) return nullptr;

    ListNode* ret = head_;
    while (--n && ret) ret = ret->next;
    return ret;
  }

  void push_back(ListNode* node) {
    ListNode head_tmp, *pre = &head_tmp;
    head_tmp.next = head_;

    while (pre->next) {
      pre = pre->next;
    }
    pre->next = node;
    head_ = head_tmp.next;
  }

  void pop_back() {
    ListNode head_tmp, *pre = &head_tmp;
    head_tmp.next = head_;

    while (pre->next && pre->next->next) {
      pre = pre->next;
    }
    pre->next = nullptr;
    head_ = head_tmp.next;
  }

  // 1.删除链表上值为target的节点
  void RemoveElement(const T& target) {
    ListNode head_temp;
    head_temp.next = head_;

    ListNode* pre = &head_temp;
    while (pre->next) {
      if (pre->next->value != target) {
        pre = pre->next;
        continue;
      }

      ListNode* need_delete = pre->next;
      pre->next = need_delete->next;
      delete need_delete;
    }
    head_ = head_temp.next;
  }

  // 2.翻转链表，一共需要三个变量
  void Reverse() {
    ListNode *pre = nullptr, *next = nullptr;
    while (head_) {
      next = head_->next;
      head_->next = pre;
      pre = head_;
      head_ = next;
    }
    head_ = pre;
  }

  // 3.两两交换链表中的元素
  void SwapPairs() {
    ListNode head_temp, *pre = &head_temp, *tmp = nullptr;
    head_temp.next = head_;

    while (pre->next && pre->next->next) {
      tmp = pre->next;
      pre->next = tmp->next;
      tmp->next = pre->next->next;
      pre->next->next = tmp;
      pre = tmp;
    }
    head_ = head_temp.next;
  }

  // 4.删除倒数第N个节点
  void RemoveNthFromEnd(int n) {
    ListNode head_temp, *slow = &head_temp, *fast = &head_temp;
    head_temp.next = head_;

    while (n-- && fast) {
      fast = fast->next;
    }
    if (!fast) return;

    while (fast->next) {
      fast = fast->next;
      slow = slow->next;
    }

    ListNode* tmp = slow->next;
    slow->next = tmp->next;
    delete tmp;

    head_ = head_temp.next;
  }

  // 5.两个链表的相交节点
  const ListNode* GetIntersectionNode(const MyList& b) const {
    if (!head_ || !b.head_) return nullptr;

    int size_a = size();
    int size_b = b.size();

    const ListNode *ptr_a = head_, *ptr_b = b.head_;
    if (size_a > size_b) {
      ptr_a = GetNthFromHead(size_a - size_b + 1);
    } else {
      ptr_b = b.GetNthFromHead(size_b - size_a + 1);
    }

    while (ptr_a && ptr_b) {
      if (ptr_a == ptr_b) return ptr_a;
      ptr_a = ptr_a->next;
      ptr_b = ptr_b->next;
    }
    return nullptr;
  }

  // 6.获取环形链表的起始节点
  ListNode* DetectCycle() const {
    ListNode *slow = head_, *fast = head_;
    while (fast && fast->next) {
      slow = slow->next;
      fast = fast->next->next;
      if (slow == fast) break;
    }
    if (slow != fast) return nullptr;

    slow = head_;
    while (slow != fast) {
      slow = slow->next;
      fast = fast->next;
    }
    return slow;
  }

 private:
  ListNode* head_;
};

// 1.测试删除链表上target节点
void TestRemoveElementFromList() {
  std::cout << "TestRemoveElementFromList" << std::endl;
  MyList<int> my_list({1, 2, 3, 4, 1, 2, 3, 4});
  my_list.RemoveElement(3);
  std::cout << my_list << std::endl;
}

// 2.测试翻转链表
void TestReverseList() {
  std::cout << "TestReverseList" << std::endl;
  MyList<int> my_list({1, 2, 3, 4, 1, 2, 3, 4});
  my_list.Reverse();
  std::cout << my_list << std::endl;
}

// 3.测试两两交换链表元素
void TestSwapPairsOfList() {
  std::cout << "TestSwapPairsOfList" << std::endl;
  MyList<int> my_list({1, 2, 3, 4, 1, 2, 3, 4});
  my_list.SwapPairs();
  std::cout << my_list << std::endl;
}

// 4.测试删除倒数第n个节点
void TestRemoveNthFromEnd() {
  std::cout << "TestRemoveNthFromEnd" << std::endl;
  MyList<int> my_list({1, 2, 3, 4, 1, 2, 3, 4});
  my_list.RemoveNthFromEnd(10);
  std::cout << my_list << std::endl;
  my_list.RemoveNthFromEnd(3);
  std::cout << my_list << std::endl;
}

// 5.测试两个链表相交节点
void TestGetIntersectionNode() {
  std::cout << "TestGetIntersectionNode" << std::endl;
  MyList<int> my_list1({1, 2, 3, 4, 1, 2, 3, 4});
  MyList<int> my_list2({1, 2, 3, 4, 1, 2, 3, 4});
  auto node = my_list1.GetIntersectionNode(my_list2);
  if (node) {
    std::cout << "intersection node is: " << node->value << std::endl;
  } else {
    std::cout << "no intersection node" << std::endl;
  }
  MyList<int> my_list3({1, 2, 3, 4});
  my_list3.push_back(my_list1.GetNthFromHead(4));
  node = my_list1.GetIntersectionNode(my_list3);
  if (node) {
    std::cout << "intersection node is: " << node->value << std::endl;
  } else {
    std::cout << "no intersection node" << std::endl;
  }
  my_list3.pop_back();
}

// 6.测试环形链表起始点
void TestDetectCycle() {
  std::cout << "TestDetectCycle" << std::endl;
  MyList<int> my_list1({1, 2, 3, 4, 1, 2, 3, 4});
  my_list1.push_back(my_list1.GetNthFromHead(3));
  auto node = my_list1.DetectCycle();
  if (node) {
    std::cout << "cycle start node is: " << node->value << std::endl;
  } else {
    std::cout << "no cycle start node" << std::endl;
  }
  node = my_list1.GetNthFromHead(8);
  node->next = nullptr;
}

int main() {
  TestRemoveElementFromList();
  TestReverseList();
  TestSwapPairsOfList();
  TestRemoveNthFromEnd();
  TestGetIntersectionNode();
  TestDetectCycle();
  return 0;
}
