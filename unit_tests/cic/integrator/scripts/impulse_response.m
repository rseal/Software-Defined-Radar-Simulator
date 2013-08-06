function impulse_response(ept)

   x = load('input.dat');
   x0 = load('out0.dat');
   x1 = load('out1.dat');
   x2 = load('out2.dat');
   x3 = load('out3.dat');
   
   h = figure();

   subplot(211);
   stem(x(1:ept),'k');
   title('input signal')
   grid on;

   subplot(212);
   stem(x0(1:ept),'k');
   title('stage 0 Integrator output')
   grid on;

   FN = findall(h,'-property','FontName');
   set(FN,'FontName','/usr/share/fonts/dejavu/DejaVuSerifCondensed.ttf');
   FS = findall(h,'-property','FontSize');
   set(FS,'FontSize',8);

   print plot_0.pdf -dpdf
   
   h = figure();

   subplot(211);
   stem(x1(1:ept),'k');
   title('stage 1 Integrator output')
   grid on;
   
   subplot(212);  
   stem(x2(1:ept),'k');
   title('stage 2 Differentiator output')
   grid on;
   
   FN = findall(h,'-property','FontName');
   set(FN,'FontName','/usr/share/fonts/dejavu/DejaVuSerifCondensed.ttf');
   FS = findall(h,'-property','FontSize');
   set(FS,'FontSize',8);

   print plot_1.pdf -dpdf

   h = figure();

   subplot(211);
   stem(x3(1:ept),'k');
   title('stage 3 Differentiator output')
   grid on;

   FN = findall(h,'-property','FontName');
   set(FN,'FontName','/usr/share/fonts/dejavu/DejaVuSerifCondensed.ttf');
   FS = findall(h,'-property','FontSize');
   set(FS,'FontSize',8);

   
   print plot_2.pdf -dpdf

end
