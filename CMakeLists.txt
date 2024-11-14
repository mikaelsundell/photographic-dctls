# Copyright 2022-present Contributors to the photographic-dctl project.
# SPDX-License-Identifier: BSD-3-Clause
# https://github.com/mikaelsundell/photographic-dctls

cmake_minimum_required(VERSION 3.27)
set( project_name "photographic-dctls" )

project( "${project_name} project" )

file(GLOB dctl "*.dctl")
install(FILES ${dctl} DESTINATION dctl)
