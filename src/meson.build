root_include = include_directories('.')

test_args = ['alsologtostderr']

# Include Abseil for utility code.
absl_proj = subproject('abseil-cpp')
absl_base_dep = absl_proj.get_variable('absl_base_dep')
absl_container_dep = absl_proj.get_variable('absl_container_dep')
absl_debugging_dep = absl_proj.get_variable('absl_debugging_dep')
absl_flags_dep = absl_proj.get_variable('absl_flags_dep')
absl_hash_dep = absl_proj.get_variable('absl_hash_dep')
absl_numeric_dep = absl_proj.get_variable('absl_numeric_dep')
absl_random_dep = absl_proj.get_variable('absl_random_dep')
absl_status_dep = absl_proj.get_variable('absl_status_dep')
absl_strings_dep = absl_proj.get_variable('absl_strings_dep')
absl_synchronization_dep = absl_proj.get_variable('absl_synchronization_dep')
absl_time_dep = absl_proj.get_variable('absl_time_dep')
absl_types_dep = absl_proj.get_variable('absl_types_dep')

# Include Loguru dependency.
emilk_loguru_proj = subproject('emilk-loguru')
emilk_loguru_dep = emilk_loguru_proj.get_variable('emilk_loguru_dep')

# Include Protobuf dependency.
protobuf_proj = subproject('protobuf')
protobuf_dep = protobuf_proj.get_variable('protobuf_dep')
protoc = protobuf_proj.get_variable('protoc')

protoc_generator = generator(
    protoc,
    output: ['@BASENAME@.pb.cc', '@BASENAME@.pb.h'],
    arguments: ['--cpp_out', '@BUILD_DIR@', '-I@CURRENT_SOURCE_DIR@', '@PLAINNAME@']
)

# Include GoogleTest for all test code.
gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_dep')
gtest_main_dep = gtest_proj.get_variable('gtest_main_dep')
gmock_dep = gtest_proj.get_variable('gmock_dep')
gmock_main_dep = gtest_proj.get_variable('gmock_main_dep')

subdir('cryptopals')
