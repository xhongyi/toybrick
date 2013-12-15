#!/usr/bin/bash

e=$2
f=$1

pushd ../../avx
make test_modifier
popd

for injected_error in {6,8,10,15,20,25,30}; do
	time ../../avx/test_modifier 128 $e $injected_error $f > ./func${f}.err${e}.inj$injected_error.txt
done
