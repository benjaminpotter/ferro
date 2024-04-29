
project('wikipedia-scraper', 'c', 'cpp')

curl_dep = dependency('libcurl', required: true)
xml_dep = dependency('libxml2', required: true)
glfw_dep = dependency('glfw3', required: true)
ft2_dep = dependency('freetype2', required: true)

headers = include_directories(
          'extern/glad/include', 
          'extern/imgui', 
          'extern/imgui/backends'
          )

sources =  ['src/main.cpp', 
            'src/fetcher.cpp', 
            'src/window.cpp', 
            'src/node.cpp', 
            'src/shader.cpp',
            'src/text.cpp']
sources += ['extern/glad/src/glad.c']
sources += ['extern/imgui/imgui.cpp', 
            'extern/imgui/imgui_demo.cpp', 
            'extern/imgui/imgui_draw.cpp', 
            'extern/imgui/imgui_tables.cpp',
            'extern/imgui/imgui_widgets.cpp']

sources += ['extern/imgui/backends/imgui_impl_glfw.cpp', 'extern/imgui/backends/imgui_impl_opengl3.cpp']

executable(
    'wikipedia-scraper', 
    sources, 
    include_directories: headers,
    dependencies: [curl_dep, xml_dep, glfw_dep, ft2_dep]
)

