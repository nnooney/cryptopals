project('cryptopals', 'cpp',
    version: '0.1.7',
    default_options: ['cpp_std=c++2a']
)

# Compiler Options
cpp = meson.get_compiler('cpp')

cpp_flags = [
  '-Wno-sign-compare',
  '-Wno-deprecated-declarations',
]
flags = cpp.get_supported_arguments(cpp_flags)
add_project_arguments(flags, language: 'cpp')

subdir('src')
