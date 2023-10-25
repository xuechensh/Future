cc_library (
    name = "gadget",
    hdrs = [
      "gadget.h",
    ],
)

cc_library (
    name = "widget",
    srcs = [
      "widget.cc",
    ],
    hdrs = [
      "widget.h",
    ],
    deps = [
      "gadget",
    ],
)

cc_binary (
    name = "pimpl_test",
    srcs = [
      "pimpl_test.cc",
    ],
    deps = [
      "widget",
    ],
)
