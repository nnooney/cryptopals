error_code_to_status_dependencies = [
    absl_status_dep,
]
error_code_to_status = library(
    'error_code_to_status',
    files(
        'error_code_to_status.cpp',
    ),
    dependencies: error_code_to_status_dependencies,
    include_directories: root_include,
)
error_code_to_status_dep = declare_dependency(
    dependencies: error_code_to_status_dependencies,
    include_directories: root_include,
    link_with: error_code_to_status,
)

status_builder_dependencies = [
    absl_base_dep,
    absl_container_dep,
    absl_status_dep,
    absl_strings_dep,
    absl_time_dep,
    logging_dep,
    symbolized_stacktrace_dep,
]
status_builder = library(
    'status_builder',
    files(
        'status_builder.cpp',
    ),
    dependencies: status_builder_dependencies,
    include_directories: root_include,
)
status_builder_dep = declare_dependency(
    dependencies: status_builder_dependencies,
    include_directories: root_include,
    link_with: status_builder,
)

ret_check_dependencies = [
    absl_flags_dep,
    absl_status_dep,
    status_builder_dep,
]
ret_check = library(
    'ret_check',
    files(
        'ret_check.cpp',
    ),
    dependencies: ret_check_dependencies,
    include_directories: root_include,
)
ret_check_dep = declare_dependency(
    dependencies: ret_check_dependencies,
    include_directories: root_include,
    link_with: ret_check,
)

statusor_dependencies = [
    absl_base_dep,
    absl_status_dep,
    logging_dep,
]
statusor = library(
    'statusor',
    files(
        'statusor.cpp',
    ),
    dependencies: statusor_dependencies,
    include_directories: root_include,
)
statusor_dep = declare_dependency(
    dependencies: statusor_dependencies,
    include_directories: root_include,
    link_with: statusor,
)

status_macros_dep = declare_dependency(
    dependencies: [
        status_builder_dep,
    ],
)

# This testonly target needs to pass its sources directly to users so that they
# don't violate ODR when including gmock, since gmock exposes its sources to
# users as well.
matchers_src = files('matchers.cpp')
matchers_dependencies = [
    absl_base_dep,
    absl_status_dep,
    absl_strings_dep,
    gmock_dep,
    status_builder_dep,
    status_macros_dep,
    statusor_dep,
]
matchers_dep = declare_dependency(
    sources: matchers_src,
    dependencies: matchers_dependencies,
    include_directories: root_include,
)
