
project('wikipedia-scraper', 'c', 'cpp')

curl_dep = dependency('libcurl', required: true)
xml_dep = dependency('libxml2', required: true)
glfw_dep = dependency('glfw3', required: true)

headers = include_directories('extern/glad/include')
sources = ['src/main.cpp', 'src/fetcher.cpp', 'src/window.cpp', 'src/node.cpp']
sources += ['extern/glad/src/glad.c']

executable(
    'wikipedia-scraper', 
    sources, 
    include_directories: headers,
    dependencies: [curl_dep, xml_dep, glfw_dep]
)

