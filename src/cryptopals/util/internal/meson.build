strerror = library(
    'strerror',
    files(
        'strerror.cpp',
    ),
    include_directories: root_include,
)
strerror_dep = declare_dependency(
    include_directories: root_include,
    link_with: strerror,
)

symbolized_stacktrace_dependencies = [
    absl_debugging_dep,
    absl_strings_dep,
]
symbolized_stacktrace = library(
    'symbolized_stacktrace',
    files(
        'symbolized_stacktrace.cpp',
    ),
    dependencies: symbolized_stacktrace_dependencies,
    include_directories: root_include,
)
symbolized_stacktrace_dep = declare_dependency(
    dependencies: symbolized_stacktrace_dependencies,
    include_directories: root_include,
    link_with: symbolized_stacktrace,
)

subdir('logging')
subdir('status')
