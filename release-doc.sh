#!/bin/bash
#
# Maintainer script to build and push documentation

set -e

base_dir=$(dirname $(readlink -f $0))
build_dir=${base_dir}/build

rm -rf ${base_dir}/doc/sphinx/_html_extra/c-api
rm -rf ${base_dir}/doc/sphinx/_html_extra/gjs-api
rm -rf ${base_dir}/doc/sphinx/_html_extra/python-api

mkdir -p ${base_dir}/doc/sphinx/_html_extra/c-api
mkdir -p ${base_dir}/doc/sphinx/_html_extra/gjs-api
mkdir -p ${base_dir}/doc/sphinx/_html_extra/python-api

rm -rf ${build_dir}
cmake -D CMAKE_BUILD_TYPE=Release -D GTKDOC_REBASE_ONLINE=Yes -B${build_dir} -H${base_dir}
make -C ${build_dir} doc

cp -R ${build_dir}/src/grx/html/* ${base_dir}/doc/sphinx/_html_extra/c-api
yelp-build html -o ${base_dir}/doc/sphinx/_html_extra/gjs-api ${build_dir}/doc/yelp/gjs
yelp-build html -o ${base_dir}/doc/sphinx/_html_extra/python-api ${build_dir}/doc/yelp/python
