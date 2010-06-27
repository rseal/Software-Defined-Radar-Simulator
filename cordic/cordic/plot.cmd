#!/bin/bash

octave plot_fft_data.m
octave plot_time_data.m
gnuplot -p plot_fft_data.scr
gnuplot -p plot_time_data.scr
