
d_in = load('../build/input.dat');
cordic_i_out = load('../build/cordic_i_output.dat');
cordic_q_out = load('../build/cordic_q_output.dat');
cic_i_out = load('../build/cic_i_output.dat');
cic_q_out = load('../build/cic_q_output.dat');
i_out = load('../build/i_out.dat');
q_out = load('../build/q_out.dat');

gain = max(i_out)/max(d_in)

