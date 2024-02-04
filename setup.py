#!/usr/bin/env python3

from setuptools import Extension, setup

setup(
	name='xdxf2html',
	ext_modules=[
		Extension(
			'xdxf2html',
			['src/utils.cc', 'src/parse.cc', 'src/build.cc', 'src/xdxf2html.cc'],
			extra_compile_args=['-std=c++11']
		)
	]
)
