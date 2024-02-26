%% Final TD3
clear
clc
close all
% EJERCICIO FINAL TECNICAS DIGITALES III
%
% Se desea diseñar un filtro IIR pasa-banda que rechace una señal de 50 Hz 
% y que deje pasar un tono de 1000 Hz.
%
% 1. Genere una señal de las menos 0.05 segundos que sea la suma de una señal 
% senoidal con frecuencia fundamental fn1 = 1000 Hz y una señal senoidal con 
% frecuencia fundamental fn2 = 50 Hz, esta última con un 50% de amplitud respecto 
% a la primera. La señal de 50 Hz se considera como una interferencia.
%% FRECUENCIAS.

fn1 = 1000; % Fundamental
fn2 = 50; % Interferencia
fs = 10000; % Frec. muestreo

t = 0:1/fs:0.05;

%% SE�AL DE ENTRADA. 

signal = sin(2*pi*fn1*t) + (0.5*sin(2*pi*fn2*t));

%% FILTRO IIR.
Hd = Final_TD3_filt;

SOS =  Hd.sosMatrix;
G = Hd.ScaleValues;

%% FILTRADO IIR

iir_output = filtfilt(SOS,G,signal);

%% GRAFICAS.

figure

title 'Dominio del tiempo'
plot(t,signal,'g',t,iir_output,'b');
legend('Se�al original','Se�al filtrada');
grid on
  
title 'Dominio Frecuencia'
[frec,mag] = my_dft(signal,fs);
[iir_frec,iir_mag] = my_dft(iir_output,fs);

figure

plot(frec,mag,'r',iir_frec,iir_mag,'b');
legend('Se�al original','Se�al filtrada');
grid on




