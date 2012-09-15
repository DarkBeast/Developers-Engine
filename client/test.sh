#!/bin/sh
export LD_LIBRARY_PATH=/usr/local/lib64:$LD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=/usr/local/lib64:$DYLD_LIBRARY_PATH
exec ./client $*

