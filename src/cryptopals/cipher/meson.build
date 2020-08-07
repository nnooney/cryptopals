single_byte_xor = library(
    'single_byte_xor',
    files(
        'single_byte_xor.cpp',
    ),
    dependencies: [
        ascii_dep,
        bytes_dep,
        frequency_analyzer_dep,
    ],
    include_directories: root_include,
)
single_byte_xor_dep = declare_dependency(
    dependencies: [
        ascii_dep,
        bytes_dep,
        frequency_analyzer_dep,
    ],
    include_directories: root_include,
    link_with: single_byte_xor,
)

repeating_key_xor = library(
    'repeating_key_xor',
    files(
        'repeating_key_xor.cpp',
    ),
    dependencies: [
        bytes_dep,
        hamming_distance_analyzer_dep,
        logging_dep,
        single_byte_xor_dep,
    ],
    include_directories: root_include,
)
repeating_key_xor_dep = declare_dependency(
    dependencies: [
        bytes_dep,
        hamming_distance_analyzer_dep,
        logging_dep,
        single_byte_xor_dep,
    ],
    include_directories: root_include,
    link_with: repeating_key_xor,
)

repeating_key_xor_test = executable(
    'repeating_key_xor_test',
    files(
        'repeating_key_xor_test.cpp',
    ),
    dependencies: [
        cryptopals_enums_dep,
        bytes_dep,
        gtest_main_dep,
    ],
    include_directories: root_include,
    link_with: repeating_key_xor,
)
test(
    'repeating_key_xor_test',
    repeating_key_xor_test,
    protocol: 'gtest',
    args: test_args,
)

aes_ecb = library(
    'aes_ecb',
    files(
        'aes_ecb.cpp',
    ),
    dependencies: [
        aes_dep,
        bytes_dep,
        logging_dep,
        statusor_dep,
    ],
    include_directories: root_include,
)
aes_ecb_dep = declare_dependency(
    dependencies: [
        bytes_dep,
    ],
    include_directories: root_include,
    link_with: aes_ecb,
)
