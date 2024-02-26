%% =================================================================
%% LOWPASS A
%% 
%% Filtro pasabajos con frecuencia de corte en pi/2
%% Disenio usando las N muestras 
%% =================================================================

clear all ;

% Parametros del filtro

N  = 81 ;
wc=pi/2.0;
M  = (N-1)/2 ;
W0 = 2*pi/N ;

% Defino las N muestras del filtro en frecuencia a partir de 
% la respuesta teorica buscada:

for k = 1:N
    Wk = W0*(k-1) ;
    if Wk < wc
       H(k) = 1;
    elseif Wk > (2.0*pi-wc)
       H(k) = 1 ;
    else 
       H(k) = 0 ;
    end
end

% Calculo el operador en tiempo:

h = real(ifft(H)) ;

% Lo hago de fase cero teniendo en cuenta que es periodico

h = [h(M+2:N) h(1:M+1)];

% Realizo el zero padding para tener la respuesta 
% en frecuencia bien muestreada:

h2     = h ;
NN     = 1024 ;
h2(NN) = 0 ;

% Calculo la espectro de amplitud del filtro obtenido:

H2 = abs(fft(h2)) ;

% Genero la respuesta teórica del filtro para comparar:

W02 = 2*pi/NN ;
for k = 1:NN
    Wk = W02*(k-1) ;
    if Wk < wc
       Ht(k) = 1 ;
    elseif Wk > (2.0*pi-wc)
       Ht(k) = 1 ;
    else 
       Ht(k) = 0 ;
    end
end

% Grafico el filtro en tiempo:

subplot(2,1,1);
stem((-M:M),h,'b'); hold on
plot((-M:M),h,';filtro en tiempo;o'); hold off
xlabel('n')
ylabel('h_n')
axis([-M-1 M+1 -0.3 0.7]);

% Grafico filtro en frecuencia para ver qué 
% tanto me aparto del pasa-bajos teórico:

subplot(2,1,2);
plot(W0*(0:(N-1)),H,';muestras originales;bo',\
     W02*(0:(NN-1)),H2,';espectro real;r-',\
     W02*(0:(NN-1)),Ht,';espectro teorico;g-') ;
legend("location","north")
xlabel('{/Symbol w}')
ylabel('|H{/Symbol w}|')
xt      = 0:pi/2:2*pi;
xtlabel = ["0";"{/Symbol p}/2";"{/Symbol p}";"3/2{/Symbol p}";"{/Symbol 2p}"];
set(gca,"xtick",xt,"xticklabel",xtlabel);
set(gca,"ytick",[0 1]);
axis([0,2*pi,-0.2,1.2]);

% Guardo un .eps con el gráfico:

print('lowpass_a.eps','-depsc');
