analyzer_interface_dependencies = [
    bytes_dep,
    cryptopals_logging_dep,
]
analyzer_interface = library(
    'analyzer_interface',
    files(
        'analyzer.cpp',
    ),
    dependencies: analyzer_interface_dependencies,
    include_directories: root_include,
)
analyzer_interface_dep = declare_dependency(
    dependencies: analyzer_interface_dependencies,
    include_directories: root_include,
    link_with: analyzer_interface,
)

frequency_analyzer_dep = declare_dependency(
    dependencies: [
        analyzer_interface_dep
    ],
)

hamming_distance_analyzer_dependencies = [
    analyzer_interface_dep,
    bytes_dep,
]
hamming_distance_analyzer = library(
    'hamming_distance_analyzer',
    files(
        'hamming_distance_analyzer.cpp',
    ),
    dependencies: hamming_distance_analyzer_dependencies,
    include_directories: root_include,
)
hamming_distance_analyzer_dep = declare_dependency(
    dependencies: hamming_distance_analyzer_dependencies,
    include_directories: root_include,
    link_with: hamming_distance_analyzer,
)

hamming_distance_analyzer_test = executable(
    'hamming_distance_analyzer_test',
    files(
        'hamming_distance_analyzer_test.cpp',
    ),
    dependencies: [
        hamming_distance_analyzer_dep,
        gtest_main_dep,
    ],
    include_directories: root_include,
)
test(
    'hamming_distance_analyzer_test',
    hamming_distance_analyzer_test,
    protocol: 'gtest',
    args: test_args,
)

aes_block_analyzer_dependencies = [
    aes_dep,
    analyzer_interface_dep,
    bytes_dep,
    bytes_util_dep,
]
aes_block_analyzer = library(
    'aes_block_analyzer',
    files(
        'aes_block_analyzer.cpp',
    ),
    dependencies: aes_block_analyzer_dependencies,
    include_directories: root_include,
)
aes_block_analyzer_dep = declare_dependency(
    dependencies: aes_block_analyzer_dependencies,
    include_directories: root_include,
    link_with: aes_block_analyzer,
)
