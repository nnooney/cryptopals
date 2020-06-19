single_byte_xor = library(
    'single_byte_xor',
    files(
        'single_byte_xor.cpp',
    ),
    dependencies: [
        bytes_dep,
    ],
    include_directories: root_include,
)

repeating_key_xor = library(
    'repeating_key_xor',
    files(
        'repeating_key_xor.cpp',
    ),
    dependencies: [
        bytes_dep,
    ],
    include_directories: root_include,
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
test('repeating_key_xor_test', repeating_key_xor_test)
