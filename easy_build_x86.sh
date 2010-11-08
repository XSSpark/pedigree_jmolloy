#!/bin/bash

# Script that can be run to set up a Pedigree repository for building with minimal
# effort.

set -e

echo "Pedigree Easy Build script"
echo "This script will ask a couple questions and then automatically fill"
echo "dependencies and compile Pedigree for you."
echo

rl=`which greadlink`
if [ -z "${rl}" ]; then
    rl=`which readlink`
fi
script_dir=`$rl -f $(dirname $0)`

if [ ! -e $script_dir/.easy_os ]; then

    echo "Checking for dependencies... Which operating system are you running on?"
    echo "Cygwin, Debian/Ubuntu, OpenSuSE, OSX, or some other system?"

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
        opensuse)
            echo "Installing packages with zypper, please wait..."
            sudo zypper install mpfr-devel mpc-devel gmp3-devel sqlite3 texinfo scons genisoimage
            ;;
        osx)
            echo "Installing packages with macports, please wait..."
            sudo port install coreutils mpfr libmpc gmp sqlite3 texinfo scons cdrtools wget
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
    
    echo $real_os > $script_dir/.easy_os

    echo

    exit 0

fi

echo "Please wait, checking for a working cross-compiler."
echo "If none is found, the source code for one will be downloaded, and it will be"
echo "compiled for you."

# Install cross-compilers
$script_dir/scripts/checkBuildSystemNoInteractive.pl i686-pedigree $script_dir/pedigree-compiler

scons CROSS=$script_dir/compilers/dir/bin/i686-pedigree-

echo
echo
echo "Pedigree is now ready to be built without running this script."
echo "To build in future, run the following command in the '$script_dir' directory:"
echo "scons CROSS=$script_dir/pedigree-compiler/bin/i686-pedigree-"
echo
echo "If you wish, you can continue to run this script. It won't ask questions"
echo "anymore, unless you remove the '.easy_os' file in '$script_dir'."
echo
echo "You can also run scons --help for more information about options."
echo
echo "Patches should be posted in the issue tracker at http://pedigree-project.org/projects/pedigree/issues"
echo "Support can be found in #pedigree on irc.freenode.net."
echo
echo "Have fun with Pedigree! :)"

