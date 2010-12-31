function result = filterResponse( num_fft_points);

% load data
i = load("inphase.dat");
q = load("quadrature.dat");

len_i = length(i);
len_q = length(q);

samples = len_i

fprintf(1, 'Total samples = %i\n',samples);

if( len_q < len_i )
   samples = len_q;
end

%decimate by 2
i = i(1:2:samples);
q = q(1:2:samples);

y = i + j*q;

% number of integrations to perform
num_integrations = length(y)/num_fft_points;

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
  yfft = fft( y(start_idx:end_idx), num_fft_points);
  yfft = yfft .* conj(yfft);
  % integrate
  result = result + yfft;
end

%format result
result = result/(num_fft_points*num_integrations);
result = fftshift(result);

