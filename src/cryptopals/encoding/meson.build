ascii = library(
  'ascii',
  files(
    'ascii.cpp',
  ),
  dependencies: [
    absl_container_dep,
    bytes_dep,
  ],
  include_directories: root_include,
)
ascii_dep = declare_dependency(
  dependencies: [
    absl_container_dep,
    bytes_dep,
  ],
  include_directories: root_include,
  link_with: ascii,
)

ascii_test = executable(
  'ascii_test',
  files(
    'ascii_test.cpp',
  ),
  dependencies: [
    absl_container_dep,
    bytes_dep,
    gtest_main_dep,
  ],
  include_directories: root_include,
  link_with: ascii,
)
test('ascii_test', ascii_test)
