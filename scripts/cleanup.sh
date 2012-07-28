#!/bin/bash

find ../ -name "*~" -delete
find ../ -name "*.swp" -delete 
find ../ -name "*.swo" -delete 
find ../ -name "*.o" -delete 

#remove all dat files
find ../ -name "*.dat" -delete
