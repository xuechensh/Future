#include <iostream>
#include <vector>
#include <string>

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& nums) {
  for (const auto& num : nums) {
    out << num << ",";
  }
  return out;
}

struct ListNode {
  ListNode() : value(0), next(nullptr) {}
  int value;
  ListNode* next;
};
std::ostream& operator<<(std::ostream& out, const ListNode& node) {
  const ListNode* ptr = &node;
  while (ptr) {
    out << ptr->value << " -> ";
    ptr = ptr->next;
  }
  out << "END";
  return out;
}
ListNode* GenerateList(const std::vector<int>& nums) {
  if (nums.size() == 0) return nullptr;

  ListNode head, *pre = &head;
  for (const auto& num : nums) {
    ListNode* tmp = new ListNode;
    tmp->value = num;
    pre->next = tmp;
    pre = tmp;
  }
  return head.next;
}
void DeleteList(ListNode* head) {
  while (head) {
    auto next = head->next;
    delete head;
    head = next;
  }
}
int GetListLen(ListNode* head) {
  int ret = 0;
  while (head) ++ret;
  return ret;
}
ListNode* MoveStep(ListNode* head, int step) {
  while (head && step--) {
    head = head->next;
  }
  return head;
}
ListNode* GetTail(ListNode* head) {
  if (!head) return nullptr;

  while (head->next) {
    head = head->next;
  }
  return head;
}

// 1. binary search, set that target in [left,right]
//    left == right exit loop
int BinarySearch(const std::vector<int>& nums, const int& target) {
  if (nums.size() == 0) return false;

  int left = 0, right = nums.size() - 1;
  while (left < right) {
    int mid = left + ((right - left) >> 1);
    if (target > nums[mid]) {
      left = mid + 1;
    } else {
      right = mid;
    }
  }
  return nums[left] == target ? left : -1;
}
void TestBinarySearch() {
  std::cout << "TestBinarySearch" << std::endl;
  std::vector<int> nums = {1, 3, 5, 7, 9, 10, 23, 45};
  std::cout << BinarySearch(nums, 1) << std::endl;
  std::cout << BinarySearch(nums, 5) << std::endl;
  std::cout << BinarySearch(nums, 45) << std::endl;
  std::cout << BinarySearch(nums, 11) << std::endl;
  std::cout << BinarySearch(nums, -1) << std::endl;
  std::cout << BinarySearch(nums, 90) << std::endl;
}

// 2. remove element
int RemoveElement1(std::vector<int>& nums, const int& target) {
  if (nums.size() == 0) return 0;

  int count = nums.size(), slow = 0;
  for (int fast = 0; fast < count; ++fast) {
    if (target != nums[fast]) {
      nums[slow++] = nums[fast];
    }
  }
  return slow;
}
int RemoveElement2(std::vector<int>& nums, const int& target) {
  if (nums.size() == 0) return 0;

  int left = 0, right = nums.size() - 1;
  while (left <= right) {
    while (left <= right && nums[left] != target) ++left;
    while (left <= right && nums[right] == target) --right;

    if (left < right) {
      nums[left++] == nums[right--];
    }
  }
  return left;
}
void TestRemoveElement() {
  std::cout << "TestRemoveElement" << std::endl;
  std::vector<int> nums = {3, 5, 5, 3, 3, 5};
  std::cout << RemoveElement1(nums, 5) << std::endl;
  nums = {3, 5, 5, 3, 3, 5};
  std::cout << RemoveElement2(nums, 5) << std::endl;
  nums = {3, 5, 5, 3, 3, 5};
  std::cout << RemoveElement1(nums, 7) << std::endl;
  nums = {3, 5, 5, 3, 3, 5};
  std::cout << RemoveElement2(nums, 7) << std::endl;
}

// 3.sorted squares
std::vector<int> SortedSquares(const std::vector<int>& nums) {
  if (nums.size() == 0) return {};

  std::vector<int> ret(nums.size(), 0);
  int i = 0, j = nums.size() - 1, k = nums.size() - 1;

  while (i <= j) {
    if (nums[i] * nums[i] < nums[j] * nums[j]) {
      ret[k--] = nums[j] * nums[j];
      --j;
    } else {
      ret[k--] = nums[i] * nums[i];
      ++i;
    }
  }
  return ret;
}
void TestSortedQquares() {
  std::cout << "TestSortedQquares" << std::endl;
  std::vector<int> nums = {-10, -7, -7, -3, 0, 4, 5, 5, 9};
  std::cout << SortedSquares(nums) << std::endl;
}

// 4.mini sub array len
// use move window, two args, one is target, another is weight
int MiniSubArrayLen(const std::vector<int>& nums, int target) {
  if (nums.size() == 0) return -1;

  int i = 0, j = 0, w_sum = 0, mini_len = INT32_MAX, l = nums.size();
  while (j < l) {
    int tmp_sum = w_sum + nums[j];

    if (tmp_sum < target) {
      w_sum = tmp_sum;
      ++j;
      continue;
    }

    int tmp_len = j - i + 1;
    mini_len = tmp_len < mini_len ? tmp_len : mini_len;
    w_sum -= nums[i++];
    if (i > j) j = i;
  }

  return mini_len == INT32_MAX ? -1 : mini_len;
}
void TestMiniSubArrayLen() {
  std::cout << "TestMiniSubArrayLen" << std::endl;
  std::vector<int> nums = {2, 3, 1, 2, 4, 3};
  std::cout << MiniSubArrayLen(nums, 5) << std::endl;
}

// 5.generate matrix

