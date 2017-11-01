close all;
freq = [3000,2500,2000,1500,1000,800,700,600,500];
dist = [30,50,70,100,150,200,250,300];
PDR = [1 0.995884 0.98 0.977 0.97 0.9 0 0;
       1 0.971098 0.96 0.96 0.95 0.97 0 0;
       0.980263 1 0.925925 0.93 0.9 0 0 0;
       1 0.989655 0.95 0 0 0 0 0;
       1 0 0 0 0 0 0 0;
       0 0 0 0 0 0 0 0];
marks=['+','o','*','x','<','square'];
    
figure('Name','Packet Reception Ratio for FSK Encoding');
hold on;
for i=1:6
    p = plot(dist, PDR(i,:),'-','DisplayName',num2str(freq(i)));
    p.Marker = marks(i);
end
hold off;

title('Packet Reception Ratio VS frequency (FSK Encoding, DataSize=8)');
xlabel('Distance from Light (cm)');
ylabel('Packet Reception Ratio');
lgd = legend('show');
lgd.FontSize = 10;
title(lgd,'Symbol frequency (Hz)')
ylim([-0.2 1.2])