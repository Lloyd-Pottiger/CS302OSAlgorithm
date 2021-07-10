#!/bin/bash
# Write the Linux bash script to view the number of files and subdirectories
# contained in given directory and export it to a given filename.
SALVEIFS=$IFS
IFS=$(echo -en "\n\b")

function dirFun(){
    arr=$(cd $1; pwd)
    l=0
    r=1
    while(($l != $r))
    do
        dir_name=${arr[$l]}
        echo "["${dir_name##*/}"]" >> $dst
        l=`expr $l + 1`
        for filename in `ls $dir_name`
        do
            echo "$dir_name/$filename" >> $dst
            if [ -d "$dir_name/$filename" ]
            then
                arr[r]=$dir_name/$filename
                r=`expr $r + 1`
            fi
        done
        echo "" >> $dst
    done
}

src=$1
dst=$2
: > $2
dirFun "$src"
file_cnt=$(find $src/ -type f -print | wc -l)
dir_cnt=$(find $src/* -type d | wc -l)
echo "[Directories Count]:$dir_cnt" >> $dst
echo "[Files Count]:$file_cnt" >> $dst

IFS=$SAVEIFS