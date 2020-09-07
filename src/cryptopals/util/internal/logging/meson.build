log_flags_dependencies = [
    absl_flags_dep,
]
log_flags = library(
    'log_flags',
    files(
        'log_flags.cpp',
    ),
    dependencies: log_flags_dependencies,
    include_directories: root_include,
)
log_flags_dep = declare_dependency(
    dependencies: log_flags_dependencies,
    include_directories: root_include,
    link_with: log_flags,
)

log_entry_dependencies = [
    absl_base_dep,
    absl_strings_dep,
    absl_time_dep,
    log_flags_dep,
]
log_entry = library(
    'log_entry',
    files(
        'log_entry.cpp',
    ),
    dependencies: log_entry_dependencies,
    include_directories: root_include,
)
log_entry_dep = declare_dependency(
    dependencies: log_entry_dependencies,
    include_directories: root_include,
    link_with: log_entry,
)

log_message_dependencies = [
    absl_base_dep,
    absl_hash_dep,
    absl_strings_dep,
    absl_synchronization_dep,
    absl_time_dep,
    log_entry_dep,
    log_flags_dep,
    strerror_dep,
    symbolized_stacktrace_dep,
]
log_message = library(
    'log_message',
    files(
        'log_message.cpp',
    ),
    dependencies: log_message_dependencies,
    include_directories: root_include,
)
log_message_dep = declare_dependency(
    dependencies: log_message_dependencies,
    include_directories: root_include,
    link_with: log_message,
)

vlog_is_on_dependencies = [
    absl_base_dep,
    absl_flags_dep,
    absl_strings_dep,
]
vlog_is_on = library(
    'vlog_is_on',
    files(
        'vlog_is_on.cpp',
    ),
    dependencies: vlog_is_on_dependencies,
    include_directories: root_include,
)
vlog_is_on_dep = declare_dependency(
    dependencies: vlog_is_on_dependencies,
    include_directories: root_include,
    link_with: vlog_is_on,
)

logging_dependencies = [
    absl_strings_dep,
    log_message_dep,
    vlog_is_on_dep,
]
logging = library(
    'logging',
    files(
        'logging.cpp',
    ),
    dependencies: logging_dependencies,
    include_directories: root_include,
)
logging_dep = declare_dependency(
    dependencies: logging_dependencies,
    include_directories: root_include,
    link_with: logging,
)

log_lines_dependencies = [
    absl_strings_dep,
    logging_dep,
    vlog_is_on_dep,
]
log_lines = library(
    'log_lines',
    files(
        'log_lines.cpp',
    ),
    dependencies: log_lines_dependencies,
    include_directories: root_include,
)
log_lines_dep = declare_dependency(
    dependencies: log_lines_dependencies,
    include_directories: root_include,
    link_with: log_lines,
)
