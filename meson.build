project('neo_minideen', 'cpp',
  default_options : ['buildtype=release', 'b_ndebug=if-release', 'cpp_std=c++17'],
  meson_version : '>=0.51.0',
  version : '1.2'
)

cpp = meson.get_compiler('cpp')
is_msvc_like = cpp.get_argument_syntax() == 'msvc'
is_x86 = host_machine.cpu_family().startswith('x86')

# Prefer system VapourSynth/AviSynth+ headers, fall back to the vendored ones (VapourSynth API 4).
system_vs_dep = dependency('vapoursynth', version : '>=55', required : false)
if system_vs_dep.found()
  vapoursynth_dep = system_vs_dep.partial_dependency(compile_args : true, includes : true)
else
  vapoursynth_dep = declare_dependency(include_directories : include_directories('include/vapoursynth'))
endif

system_avs_dep = dependency('avisynth', required : false)
if system_avs_dep.found()
  avisynth_dep = system_avs_dep.partial_dependency(compile_args : true, includes : true)
else
  avisynth_dep = declare_dependency(include_directories : include_directories('include/avisynth'))
endif

# src/version.hpp.in -> <builddir>/version.hpp
configure_file(
  input : 'src/version.hpp.in',
  output : 'version.hpp',
  configuration : { 'VERSION' : meson.project_version() }
)

sources = [
  'main.cpp',
  'src/cpuid.cpp',
  'src/minideen_core.cpp',
]

# The SIMD routines are selected at runtime, so they are built as separate
# objects with their own instruction set flags. Other architectures (e.g. arm64) use the C routine.
simd_libs = []
if is_x86
  sse2_args = []
  avx2_args = []
  if is_msvc_like
    avx2_args += '/arch:AVX2'
    if host_machine.cpu_family() == 'x86'
      sse2_args += '/arch:SSE2'
    endif
  else
    sse2_args += '-msse2'
    avx2_args += '-mavx2'
  endif
  simd_libs += static_library('minideen_sse2', 'src/minideen_core_SSE2.cpp',
    cpp_args : sse2_args,
    include_directories : include_directories('.'),
    pic : true
  )
  simd_libs += static_library('minideen_avx2', 'src/minideen_core_AVX2.cpp',
    cpp_args : avx2_args,
    include_directories : include_directories('.'),
    pic : true
  )
endif

# VapourSynth only autoloads *.dylib on macOS, while Meson defaults to *.so for modules.
suffix = host_machine.system() == 'darwin' ? 'dylib' : []

if get_option('python_package')
  # Same layout as the VapourSynth PyPI package autoloads from: site-packages/vapoursynth/plugins
  py = import('python').find_installation(pure : false)
  install_dir = join_paths(py.get_install_dir(), 'vapoursynth', 'plugins')
else
  libdir = join_paths(get_option('prefix'), get_option('libdir'))
  if system_vs_dep.found()
    libdir = system_vs_dep.get_variable(pkgconfig : 'libdir', default_value : libdir)
  endif
  install_dir = join_paths(libdir, 'vapoursynth')
endif

shared_module('neo-minideen', sources,
  dependencies : [vapoursynth_dep, avisynth_dep],
  include_directories : include_directories('.', 'include/dualsynth'),
  link_with : simd_libs,
  install : true,
  install_dir : install_dir,
  name_suffix : suffix,
  gnu_symbol_visibility : 'hidden'
)
