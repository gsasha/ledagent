package(default_visibility = ["//visibility:public"])

exports_files([
  "emulator-layout-rectangle.yaml",
  "emulator-layout-screens.yaml",
])

cc_library(
  name="rate-data",
  srcs=["rate-data.cc"],
  hdrs=["rate-data.h"],
)

cc_library(
  name="server-pru",
  srcs=["server-pru.cc"],
  hdrs=["server-pru.h"],
)

cc_library(
  name="driver",
  srcs = [
    "animation.cc",
    "driver.cc",
    "effect.cc",
    "render.cc",
  ],
  hdrs=[
    "animation.h",
    "driver.h",
    "effect.h",
    "render.h",
  ],
  deps = [
    "@yaml-cpp//:yaml-cpp",
    ":rate-data",
  ],
  linkopts = [
    "-pthread",
  ],
)

cc_library(
  name="fltk_driver",
  srcs = [
    "fltk-driver.cc",
  ],
  hdrs = [
    "fltk-driver.h",
  ],
  deps = [
    "@system//:libfltk",
    "@yaml-cpp//:yaml-cpp",
    ":driver",
  ]
)

cc_library(
  name="ledscape_driver",
  srcs = [
    "ledscape_driver.cc",
    "server-config.cc",
    "server-error.cc",
    "server-pru.cc",
    "color.cc",
  ],
  hdrs = [
    "ledscape_driver.h",
    "server-config.h",
    "server-error.h",
    "server-pru.h",
    "color.h",
  ],
  deps = [
    "@cesanta_frozen//:frozen",
    ":driver",
    "//ledscape:ledscape",
  ]
)

cc_binary(
  name="screen_runner",
  srcs=["screen_runner.cc"],
  deps=[
    "@com_google_absl//absl/flags:flag",
    "@com_google_absl//absl/flags:parse",
    #"@system//:libyaml",
    "@yaml-cpp//:yaml-cpp",
    ":fltk_driver",
  ],
  linkopts = [
    #'-lyaml-cpp',
  ],
)
