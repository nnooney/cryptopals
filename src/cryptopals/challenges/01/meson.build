executable(
  'convert_tool',
  files(
    'convert_tool.cpp',
  ),
  include_directories: root_include,
  dependencies: [
    absl_flags_dep,
    cryptopals_enums_dep,
    logging_dep,
  ],
  link_with: [
    bytes,
    string_utils,
  ],
)
