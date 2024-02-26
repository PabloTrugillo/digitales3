% 
% EJERCICIO FINAL TECNICAS DIGITALES III
%
% Se desea diseñar un filtro IIR pasa-banda que rechace una señal de 50 Hz 
% y que deje pasar un tono de 1000 Hz.
%
% 1. Genere una señal de las menos 0.05 segundos que sea la suma de una señal 
% senoidal con frecuencia fundamental fn1 = 1000 Hz y una señal senoidal con 
% frecuencia fundamental fn2 = 50 Hz, esta última con un 50% de amplitud respecto 
% a la primera. La señal de 50 Hz se considera como una interferencia. 
% 
% 2. Elija una frecuencia de muestreo adecuada.
% 
% 3. Use la herramienta filterDesigner para diseñar un filtro IIR pasa-banda 
% con frecuencias de corte de 1000 Hz y 3000 Hz tipo Butterworth, orden 6.
% 
% 4. Utilice la funcion filtfilt() para procesar la señal de interés con el 
% filtro IIR diseñado.
% 
% 5. Grafique la señal de entrada al filtro y la señal de salida del filtro, 
% ambas en el dominio del tiempo y de la frencuencia.

clc
clear
close all

%% FRECUENCIAS

fs  = 10000;            % Sampling freq.
fn1 = 1000;
fn2 = 50;  
 

%% SENAL DE ENTRADA


%% FILTRO IIR


%% FILTRADO IIR

iir_output = filtfilt(SOS, G, signal);

%% GRAFICAS



