#!/bin/bash
for i in $(find crashed/success -type f); do echo $i; ./jpg2bmp $i test.bmp; done
