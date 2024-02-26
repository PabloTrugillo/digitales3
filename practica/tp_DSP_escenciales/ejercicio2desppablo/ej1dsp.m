t = 0:0.001:0.5;
F = 100;
snr = 15;
onda_senoidal = sin(2*pi*F*t);
%plot(t,onda_senoidal)
%con_ruido = my_awgn(onda_senoidal, snr); prototipo.
%con_ruido = onda_senoidal + 0.5*randn(size(t));  %generar una señal con ruido blanco gaussiano.
%con_ruido = awgn(onda_senoidal,snr);   %generar una señal con ruido blanco gaussiano.
vars = var(onda_senoidal);  %calculo de la varianza de la señal senoidal.
varn = vars/(10^(snr/10));  %calculo de la varianza de la señal con ruido a partir de la formula snr.

%con_ruido = onda_senoidal + sqrt(varn)*randn(size(t));

r1 = rmse(onda_senoidal,con_ruido)  %calculo del error cuadrático medio (RMSE, root mean squared error) entre la señal original y las señales con ruido.
plot(t,con_ruido)