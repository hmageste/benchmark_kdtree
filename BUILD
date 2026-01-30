load("@rules_cc//cc:cc_binary.bzl", "cc_binary")
load("@rules_cc//cc:cc_library.bzl", "cc_library")
load("@rules_cc//cc:cc_import.bzl", "cc_import")

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
        "@nlohmann_json//:json",
    ],
    copts = ["-std=c++17", "-O2", "-Wall", "-Wextra"],
    linkopts = ["-lm"],
)
