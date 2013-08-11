function bits = bit_adjust( N, rmin, rmax, btrim, bin, bout)
% usage : bits = bit_adjust(N,rmin,rmax,btrim,bout)
%
%     N - number of CIC stages
%  rmin - minimum decimation 
%  rmax - maximum decimation
% btrim - number of bits trimmed by hogenauer method
%  bout - number of output bits

gmin = ceil(N*log2(rmin));
gmax = ceil(N*log2(rmax));

input_bits = bin + N*log2(rmax);
output_bits = bout + N*log2(rmin);
trim_bits = input_bits - output_bits;

printf(' --- CIC BIT PRUNING ALGORITHM ---\n');
printf('MIN BIT GAIN %d\n',gmin);
printf('MAX BIT GAIN %d\n',gmax);
printf('INPUT BITS   %d\n', input_bits);
printf('OUTPUT BITS  %d\n', output_bits);
printf('TRIM BITS    %d\n', trim_bits);
printf(' ---------------------------------\n');

end

