#!/bin/bash

usage(){
    printf "usage:\n\t$0 script dir regex\n\n"
}

if [[ "$#" -ne 3 ]]
then
    usage
    exit $!
fi

make clean
for i in $(find $2 -regextype sed -regex $3)
do
    $1 $i
done