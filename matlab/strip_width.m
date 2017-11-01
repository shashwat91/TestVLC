% close all;
% clear all;
% img = imread('../Opencv_imagpros/log_30_6/50_1000_7_circle.jpg');
% f=1000;
% tr = 10.5*1e-6;
% [r,c] = size(img);
% values = double(img(:,floor(c/2)));
% values_n = values/max(values);
% % figure(1);
% % plot(values_n,'x-');
% 
% th_sig=zeros(1,r);
% for i=1:r
%     if(values_n(i)>0.5)
%         th_sig(i)=1;
%     else
%         th_sig(i)=0;
%     end
% end
% 
% figure(2)
% plot(th_sig,'x-');
% 
% N_bits = r*f*tr
close all;
clear all;
f=500:100:5000;
tr = [18 14.1 10.5 8 6 5]*1e-6;

W = zeros(length(f),length(tr));
hold on;
for i=1:length(f)
    for j=1:length(tr)
        W(i,j) = floor(1/(f(i)*tr(j)));
    end
end

for i=1:length(tr)
    p = plot(f, W(:,i),'-');
end



title('Strip Widths for Frequency vs Exposure time');
xlabel('Frequency (Hz)');
ylabel('Strip Width');
lgd = legend('show');
% lgd.FontSize = 12;
title(lgd,'Exposure time (Sec)')
% ylim([-0.2 1.2])
    
hold off;