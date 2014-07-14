#!/bin/bash
#filename:chy
first=""
while true
do
    word=`xclip -out`
    if [ "$word" != "$first" ]
    then
        reset
        echo $word "翻译结果（英->中):"
        chy $word
    fi
    first=$word
    sleep 1
done
