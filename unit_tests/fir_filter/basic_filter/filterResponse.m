function result = filterResponse( file_name, num_fft_points);

% load data
x = load(file_name);
%decimate by 2
%x = x(1:2:end);

%y = cic(x,4,1,4,4**(-4));
%y=x;

% number of integrations to perform
num_integrations = length(x)/num_fft_points;

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
   xfft = fft(x(start_idx:end_idx),num_fft_points);
   xfft = xfft .* conj(xfft);
   % integrate
   result = result + xfft;
end

%format result
result = result/(num_fft_points*num_integrations);
result = fftshift(result);
