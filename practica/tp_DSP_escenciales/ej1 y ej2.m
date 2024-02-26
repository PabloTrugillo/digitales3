

 Ft= 100;
 Fn=1000;
 T=1/Fn;
  %creacion de la se�al continua
 t= (0:T:0.5);
 xt=sin(2*pi*Ft*t);%creacion de la senoidal
 %Hasta aca el ej 1, para abajo ej2
 %---------------------------------
 snr = 30; %relacion se�al - ruido
 signal_n = my_awgn(xt,snr)
 signalruido = awgn(xt,snr); %usamos la funcion awgn
%figure;
 plot(t,xt, '--r'); %graficamos la senoidal color rojo
 hold on;
 plot(t, signalruido,'-b');%graficamos la se�al + ruido color azul
 hold on;
plot(t, signal_n,'-og'); %graficamos la se�al + ruido de my_awgn color verdes con circulos
plot(t,xt, t, signalruido, t, signal_n);
