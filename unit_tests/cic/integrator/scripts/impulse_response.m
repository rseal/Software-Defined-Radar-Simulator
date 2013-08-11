function impulse_response(ept)

   x   = load ( 'input.dat' )  ; 
   s1  = load ( 'out1.dat'  )  ; 
   s2  = load ( 'out2.dat'  )  ; 
   s3  = load ( 'out3.dat'  )  ; 
   s4  = load ( 'out4.dat'  )  ; 
   s5  = load ( 'out5.dat'  )  ; 
   s6  = load ( 'out6.dat'  )  ; 
   s7  = load ( 'out7.dat'  )  ; 
   s8  = load ( 'out8.dat'  )  ; 
   s9  = load ( 'out9.dat'  )  ; 
   s10 = load ( 'out10.dat' )  ; 
   s11 = load ( 'out11.dat' )  ; 
  
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  

   h = figure();

   subplot(211);
   stem(x(1:ept),'k');
   title('input signal')
   grid on;

   subplot(212);
   stem(s1(1:ept),'k');
   title('stage 1 Integrator output')
   grid on;

   FN = findall(h,'-property','FontName');
   set(FN,'FontName','/usr/share/fonts/dejavu/DejaVuSerifCondensed.ttf');
   FS = findall(h,'-property','FontSize');
   set(FS,'FontSize',8);

   print plot_0.pdf -dpdf
   
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  

   h = figure();

   subplot(211);
   stem(s2(1:ept),'k');
   title('stage 2 Integrator output')
   grid on;
   
   subplot(212);  
   stem(s3(1:ept),'k');
   title('stage 3 Integrator output')
   grid on;
   
   FN = findall(h,'-property','FontName');
   set(FN,'FontName','/usr/share/fonts/dejavu/DejaVuSerifCondensed.ttf');
   FS = findall(h,'-property','FontSize');
   set(FS,'FontSize',8);

   print plot_1.pdf -dpdf

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  

   h = figure();

   subplot(211);
   stem(s4(1:ept),'k');
   title('stage 4 Integrator output')
   grid on;

   subplot(212);
   stem(s5(1:ept),'k');
   title('stage 5 Integrator output')
   grid on;

   FN = findall(h,'-property','FontName');
   set(FN,'FontName','/usr/share/fonts/dejavu/DejaVuSerifCondensed.ttf');
   FS = findall(h,'-property','FontSize');
   set(FS,'FontSize',8);

   print plot_2.pdf -dpdf
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  

   h = figure();

   subplot(211);
   stem(s6(1:ept),'k');
   title('stage 1 Differentiator output')
   grid on;
   
   subplot(212);  
   stem(s7(1:ept),'k');
   title('stage 2 Differentiator output')
   grid on;
   
   FN = findall(h,'-property','FontName');
   set(FN,'FontName','/usr/share/fonts/dejavu/DejaVuSerifCondensed.ttf');
   FS = findall(h,'-property','FontSize');
   set(FS,'FontSize',8);

   print plot_3.pdf -dpdf

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  

   h = figure();

   subplot(211);
   stem(s8(1:ept),'k');
   title('stage 3 Differentiator output')
   grid on;

   subplot(212);
   stem(s9(1:ept),'k');
   title('stage 4 Differentiator output')
   grid on;

   FN = findall(h,'-property','FontName');
   set(FN,'FontName','/usr/share/fonts/dejavu/DejaVuSerifCondensed.ttf');
   FS = findall(h,'-property','FontSize');
   set(FS,'FontSize',8);

   print plot_4.pdf -dpdf

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  

   h = figure();

   subplot(211);
   stem(s10(1:ept),'k');
   title('stage 5 Differentiator output')
   grid on;

   subplot(212);
   stem(s11(1:ept),'k');
   title('Output Signal')
   grid on;

   FN = findall(h,'-property','FontName');
   set(FN,'FontName','/usr/share/fonts/dejavu/DejaVuSerifCondensed.ttf');
   FS = findall(h,'-property','FontSize');
   set(FS,'FontSize',8);

   print plot_5.pdf -dpdf


end
