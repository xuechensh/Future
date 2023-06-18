cc_binary (
    name = "move_forward",
    srcs = [
        "move_forward.cc",
    ]
)

cc_binary (
    name = "boost_learn",
    srcs = [
        "boost_learn.cc",
    ]
)

cc_binary (
    name = "coroutine",
    srcs = [
        "coroutine.cc",
    ],
    linkopts = [
        "-L/usr/local/lib",
        "-lboost_coroutine",
        "-lboost_fiber",
        "-lboost_context",
    ],
)

cc_binary (
    name = "libco_run",
    srcs = [
        "libco_run.cc",
    ],
    linkopts = [
        "-L/usr/local/lib",
        "-lpthread",
        "-lcolib",
        "-ldl",
    ],
)

cc_binary (
    name = "libgo_run",
    srcs = [
        "libgo_run.cc",
    ],
    linkopts = [
        "-L/usr/local/lib",
        "-lpthread",
        "-llibgo",
        "-ldl",
    ],
)

cc_library(
    name = "redis",
    srcs = [
        "redis.cc",
    ],
    hdrs = [
        "redis.h",
    ],
)

cc_binary (
    name = "study",
    srcs = [
        "study.cc",
    ],
    copts = [
        "-std=c++20",
    ],
    linkopts = [
        "-lpthread",
    ],
)
