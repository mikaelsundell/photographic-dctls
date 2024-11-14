#!/usr/bin/env python3
## Copyright 2022-present Contributors to the photographic-dctl project.
## SPDX-License-Identifier: BSD-3-Clause
## https://github.com/mikaelsundell/photographic-dctls

import glob
import os
import sys
import shutil
import datetime
import subprocess

def is_mac():
    return sys.platform == 'darwin'

def run_log():
    if is_mac():
        log_file = os.path.expanduser("~/Library/Application Support/Blackmagic Design/DaVinci Resolve/logs/davinci_resolve.log")
    else:
        log_file = os.path.expanduser(r"~\AppData\Roaming\Blackmagic Design\DaVinci Resolve\Support\logs\davinci_resolve.log")
    
    if os.path.isfile(log_file):
        if is_mac():
            os.system('clear')
        else:
            os.system('cls')
        print("Starting to trace the log file...")
        if is_mac():
            subprocess.run(['tail', '-f', log_file])
        else:
            subprocess.run(['powershell', '-Command', f"Get-Content -Path '{log_file}' -Wait"])
    else:
        print("Log file does not exist, make sure Davinci Resolve is installed.")
        sys.exit(1)

def run_readme():
    if is_mac():
        readme_file = "/Library/Application Support/Blackmagic Design/DaVinci Resolve/Developer/DaVinciCTL/README.txt"
    else:
        readme_file = r"C:\ProgramData\Blackmagic Design\DaVinci Resolve\Support\Developer\DaVinciCTL\README.txt"
    
    if os.path.isfile(readme_file):
        if is_mac():
            subprocess.run(['open', readme_file])
        else:
            os.startfile(readme_file)
    else:
        print("README file does not exist.")
        sys.exit(1)

def run_examples():
    if is_mac():
        examples_dir = "/Library/Application Support/Blackmagic Design/DaVinci Resolve/Developer/DaVinciCTL"
    else:
        examples_dir = r"C:\ProgramData\Blackmagic Design\DaVinci Resolve\Support\Developer\DaVinciCTL"
    
    if os.path.isdir(examples_dir):
        if is_mac():
            subprocess.run(['open', examples_dir])
        else:
            os.startfile(examples_dir)
    else:
        print("Examples directory does not exist.")
        sys.exit(1)

def run_install():
    if is_mac():
        target_dir = "/Library/Application Support/Blackmagic Design/DaVinci Resolve/LUT/DCTL"
    else:
        target_dir = r"C:\ProgramData\Blackmagic Design\DaVinci Resolve\Support\LUT\DCTL"
    
    if not os.path.isdir(target_dir):
        print(f"Target directory {target_dir} does not exist. Creating the directory.")
        os.makedirs(target_dir)
    
    current_dir = os.getcwd()
    h_files = glob.glob(os.path.join(current_dir, '*.h'))
    dctl_files = glob.glob(os.path.join(current_dir, '*.dctl'))
    lut_files = glob.glob(os.path.join(current_dir, '*.cube'))
    
    for file in h_files + dctl_files + lut_files:
        shutil.copy(file, target_dir)
        print(f"Copied {file} to {target_dir}")

def run_snapshot():
    if is_mac():
        target_dir = "/Library/Application Support/Blackmagic Design/DaVinci Resolve/LUT/DCTL"
    else:
        target_dir = r"C:\ProgramData\Blackmagic Design\DaVinci Resolve\Support\LUT\DCTL"
    
    date_time = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
    archive_name = f"DCTL_{date_time}.tar.gz"
    if os.path.isdir(target_dir):
        print(f"Creating snapshot of {target_dir}...")
        shutil.make_archive(f"DCTL_{date_time}", 'gztar', os.path.dirname(target_dir), os.path.basename(target_dir))
        print(f"Snapshot saved as {archive_name}")
    else:
        print("Target directory does not exist.")
        sys.exit(1)

def main():
    if len(sys.argv) != 2:
        print("Usage: python script.py {log|readme|examples|install|snapshot}")
        sys.exit(1)

    command = sys.argv[1]
    if command == 'log':
        run_log()
    elif command == 'readme':
        run_readme()
    elif command == 'examples':
        run_examples()
    elif command == 'install':
        run_install()
    elif command == 'snapshot':
        run_snapshot()
    else:
        print("Usage: python script.py {log|readme|examples|install|snapshot}")
        sys.exit(1)

if __name__ == "__main__":
    main()
