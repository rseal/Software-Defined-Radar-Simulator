function result = filterResponse( num_fft_points);

% load data
x = load("i_out.dat");
y = load("q_out.dat");

x_length = length(x);
y_length = length(y);

samples = x_length

fprintf(1, 'Total samples = %i\n',samples);

if( y_length < x_length )
   samples = y_length;
end

offset = 20;
%decimate by 2
x = x(offset:samples);
y = y(offset:samples);

z = x + j*y;

% number of integrations to perform
num_integrations = length(z)/num_fft_points;

% result storage
result = zeros(num_fft_points,1);

%integration loop
for i=1:num_integrations
  % index offset
  idx = (i-1)*num_fft_points;
  % starting index
  start_idx = 1 + idx;
  % ending index
  end_idx = start_idx + num_fft_points;
  % compute fft
  zfft = fft( z(start_idx:end_idx), num_fft_points);
  zfft = zfft .* conj(zfft);
  % integrate
  result = result + zfft;
end

%format result
result = result/(num_fft_points*num_integrations);
result = fftshift(result);
result = 10*log10(result/max(result));

