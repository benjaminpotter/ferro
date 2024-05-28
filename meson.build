
project('ferro', 'c')

d_raylib = dependency('raylib', required: true)
d_curl = dependency('libcurl', required: true)
d_libxml = dependency('libxml2', required: true)
dependencies = [d_raylib, d_curl, d_libxml]

includes = include_directories('include')
sources = [ 'source/main.c', 'source/page.c', 'source/fetch.c', 'source/scene.c' ]

executable('ferro', sources, include_directories: includes, dependencies: dependencies)

