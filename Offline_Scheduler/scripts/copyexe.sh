#!/bin/bash
# scriptname Filename 
#Filename should be a list of the scripts you want to run 
for File in $(cat "$1")
do 
 cp $2 $(dirname $File)    
#pushd $(dirname $File) 
 #   ls -l $(basename $File) 
#   bash $(basename $File) 
#   popd  
done  
