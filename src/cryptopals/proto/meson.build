cryptopals_enums_gen = protoc_generator.process(
    'cryptopals_enums.proto',
)
cryptopals_enums_dependencies = [
    protobuf_dep,
]
cryptopals_enums = library(
    'cryptopals_enums',
    cryptopals_enums_gen,
    dependencies: cryptopals_enums_dependencies,
    include_directories: root_include,
)
cryptopals_enums_dep = declare_dependency(
    dependencies: cryptopals_enums_dependencies,
    include_directories: [
        root_include,
        cryptopals_enums.private_dir_include(),
    ],
    link_with: cryptopals_enums,
)
