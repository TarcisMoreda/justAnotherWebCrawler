project(
  'jawc', 'c',
  version: '1.0',
  default_options: ['warning_level=2', 'buildType=debugoptimized', 'c_std=c23']
)

cc = meson.get_compiler('c')
dependencies = [
  cc.find_library('curl'),
  cc.find_library('tidy')
]

executable(
  'jawc',
  sources: [
    'src/main.c'
  ],
  dependencies: [dependencies],
  install: true
)
