% freq = [3000,2500,2000,1500,1000,800,700,600,500];
% dist = [30,50,70,100,150,200,250,300];
% PDR = [1 0.995884 0.98 0 0 0 0 0;
%        1 0.971098 0.96 0 0 0 0 0;
%        0.980263 1 0.925925 0 0 0 0 0;
%        1 0.689655 0 0 0 0 0 0;
%        1 0 0 0 0 0 0 0];
% marks=['+','o','*','x','square'];
%     
% figure('Name','Packet Delivery Ratio for Manchester Encoding (data size=8)');
% hold on;
% for i=1:5
%     p = plot(dist, PDR(i,:),'-','DisplayName',num2str(freq(i)));
%     p.Marker = marks(i);
% end
% hold off;
% 
% title('Packet Delivery Ratio VS frequency (Manchester Encoding, DataSize=8)');
% xlabel('Distance from Light (cm)');
% ylabel('Packet Delivery Ratio');
% lgd = legend('show');
% lgd.FontSize = 12;
% title(lgd,'Symbol frequency (Hz)')
% ylim([-0.2 1.2])

t= 0:0.01:30;
% d_cycle = 50;
% c_val = 0;
% val = zeros(1,length(t));
% for i=1:length(t)
%     if(mod(i,100) == 0)
%         if(c_val == 0)
%             c_val = 1;
%         else
%             c_val = 0;
%         end
%         val(i) = c_val;
%     else
%         val(i) = c_val;
%     end
% end

% s = square(t,60);
% for i=1:length(s)
%     if(s(i)<0)
%         s(i) = 0;
%     end
% end
% 
% plot(t,s)
% ylim([-0.2 1.2])

f = [1,2,5,10,20,30,40,50,60,70,80,90,100,200,500,1000];
s = [11,15,18,32,45,52,52,63,70,65,65,54,51,63,50,45];
plot(f,s,'xb-')

title('Samples acquired (per second) vs Transmittor frequency');
xlabel('Bit Rate (bps)');
ylabel('Sample (per second)');
legend('Samples (per second)');