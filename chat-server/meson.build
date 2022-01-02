project('boost-asio-chat-server','cpp',
    version : '0.1',
    default_options : ['warning_level=3', 'cpp_std=c++14'])

deps = [dependency('boost', modules: ['filesystem', 'thread', 'system'])]
incdir = include_directories('include')

executable('app', 'src/main.cpp', dependencies : deps, include_directories : incdir)