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

status_builder = library(
    'status_builder',
    files(
        'internal/status_builder.cpp',
    ),
    dependencies: [
        absl_status_dep,
    ],
    include_directories: root_include,
)
status_builder_dep = declare_dependency(
    dependencies: [
        absl_status_dep,
    ],
    include_directories: root_include,
    link_with: status_builder,
)
statusor = library(
    'statusor',
    files(
        'internal/statusor.cpp',
    ),
    dependencies: [
        absl_status_dep,
        logging_dep,
        status_builder_dep,
    ],
    include_directories: root_include,
)
statusor_dep = declare_dependency(
    dependencies: [
        absl_status_dep,
        logging_dep,
        status_builder_dep,
    ],
    include_directories: root_include,
    link_with: statusor,
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
        bytes_dep,
        gtest_main_dep,
    ],
    include_directories: root_include,
)
test(
    'bytes_test',
    bytes_test,
    protocol: 'gtest',
    args: test_args,
)

string_utils = library(
    'string_utils',
    files(
        'string_utils.cpp',
    ),
    include_directories: root_include,
)

aes = library(
    'aes',
    files(
        'aes.cpp',
    ),
    dependencies: [
        absl_status_dep,
        absl_strings_dep,
        bytes_dep,
        statusor_dep,
    ],
    include_directories: root_include,
)
aes_dep = declare_dependency(
    dependencies: [
        absl_status_dep,
        absl_strings_dep,
        bytes_dep,
        statusor_dep,
    ],
    include_directories: root_include,
    link_with: aes,
)
aes_test = executable(
    'aes_test',
    files(
        'aes_test.cpp',
    ),
    dependencies: [
        aes_dep,
        bytes_dep,
        gmock_main_dep,
        statusor_dep,
    ],
    include_directories: root_include,
)
test(
    'aes_test',
    aes_test,
    protocol: 'gtest',
    args: test_args,
)
