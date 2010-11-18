x = load( "xout.dat" );
y = load( "yout.dat" );
%x = cic(x,4,1,16,1);
%y = cic(y,4,1,16,1);

offset = 20 % garbage samples from reset etc..

z = x(offset:end) + j*y(offset:end);
%z = conv(z,hamming(2*63));
fs = 64e6;
num_fft_points = 8*1024;
num_ffts = floor( length(z)/num_fft_points )

result = zeros(1,num_fft_points);
data = zeros(1,num_fft_points);

for idx=1:num_ffts
   start_idx = (idx-1)*num_fft_points + 1;
   end_idx = idx*num_fft_points;

   data = z(start_idx : end_idx );
   data = fft(data,num_fft_points);
   data = (data .* conj(data))/num_fft_points;

   result = result + transpose(data);
end

%normalize results
result = result/max(result);

%reorder fft output
result = fftshift( transpose( result ) );

%setup log format
result = 10*log10(result);

%create x scale
x_scale = -fs/2:fs/num_fft_points:fs/2-1;

%plot power spectrum
plot(x_scale,result);
grid("minor", "on");
title( "12 Stage 16-bit CORDIC DDC" );
print -deps "results.eps";
ylabel("Power (dB)");
xlabel("Frequency (Hz)");
text(-37e6,-10,"ADC Resolution: 12 bits");
text(-37e6,-15,"Sample Rate: 64 MSPS");
text(-37e6,-20,"DDC rate: 19.609375 MHz");
text(-37e6,-25,"FFT size: 8192 points");
%save "freq.dat" ppx;
