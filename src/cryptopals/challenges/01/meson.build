executable(
  'convert_tool',
  files(
    'convert_tool.cpp',
  ),
  include_directories: root_include,
  dependencies: [
    absl_flags_dep,
    bytes_dep,
    cryptopals_enums_dep,
    logging_dep,
  ],
  link_with: [
    string_utils,
  ],
)

executable(
  'xor_tool',
  files(
    'xor_tool.cpp',
  ),
  include_directories: root_include,
  dependencies: [
    absl_flags_dep,
    bytes_dep,
    cryptopals_enums_dep,
    logging_dep,
  ],
  link_with: [
    string_utils,
  ],
)

executable(
  'single_byte_xor_tool',
  files(
    'single_byte_xor_tool.cpp',
  ),
  include_directories: root_include,
  dependencies: [
    absl_flags_dep,
    absl_status_dep,
    bytes_dep,
    cryptopals_enums_dep,
    logging_dep,
    single_byte_xor_dep,
  ],
  link_with: [
    string_utils,
  ],
)

executable(
  'repeating_key_xor_tool',
  files(
    'repeating_key_xor_tool.cpp',
  ),
  include_directories: root_include,
  dependencies: [
    absl_flags_dep,
    absl_status_dep,
    bytes_dep,
    cryptopals_enums_dep,
    logging_dep,
    repeating_key_xor_dep,
  ],
  link_with: [
    string_utils,
  ],
)

executable(
  'aes_ecb_tool',
  files(
    'aes_ecb_tool.cpp',
  ),
  include_directories: root_include,
  dependencies: [
    absl_flags_dep,
    absl_status_dep,
    bytes_dep,
    cryptopals_enums_dep,
    logging_dep,
    aes_ecb_dep,
  ],
  link_with: [
    string_utils,
  ],
)
