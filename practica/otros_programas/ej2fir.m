function ex_02
clear all;
close all;



% a)Cargue el archivo de audio provisto llamado Tchaikovsky.mat. En el mismo
% encontrará dos variables, la matriz signal con dos canales (stereo) y la variable Fs. Elija 1 de
% los 2 canales disponibles.


load ('Tchaikovsky.mat'); %Cargo datos, ver Workspace
r=signal(:,1);%signal es una matriz
%r es vector columna del canal derecho
r=r';%r es vector fila

% b)Agregue ruido gaussiano a esta señal tal que la relación señal-ruido entre la señal y
% la señal con ruido sea de 50 dB.

SNR=50;
ruido=awgn(r,SNR);

% c) Calcule el valor máximo de N (N_max), con las frecuencias fs = Fs y fco = 11.025Hz.

%Fs ya viene declarada en el archivo Tchaikovsky.mat

fco = 11025; %Frecuencia de corte
Nmax=round(sqrt((((0.885894^2)*(Fs^2))/((fco)^2))-1)) %Fs viene de signal

% d) Aplique filtrado del tipo moving average a la señal con ruido para un filtro MA con
% dimensión igual N = N_max. Utilice la función filter().

windowSize = Nmax;
b = (1/windowSize)*ones(1,windowSize);
a = 1; 
sf = filter(b,a,ruido);

% e) Utilice la función sound(signal_n, Fs) para reproducir las señales sin ruido, con
% ruido y filtrada

%sound(r, Fs); %Senial sin ruido
sound(ruido, Fs); %Senial con ruido

% f) Grafique la respuesta en frecuencia de las señales original y filtrada y compare.
% Utilice la función provista my_dft.

figure (2);
freqz(sf,1)

[frq, mag] = my_dft(r, Fs);
[frq_n, mag_n] = my_dft(ruido, Fs);
[frq_ma, mag_ma] = my_dft(sf, Fs);

figure (3);

plot(frq, mag, '-b')
hold on
plot(frq_n, mag_n, '.-r')
plot(frq_ma, mag_ma, '--g')
hold off

grid on

legend('Signal original', 'Con ruido', 'Filtrada')

end