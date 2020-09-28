cryptopals_enums_gen = custom_target(
    'cryptopals_enums_gen',
    input: ['cryptopals_enums.proto'],
    output: ['cryptopals_enums.pb.cc', 'cryptopals_enums.pb.h'],
    command: protoc_command,
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
# Include the generated header in dependencies to ensure the correct build
# order. See https://mesonbuild.com/Wrap-best-practices-and-tips.html#declare-generated-headers-explicitly.
cryptopals_enums_dep = declare_dependency(
    sources: cryptopals_enums_gen[1],
    dependencies: cryptopals_enums_dependencies,
    include_directories: root_include,
    link_with: cryptopals_enums,
)
