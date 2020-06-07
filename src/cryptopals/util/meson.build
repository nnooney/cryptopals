bytes = library(
    'bytes',
    files(
        'bytes.cpp',
    ),
    include_directories: root_include,
)

bytes_test = executable(
    'bytes_test',
    files(
        'bytes_test.cpp',
    ),
    dependencies: [
        gtest_main_dep,
        gmock_dep,
    ],
    include_directories: root_include,
    link_with: bytes,
)
test('bytes_test', bytes_test)

string_utils = library(
    'string_utils',
    files(
        'string_utils.cpp',
    ),
    include_directories: root_include,
)

# Internal

statusor = library(
    'statusor',
    files(
        'internal/statusor.cpp',
    ),
    dependencies: [
        absl_status_dep,
    ],
    include_directories: root_include,
)