t = 0:0.001:0.5;
F = 100;
snr = 90;
onda_senoidal = sin(2*pi*F*t);
%plot(t,onda_senoidal)

con_ruido = onda_senoidal + 0.5*randn(size(t));

%con_ruido = my_awgn(onda_senoidal, snr)


vars = var(onda_senoidal);
varn = vars/(10^(snr/10));
plot(t,con_ruido)

%awgn
