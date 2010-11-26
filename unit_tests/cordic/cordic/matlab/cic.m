function Y = cic(X,n,m,r,g)

% X data sequence
% n number stages
% m differential delay
% r decimation
% g gain

% this creates taps for CIC filter
cic = ones(m*r,1);
Y = zeros(1,length(X)/r);
for i=1:n
    X = conv(X,cic);
end

% decimate and scale by g
Y = g*X(1:r:end);

