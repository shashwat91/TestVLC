freq = [3000,2500,2000,1500,1000,800,700,600,500];
dist = [30,50,70,100,150,200,250,300];
PDR = [1 0.995884 0.98 0 0 0 0 0;
       1 0.971098 0.96 0 0 0 0 0;
       0.980263 1 0.925925 0 0 0 0 0;
       1 0.689655 0 0 0 0 0 0;
       1 0 0 0 0 0 0 0;
       0 0 0 0 0 0 0 0];
marks=['+','o','*','x','<','square'];
    
figure('Name','Packet Delivery Ratio for Manchester Encoding (data size=8)');
hold on;
for i=1:6
    p = plot(dist, PDR(i,:),'-','DisplayName',num2str(freq(i)));
    p.Marker = marks(i);
end
hold off;

title('Packet Delivery Ratio VS frequency (Manchester Encoding, DataSize=8, symbols=23)');
xlabel('Distance from Light (cm)');
ylabel('Packet Delivery Ratio');
lgd = legend('show');
lgd.FontSize = 12;
title(lgd,'Symbol frequency (Hz)')
ylim([-0.2 1.2])d