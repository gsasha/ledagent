#load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def googletest_repository(name):
    commit = "703bd9caab50b139428cea1aaff9974ebee5742e"
    url = "https://github.com/google/googletest/archive/{}.zip".format(commit)
    # Try the following empty sha256 hash first, then replace with whatever
    # bazel says it is looking for once it complains.
    sha256 = ""
    http_archive(
        name = "googletest",
        url = url,
        sha256 = sha256,
        strip_prefix = "googletest-{}".format(commit),
    )
