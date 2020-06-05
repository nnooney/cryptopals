proto_src_dir = meson.current_source_dir()

cryptopals_enums_proto = custom_target(
  'cryptopals_enums_proto',
  input: files('cryptopals_enums.proto'),
  output: ['cryptopals_enums.pb.cc', 'cryptopals_enums.pb.h'],
  command: [protoc, '--cpp_out', '@OUTDIR@', '-I' + proto_src_dir, '@PLAINNAME@'],
)
cryptopals_enums_dep = declare_dependency(
  sources: cryptopals_enums_proto,
  dependencies: [protobuf_dep],
)
