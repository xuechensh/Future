#include <iostream>
#include <vector>

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& nums) {
  for (const auto& num : nums) {
    out << num << ",";
  }
  return out;
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
  return  ret;
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
std::vector<std::vector<int>> GenerateMatrix(int m, int n) {
  std::vector<std::vector<int>> ret(m, std::vector<int>(n, 0));

  int up = 0, down = m - 1, left = 0, right = n - 1;
  int i = 0, j = 0, num = 1;
  while (up <= down && left <= right) {
    i = up;
    j = left;
    while (j <= right) {
      ret[i][j++] = num++;
    }
    i = up + 1;
    j = right;
    while (i <= down) {
      ret[i++][j] = num++;
    }

    while (j > left) {
      ret[i][j--] = num++;
    }
    while (i > up) {
      ret[i--][j] = num++;
    }
  }
  return 
}

int main() {
  TestBinarySearch();
  TestRemoveElement();
  TestSortedQquares();
  TestMiniSubArrayLen();
}
