
project('wikipedia-scraper', 'cpp')

curl_dep = dependency('libcurl', required: true)

executable('wikipedia-scraper', ['src/main.cpp', 'src/fetcher.cpp'], dependencies: [curl_dep])

