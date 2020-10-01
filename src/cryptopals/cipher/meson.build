single_byte_xor_dependencies = [
    ascii_dep,
    bytes_dep,
    frequency_analyzer_dep,
]
single_byte_xor = library(
    'single_byte_xor',
    files(
        'single_byte_xor.cpp',
    ),
    dependencies: single_byte_xor_dependencies,
    include_directories: root_include,
)
single_byte_xor_dep = declare_dependency(
    dependencies: single_byte_xor_dependencies,
    include_directories: root_include,
    link_with: single_byte_xor,
)

repeating_key_xor_dependencies = [
    bytes_dep,
    bytes_util_dep,
    hamming_distance_analyzer_dep,
    cryptopals_logging_dep,
    single_byte_xor_dep,
]
repeating_key_xor = library(
    'repeating_key_xor',
    files(
        'repeating_key_xor.cpp',
    ),
    dependencies: repeating_key_xor_dependencies,
    include_directories: root_include,
)
repeating_key_xor_dep = declare_dependency(
    dependencies: repeating_key_xor_dependencies,
    include_directories: root_include,
    link_with: repeating_key_xor,
)

repeating_key_xor_test = executable(
    'repeating_key_xor_test',
    files(
        'repeating_key_xor_test.cpp',
    ),
    dependencies: [
        gtest_main_dep,
        repeating_key_xor_dep,
    ],
    include_directories: root_include,
)
test(
    'repeating_key_xor_test',
    repeating_key_xor_test,
    protocol: 'gtest',
    args: test_args,
)

aes_ecb_dependencies = [
    aes_block_analyzer_dep,
    aes_dep,
    bytes_dep,
    cryptopals_logging_dep,
    gl_absl_status_dep,
]
aes_ecb = library(
    'aes_ecb',
    files(
        'aes_ecb.cpp',
    ),
    dependencies: aes_ecb_dependencies,
    include_directories: root_include,
)
aes_ecb_dep = declare_dependency(
    dependencies: aes_ecb_dependencies,
    include_directories: root_include,
    link_with: aes_ecb,
)

aes_cbc_dependencies = [
    aes_dep,
    bytes_dep,
    cryptopals_logging_dep,
    gl_absl_status_dep,
]
aes_cbc = library(
    'aes_cbc',
    files(
        'aes_cbc.cpp',
    ),
    dependencies: aes_cbc_dependencies,
    include_directories: root_include,
)
aes_cbc_dep = declare_dependency(
    dependencies: aes_cbc_dependencies,
    include_directories: root_include,
    link_with: aes_cbc,
)
