#!/bin/bash
file=$(ls *.cpp | tail -n 1)        # last .cpp file
num=${file:0:3}                     # the first three letters of the filename which are numbers
num=$(($((10#$num))+1))             # increment the decimal number
num=$(printf "%03d" $num)           # print the number as three digits
name=$(echo ${*^} | sed 's/ //g')   # make new filename title case and remove spaces
printf "#include <iostream>\n\nusing namespace std;\n\n/*\n${*}\n\n*/\n\nint main(int argc, char const *argv[])\n{\n    return 0;\n}" >> "$num-$name.cpp"
echo $num-$name.cpp                 # print template