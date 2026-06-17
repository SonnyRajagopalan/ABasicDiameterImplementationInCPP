#!/bin/bash

directory=$1;
wildcard=$2;

cd $directory;
for i in `ls $wildcard`;
do
  echo $i;
  sed 's/std::cout/\/\/std::cout/g' $i;
done;