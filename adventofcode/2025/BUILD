load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "main",
    srcs = [
        "main.cpp",
        "utils.cpp",
        "days.cpp",
        "utils.h",
        "days.h",
    ],
    copts = [
        "/std:c++17",
        "/EHsc",
    ],
    data = glob(["data/**"]),
)
