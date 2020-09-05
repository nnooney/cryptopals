ascii_dependencies = [
    absl_container_dep,
    bytes_dep,
]
ascii = library(
    'ascii',
    files(
        'ascii.cpp',
    ),
    dependencies: ascii_dependencies,
    include_directories: root_include,
)
ascii_dep = declare_dependency(
  dependencies: ascii_dependencies,
  include_directories: root_include,
  link_with: ascii,
)

ascii_test = executable(
  'ascii_test',
  files(
    'ascii_test.cpp',
  ),
  dependencies: [
      ascii_dep,
      gtest_main_dep,
   ],
  include_directories: root_include,
)
test(
    'ascii_test',
    ascii_test,
    protocol: 'gtest',
    args: test_args
)
