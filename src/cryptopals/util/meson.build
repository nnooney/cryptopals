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

bytes = library(
    'bytes',
    files(
        'bytes.cpp',
    ),
    dependencies: [
        cryptopals_enums_dep,
    ],
    include_directories: root_include,
)
bytes_dep = declare_dependency(
    dependencies: [
        cryptopals_enums_dep,
    ],
    include_directories: root_include,
    link_with: bytes,
)

bytes_test = executable(
    'bytes_test',
    files(
        'bytes_test.cpp',
    ),
    dependencies: [
        cryptopals_enums_dep,
        gtest_main_dep,
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
