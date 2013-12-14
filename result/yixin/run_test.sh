#!/usr/bin/bash

e=$1

pushd ../../avx
make test_modifier
popd

for injected_error in {1,2,3,4,5,10,20,30}; do
	../../avx/test_modifier 128 $e $injected_error > ./err${e}.inj$injected_error.txt
done
