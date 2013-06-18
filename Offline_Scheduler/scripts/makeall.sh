#!/bin/bash
# scriptname Filename 
#Filename should be a list of the scripts you want to run 

for File in $(cat "$1")
do 
   pushd $(dirname $File) 
 #   ls -l $(basename $File) 
   make clean; make exe 
   popd  
done  
