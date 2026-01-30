load("@bazel_tools//tools/cpp:cpp.bzl", "cc_library", "cc_binary")

cc_import(
    name = "headers",
    hdrs = glob(["include/*.h"]),
    includes = ["include"],
    visibility = ["//visibility:private"],
)

cc_binary(
    name = "benchmark_kdtree",
    srcs = ["src/main.cpp"],
    deps = [
        ":headers",
        "@nlohmann_json",
    ],
    copts = ["-std=c++17", "-O2", "-Wall", "-Wextra"],
    linkopts = ["-lm"],
)
