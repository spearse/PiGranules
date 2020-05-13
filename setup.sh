#!/bin/bash

echo "Setting up PiGranules"
echo "Installing python-osc"
pip3 install python-osc

echo "Copying engine "

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        # ...
        cp Engine/Builds/LinuxMakefile/build/PiGranuleEngine .
else
        cp Engine/Builds/MacOSX/build/Debug/PiGranuleEngine .
        # Unknown.
fi
