load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "producter",
    srcs = [
        "producter.cc",
    ],
    hdrs = [
        "producter.h",
    ],
)

cc_binary(
    name = "producter_main",
    srcs = [
        "producter_main.cc",
    ],
    linkopts = [
        "-lrdkafka",
        "-lrdkafka++",
    ],
    deps = [
        ":producter",
    ],
)

cc_library(
    name = "consumer",
    srcs = [
        "consumer.cc",
    ],
    hdrs = [
        "consumer.h",
    ],
)

cc_binary(
    name = "consumer_main",
    srcs = [
        "consumer_main.cc",
    ],
    linkopts = [
        "-lrdkafka",
        "-lrdkafka++",
    ],
    deps = [
        ":consumer",
    ],
)