// 6.remove element from list
ListNode* RemoveElementFromList(ListNode* head, int target) {
  ListNode head_tmp;
  head_tmp.next = head;

  ListNode* pre = &head_tmp;
  while (pre->next) {
    if (pre->next->value != target) {
      pre = pre->next;
      continue;
    }

    ListNode* tmp = pre->next;
    pre->next = tmp->next;
    delete tmp;
  }
  return head_tmp.next;
}
void TestRemoveElementFromList() {
  std::cout << "TestRemoveElementFromList" << std::endl;
  ListNode* head = GenerateList({1, 2, 4, 6, 4, 2, 5});
  head = RemoveElementFromList(head, 1);
  std::cout << *head << std::endl;
  head = RemoveElementFromList(head, 2);
  std::cout << *head << std::endl;
  DeleteList(head);
}

// 7.reverse list
ListNode* ReverseList(ListNode* head) {
  ListNode *pre = nullptr, *tmp = nullptr;
  while (head) {
    tmp = head;
    head = head->next;
    tmp->next = pre;
    pre = tmp;
  }
  return pre;
}
void TestReverseList() {
  std::cout << "TestReverseList" << std::endl;
  ListNode* head = GenerateList({1, 2, 4, 6, 4, 2, 5});
  head = ReverseList(head);
  std::cout << *head << std::endl;
  DeleteList(head);
}

// 8.swap tow near node in list
ListNode* SwapPairs(ListNode* head) {
  ListNode head_tmp;
  head_tmp.next = head;

  ListNode *pre = &head_tmp, *tmp = nullptr;
  while (pre->next && pre->next->next) {
    tmp = pre->next;
    pre->next = tmp->next;
    tmp->next = pre->next->next;
    pre->next->next = tmp;
    pre = tmp;
  }

  return head_tmp.next;
}
void TestSwapPairs() {
  std::cout << "TestSwapPairs" << std::endl;
  ListNode* head = GenerateList({1, 2, 4, 6, 4, 2, 5});
  head = SwapPairs(head);
  std::cout << *head << std::endl;
  DeleteList(head);
}

// 9.remove Nth from end of list
ListNode* RemoveNthFromEnd(ListNode* head, int n) {
  ListNode head_tmp;
  head_tmp.next = head;

  ListNode *slow = &head_tmp, *fast = &head_tmp;
  while (n-- && fast) {
    fast = fast->next;
  }
  if (!fast) return head;

  while (fast->next) {
    fast = fast->next;
    slow = slow->next;
  }
  ListNode* tmp = slow->next;
  slow->next = tmp->next;
  delete tmp;

  return head_tmp.next;
}
void TestRemoveNthFromEnd() {
  std::cout << "TestRemoveNthFromEnd" << std::endl;
  ListNode* head = GenerateList({1, 2, 4, 6, 4, 2, 5});
  head = RemoveNthFromEnd(head, 10);
  std::cout << *head << std::endl;
  head = RemoveNthFromEnd(head, 7);
  std::cout << *head << std::endl;
  head = RemoveNthFromEnd(head, 1);
  std::cout << *head << std::endl;
  DeleteList(head);
}

// 10.两个链表的相交节点，注重思想，没法构建测试用例。
ListNode* GetIntersectionNode(ListNode* a, ListNode* b) {
  if (!a || !b) return nullptr;

  int len_a = GetListLen(a);
  int len_b = GetListLen(b);

  if (len_a > len_b) {
    a = MoveStep(a, len_a - len_b);
  } else {
    b = MoveStep(b, len_b - len_a);
  }

  while (a && b) {
    if (a == b) return a;
    a = a->next;
    b = b->next;
  }
  return nullptr;
}

// 11.环形链表，找到环的起始点
ListNode* DetectCycle(ListNode* head) {
  ListNode *slow = head, *fast = head;
  while (fast && fast->next) {
    slow = slow->next;
    fast = fast->next->next;
    if (fast == slow) break;
  }

  if (fast != slow) return nullptr;
  slow = head;
  while (fast != slow) {
    slow = slow->next;
    fast = fast->next;
  }
  return slow;
}
void TestDetectCycle() {
  std::cout << "TestDetectCycle" << std::endl;
  ListNode* head = GenerateList({1, 2, 3, 4, 5});
  if (!DetectCycle(head)) std::cout << "no cycle" << std::endl;
  ListNode* tmp = MoveStep(head, 3);
  ListNode* tail = GetTail(head);
  tail->next = tmp;

  tmp = DetectCycle(head);
  std::cout << tmp->value << std::endl;
}

// 12.有效字母异位词
bool IsAnagram(const std::string& a, const std::string& b) {
  std::vector<int> cnt(26, 0);
  for (const auto& c : a) {
    cnt[c - 'a']++;
  }
  for (const auto& c : b) {
    cnt[c - 'a']--;
  }
  for (const auto& i : cnt) {
    if (i != 0) return false;
  }
  return true;
}
void TestIsAnagram() {
  std::cout << "TestIsAnagram" << std::endl;
  std::string a("asdfbbc"), b("bcbsdfa"), c("asdfbcc");
  std::cout << IsAnagram(a, b) << std::endl;
  std::cout << IsAnagram(a, c) << std::endl;
}

int main() {
  TestBinarySearch();
  TestRemoveElement();
  TestSortedQquares();
  TestMiniSubArrayLen();
  TestRemoveElementFromList();
  TestReverseList();
  TestSwapPairs();
  TestRemoveNthFromEnd();
  TestDetectCycle();
  TestIsAnagram();
}
