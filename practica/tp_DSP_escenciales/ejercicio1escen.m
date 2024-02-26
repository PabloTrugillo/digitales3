
fx = 100; %Frecuencia de mi Señal
fs1 = 1000; %Frecuencia de muestreo ... 10 puntos definidos 
fs2 = 10000; % Frecuencia de muestreo ... 100 puntos definidos
fs3 = 200; % Frecuencia de muestreo ... 2 puntos definidos
dt1 = 1/fs1;  %Distancia entre puntos
dt2 = 1/fs2;  %Distancia entre puntos
dt3 = 1/fs3;  %Distancia entre puntos

xt1 = 0 : dt1 : 0.02 ; % Definicion de los limites sobre el eje X y los saltos
xt2 = 0 : dt2 : 0.02 ; % Definicion de los limites sobre el eje X y los saltos
xt3 = 0 : dt3 : 0.02 ; % Definicion de los limites sobre el eje X y los saltos
% 0.01 es el periodo de mi señal 
y1 = sin(2*pi*fx*xt1) % Funcion sin pura
y2 = sin(2*pi*fx*xt2) % Funcion sin pura
y3 = sin(2*pi*fx*xt3 + pi/2) % Funcion sin pura desfasamos 90 grados porq si no todos los puntos caerian en cero
y4 = sin(2*pi*fx*xt3 ) % Vemos valores cercanos a cero 
figure %Crea una nueva ventana
subplot(4,1,1);plot (xt1,y1,'-+');%Grafica de los valores xt e y
subplot(4,1,2);plot (xt2,y2,'--+')
subplot(4,1,3);plot (xt3,y3,':+')
subplot(4,1,4);plot (xt3,y4,'-+')