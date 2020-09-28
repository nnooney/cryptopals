root_include = include_directories('.')

test_args = ['alsologtostderr']

# Include google-lacuna
gl_proj = subproject('google-lacuna')
gl_absl_base_dep = gl_proj.get_variable('gl_absl_base_dep')
absl_container_dep = gl_proj.get_variable('absl_container_dep')
gl_absl_debugging_dep = gl_proj.get_variable('gl_absl_debugging_dep')
absl_flags_dep = gl_proj.get_variable('absl_flags_dep')
absl_hash_dep = gl_proj.get_variable('absl_hash_dep')
gl_absl_logging_dep = gl_proj.get_variable('gl_absl_logging_dep')
absl_numeric_dep = gl_proj.get_variable('absl_numeric_dep')
absl_random_dep = gl_proj.get_variable('absl_random_dep')
gl_absl_status_dep = gl_proj.get_variable('gl_absl_status_dep')
absl_strings_dep = gl_proj.get_variable('absl_strings_dep')
absl_synchronization_dep = gl_proj.get_variable('absl_synchronization_dep')
absl_time_dep = gl_proj.get_variable('absl_time_dep')
absl_types_dep = gl_proj.get_variable('absl_types_dep')
protobuf_dep = gl_proj.get_variable('protobuf_dep')
gl_gtest_dep = gl_proj.get_variable('gl_gtest_dep')
gtest_dep = gl_proj.get_variable('gtest_dep')
gtest_main_dep = gl_proj.get_variable('gtest_main_dep')
gmock_dep = gl_proj.get_variable('gmock_dep')
gmock_main_dep = gl_proj.get_variable('gmock_main_dep')

protoc = gl_proj.get_variable('protoc')
protoc_generator = generator(
    protoc,
    output: ['@BASENAME@.pb.cc', '@BASENAME@.pb.h'],
    arguments: [
        '--proto_path=@CURRENT_SOURCE_DIR@',
        '--cpp_out=@BUILD_DIR@',
        '@INPUT@'
    ],
)
protoc_command = [
    protoc,
    '--proto_path=@CURRENT_SOURCE_DIR@',
    '--cpp_out=@OUTDIR@',
    '@INPUT@'
]

subdir('cryptopals')
