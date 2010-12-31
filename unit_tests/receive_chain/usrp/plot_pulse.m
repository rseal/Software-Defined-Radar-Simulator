i = load('i_out.dat');
q = load('q_out.dat');

plot(i);
hold on;
plot(q,'g');
xlim([0,50]);
title('USRP 2us pulse width, 3kHz doppler, decimation = 32, sample rate = 64e6');
xlabel('sample number (n)');
ylabel('output value');

