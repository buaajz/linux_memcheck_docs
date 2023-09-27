cc_binary(
    name = "hello_world",
    srcs = ["hello_world.cc"],
    malloc = "@com_google_tcmalloc//tcmalloc",
)

# This library provides tcmalloc always
cc_binary(
    name = "libgwp-asan-clang.so",
    srcs = [
        "gwp-asan.cc",
    ],
    #linkstatic = 1,
    deps = [
                "@com_google_tcmalloc//tcmalloc",
        ],
    malloc = "@com_google_tcmalloc//tcmalloc",
    linkshared=True,
)

