load("@rules_cc//cc:defs.bzl", "cc_library", "cc_binary", "cc_test")

package(default_visibility = ["//visibility:public"])

cc_library(
  name = "atomic_mutex",
  hdrs = [
    "atomic_mutex.h",
  ],
)

cc_test(
  name = "atomic_mutex_test",
  srcs = [
    "atomic_mutex_test.cc"
  ],
  deps = [
    ":atomic_mutex",
    "@com_google_googletest//:gtest_main"
  ],
)

cc_library(
  name = "recursive_mutex",
  hdrs = [
    "recursive_mutex.h",
  ],
  linkopts = [
    "-lpthread",
  ],
)

cc_binary(
  name = "recursive_mutex_test",
  srcs = [
    "recursive_mutex_test.cc",
  ],
  deps = [
    "recursive_mutex",
  ],
  linkopts = [
    "-lpthread",
  ],
)

cc_library(
  name = "print",
  srcs = [
    "print.cc",
  ],
  hdrs = [
    "print.h",
  ],
  deps = [
    ":recursive_mutex",
  ],
)

cc_binary(
  name = "print_test",
  srcs = [
    "print_test.cc",
  ],
  deps = [
    "print",
  ],
  linkopts = [
    "-lpthread",
  ],
)

cc_library(
  name = "thread_pool",
  srcs = [
    "thread_pool.cc",
  ],
  hdrs = [
    "thread_pool.h",
  ],
  deps = [
    "atomic_mutex",
  ],
  linkopts = [
    "-lpthread",
  ],
)

cc_binary(
  name = "thread_pool_test",
  srcs = [
    "thread_pool_test.cc",
  ],
  deps = [
    "thread_pool",
  ],
)

cc_library(
  name = "thread_wrapper",
  hdrs = [
    "thread_wrapper.h",
  ],
  linkopts = [
    "-lpthread",
  ],
)
