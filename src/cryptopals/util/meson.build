logging_dependencies = [
    absl_flags_dep,
    emilk_loguru_dep,
]
logging = library(
    'logging',
    files(
        'logging.cpp',
    ),
    dependencies: logging_dependencies,
    include_directories: root_include,
)
logging_dep = declare_dependency(
    dependencies: logging_dependencies,
    include_directories: root_include,
    link_with: logging,
)

status_builder_dependencies = [
    absl_status_dep,
]
status_builder = library(
    'status_builder',
    files(
        'internal/status_builder.cpp',
    ),
    dependencies: status_builder_dependencies,
    include_directories: root_include,
)
status_builder_dep = declare_dependency(
    dependencies: status_builder_dependencies,
    include_directories: root_include,
    link_with: status_builder,
)

statusor_dependencies = [
    absl_status_dep,
    logging_dep,
    status_builder_dep,
]
statusor = library(
    'statusor',
    files(
        'internal/statusor.cpp',
    ),
    dependencies: statusor_dependencies,
    include_directories: root_include,
)
statusor_dep = declare_dependency(
    dependencies: statusor_dependencies,
    include_directories: root_include,
    link_with: statusor,
)

status_macros_dep = declare_dependency(
    dependencies: [
        status_builder_dep,
        statusor_dep,
    ],
)

bytes_dependencies = [
    cryptopals_enums_dep,
]
bytes = library(
    'bytes',
    files(
        'bytes.cpp',
    ),
    dependencies: bytes_dependencies,
    include_directories: root_include,
)
bytes_dep = declare_dependency(
    dependencies: bytes_dependencies,
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

bytes_util_dependencies = [
    bytes_dep,
]
bytes_util = library(
    'bytes_util',
    files(
        'bytes_util.cpp',
    ),
    dependencies: bytes_util_dependencies,
    include_directories: root_include,
)
bytes_util_dep = declare_dependency(
    dependencies: bytes_util_dependencies,
    include_directories: root_include,
    link_with: bytes_util,
)

string_utils = library(
    'string_utils',
    files(
        'string_utils.cpp',
    ),
    include_directories: root_include,
)
string_utils_dep = declare_dependency(
    include_directories: root_include,
    link_with: string_utils,
)

aes_dependencies = [
    absl_status_dep,
    absl_strings_dep,
    bytes_dep,
    statusor_dep,
]
aes = library(
    'aes',
    files(
        'aes.cpp',
    ),
    dependencies: aes_dependencies,
    include_directories: root_include,
)
aes_dep = declare_dependency(
    dependencies: aes_dependencies,
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
        gmock_main_dep,
    ],
    include_directories: root_include,
)
test(
    'aes_test',
    aes_test,
    protocol: 'gtest',
    args: test_args,
)

tool_helpers_dependencies = [
    cryptopals_enums_dep,
    logging_dep,
    statusor_dep,
    string_utils_dep,
]
tool_helpers = library(
    'tool_helpers',
    files(
        'tool_helpers.cpp',
    ),
    dependencies: tool_helpers_dependencies,
    include_directories: root_include,
)
tool_helpers_dep = declare_dependency(
    dependencies: tool_helpers_dependencies,
    include_directories: root_include,
    link_with: tool_helpers,
)

init_cryptopals_dependencies = [
    absl_flags_dep,
    logging_dep,
]
init_cryptopals = library(
    'init_cryptopals',
    files(
        'init_cryptopals.cpp',
    ),
    dependencies: init_cryptopals_dependencies,
    include_directories: root_include,
)
init_cryptopals_dep = declare_dependency(
    dependencies: init_cryptopals_dependencies,
    include_directories: root_include,
    link_with: init_cryptopals,
)
