#!/bin/bash

changeFile="../../src/Driver.cpp"

basePath="API"
headersDir="Handlers"
startSign="BASH AUTO APPEND INCLUDE START"
endSign="BASH AUTO APPEND INCLUDE END"

totalLines=$(wc -l $changeFile | cut -d ' ' -f 1)
startLine=$(grep -n "BASH AUTO APPEND INCLUDE START" $changeFile | cut -d':' -f1)
endLine=$(grep -n "BASH AUTO APPEND INCLUDE END" $changeFile | cut -d':' -f1)

headFile="._head_$(basename "$changeFile").tmp"
tailFile="._tail_$(basename "$changeFile").tmp"
includesFile="._includes_$(basename $changeFile).tmp"

cp $changeFile $changeFile.save

head -n $startLine $changeFile > $headFile
tail -n $(( totalLines - endLine + 1 )) $changeFile > $tailFile

> $includesFile
for hpp in $headersDir/*.hpp
do
        echo "#include \"$basePath/$hpp\"" >> $includesFile
done

cat $headFile > $changeFile
cat $includesFile >> $changeFile
cat $tailFile >> $changeFile

rm $includesFile
rm $headFile
rm $tailFile
