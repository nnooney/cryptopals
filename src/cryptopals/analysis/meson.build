analyzer_interface = library(
    'analyzer_interface',
    files(
        'analyzer.cpp',
    ),
    dependencies: [
        bytes_dep,
        logging_dep,
    ],
    include_directories: root_include,
)
analyzer_interface_dep = declare_dependency(
    dependencies: [
        bytes_dep,
        logging_dep,
    ],
    include_directories: root_include,
    link_with: analyzer_interface,
)

frequency_analyzer_dep = declare_dependency(
    dependencies: [
        analyzer_interface_dep
    ],
)

hamming_distance_analyzer = library(
    'hamming_distance_analyzer',
    files(
        'hamming_distance_analyzer.cpp',
    ),
    dependencies: [
        analyzer_interface_dep,
        bytes_dep,
    ],
    include_directories: root_include,
)
hamming_distance_analyzer_dep = declare_dependency(
    dependencies: [
        analyzer_interface_dep,
        bytes_dep,
    ],
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
