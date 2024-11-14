#!/bin/bash
## Copyright 2022-present Contributors to the photographic-dctl project.
## SPDX-License-Identifier: BSD-3-Clause
## https://github.com/mikaelsundell/photographic-dctls

script_dir=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
dctl_dir="/Library/Application Support/Blackmagic Design/DaVinci Resolve/LUT/DCTL"

if [ ! -d "$dctl_dir" ]; then
    echo "DCTL could not be found, make sure Davinci Resolve is installed."
    exit 1
fi

echo "Copying .dctl and .h files to $dctl_dir"
cp -v "$script_dir"/*.dctl "$dctl_dir"
cp -v "$script_dir"/*.h "$dctl_dir"

echo "Copy of dctl files completed."