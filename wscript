import os
import sys
import shutil

def options(ctx):
        ctx.load('compiler_c compiler_cxx')

def configure(ctx):
        ctx.load('compiler_c compiler_cxx')
        ctx.check(
                  features     = 'cxx cxxprogram',
                  libpath      = ['/usr/lib/','/usr/local/lib'],
                  lib          = ['yaml-cpp'],
                  cflags       = ['-std=gnu++0x','-Wall','-W','-02'],
                  uselib_store = 'M'
                 )

def build(ctx):

      src_files = ctx.path.ant_glob('src/*.cpp')

      ctx.shlib( 
            source = src_files,
            target = 'sdr-yaml'
            )

      ctx(rule='cp ${SRC} ${TGT}', source='unit_tests/yaml/sdr.yml', target='sdr.yml')

      #src_files = ctx.path.ant_glob('src/*.cpp')
      #src_files.extend(ctx.path.ant_glob('unit_tests/yaml/*.cpp'))

      ### build primary program
      #ctx.program(
            #name     = 'yaml-test',
            #features = 'cxx cxxprogram',
            #cxxflags = ['-std=gnu++0x','-Wall', '-W'],
            #source   = src_files,
            #target   = 'yaml-test',
            #lib      = ['yaml-cpp'],
            #)

      #ctx.recurse('unit_tests/accumulator')
