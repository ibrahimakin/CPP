#!/bin/bash
if [ -z "$1" ]          # if $1 is empty
then
    for x in ./*
    do                  # run all executables except .sh files
        if [[ $x != *.sh ]] && [ -x "$x" ];
        then
            echo $(basename $x);
            $x;
        fi
    done
else                    # run executable starts with first ($1) argument
    find ./ -type f -name "$1*" -executable -exec sh -c 'echo $(basename $1); exec "$1"' _ {} \;
fi