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

logging = library(
    'logging',
    files(
        'logging.cpp',
    ),
    dependencies: [
        absl_flags_dep,
        emilk_loguru_dep,
    ],
    include_directories: root_include,
)
logging_dep = declare_dependency(
    dependencies: [
        absl_flags_dep,
        emilk_loguru_dep,
    ],
    include_directories: root_include,
    link_with: logging,
)

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
