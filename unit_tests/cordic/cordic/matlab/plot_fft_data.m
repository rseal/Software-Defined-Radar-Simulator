x = load( "xout.dat" );
y = load( "yout.dat" );
%x = cic(x,4,1,16,1);
%y = cic(y,4,1,16,1);

Fs = 64e6;
N = 4*1024;

offset = 20 % garbage samples from reset etc..

z = x(offset:end) + j*y(offset:end);

%z = conv(z,hamming(2*63));

num_ffts = floor( length(z)/N )
result = zeros(1,N);
data = zeros(1,N);

for idx=1:num_ffts
   start_idx = (idx-1)*N + 1;
   end_idx = idx*N;

   data = z(start_idx : end_idx );
   data = fft(data,N);
   data = (data .* conj(data))/N;

   result = result + transpose(data);
end

%normalize results
result = result/max(result);

%reorder fft output
result = fftshift( transpose( result ) );

%setup log format
result = 10*log10(result);

%create x scale
x_scale = -Fs/2:Fs/N:Fs/2-1;

%plot power spectrum
plot(x_scale,result);
grid("minor", "on");
title( "12 Stage 16-bit CORDIC DDC" );
print -deps "results.eps";
ylabel("Power (dB)");
xlabel("Frequency (Hz)");
text(-37e6,-10,"ADC Resolution: 12 bits");
text(-37e6,-13,"Sample Rate: 64 MSPS");
text(-37e6,-16,"DDC rate: 6.0398 MHz");
text(-37e6,-19,"FFT size: 8192 points");
text(-37e6,-22,"Integration Size: ");
%save "freq.dat" ppx;
