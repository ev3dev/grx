#!/bin/bash
#
# Maintainer script to build and push documentation

set -e

base_dir=$(dirname $(readlink -f $0))
build_dir=${base_dir}/doc-build
html_extra_dir=${base_dir}/doc/sphinx/_html_extra

rm -rf ${html_extra_dir}/c-api
rm -rf ${html_extra_dir}/gjs-api
rm -rf ${html_extra_dir}/python-api
rm -rf ${html_extra_dir}/vala-api

mkdir -p ${html_extra_dir}/c-api
mkdir -p ${html_extra_dir}/gjs-api
mkdir -p ${html_extra_dir}/python-api
mkdir -p ${html_extra_dir}/vala-api

rm -rf ${build_dir}
cmake -D CMAKE_BUILD_TYPE=Release -D GTKDOC_REBASE_ONLINE=Yes -B${build_dir} -H${base_dir}
make -s -C ${build_dir} doc

cp -R ${build_dir}/src/grx/html/* ${html_extra_dir}/c-api
yelp-build html -o ${html_extra_dir}/gjs-api ${build_dir}/doc/yelp/gjs
yelp-build html -o ${html_extra_dir}/python-api ${build_dir}/doc/yelp/python
cp -R ${build_dir}/doc/valadoc/valadoc/* ${html_extra_dir}/vala-api

rm -rf ${build_dir}
