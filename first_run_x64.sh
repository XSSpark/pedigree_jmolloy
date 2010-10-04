#!/bin/bash

# Script that can be run to set up a Pedigree repository for building with minimal
# effort.

set -e

echo "Pedigree First Build script"
echo "This script will ask a couple questions and then automatically fill"
echo "dependencies and compile Pedigree for the first time."
echo
echo "Checking for dependencies... Which operating system are you running on?"
echo "Cygwin, Debian/Ubuntu, or some other system?"

read os

shopt -s nocasematch

real_os=$os

case $real_os in
     debian)
        # TODO: Not sure if the package list is any different for debian vs ubuntu?
        echo "Installing packages with apt-get, please wait..."
        sudo apt-get install libmpfr-dev libmpc-dev libgmp3-dev sqlite3 texinfo scons genisoimage
        ;;
     ubuntu)
        echo "Installing packages with apt-get, please wait..."
        sudo apt-get install libmpfr-dev libmpc-dev libgmp3-dev sqlite3 texinfo scons genisoimage
        ;;
    cygwin)
        echo "Please ensure you use Cygwin's 'setup.exe' to install the following:"
        echo " - Python"
        echo " - GCC & binutils"
        echo " - libgmp, libmpc, libmpfr"
        echo " - mkisofs/genisoimage"
        echo " - sqlite"
        echo "You will need to find alternative sources for the following:"
        echo " - mtools"
        echo " - scons"
        ;;
    *)
        echo "Operating system '$os' is not supported yet."
        echo "You will need to find alternative sources for the following:"
        echo " - Python"
        echo " - GCC & binutils"
        echo " - libgmp, libmpc, libmpfr"
        echo " - mkisofs/genisoimage"
        echo " - sqlite"
        echo " - mtools"
        echo " - scons"
        echo
        echo "If you can modify this script to support '$os', please provide patches."
        ;;
esac

shopt -u nocasematch

echo
echo

echo "Please wait, checking for a working cross-compiler."
echo "If none is found, the source code for one will be downloaded, and it will be"
echo "compiled for you."

# Install cross-compilers
script_dir=`readlink -f $(dirname $0)`
$script_dir/scripts/checkBuildSystemNoInteractive.pl amd64-pedigree $script_dir/pedigree-compiler

scons CROSS=$script_dir/compilers/dir/bin/amd64-pedigree-

echo
echo
echo "Pedigree is now ready to be built without running this script."
echo "To build in future, run the following command in the '$script_dir' directory:"
echo "scons CROSS=$script_dir/pedigree-compiler/bin/amd64-pedigree-"
echo
echo "You can also run scons --help for more information about options."
echo
echo "Patches should be posted in the issue tracker at http://pedigree-project.org/projects/pedigree/issues"
echo "Support can be found in #pedigree on irc.freenode.net."
echo
echo "Have fun with Pedigree! :)"

