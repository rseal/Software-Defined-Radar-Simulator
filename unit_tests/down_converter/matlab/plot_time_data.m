x = load( "yout.dat" );
size = length(x);
y = x(1:1000);
save "time.dat" y;
