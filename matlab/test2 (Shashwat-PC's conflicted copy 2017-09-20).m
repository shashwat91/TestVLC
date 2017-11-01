%% Seeing rise and fall of received signals 
f=500:100:6000;
markers = ['x','o','+','*','.','s','^','v','>','<','p','h','d'];
figure(2)
hold on;
m=1;
for itteration= 36
    %plot(samples(:,itteration),'-','Marker',markers(m),'DisplayName',num2str(f(itteration)));
    plot(samples(:,itteration),'x-','DisplayName',num2str(f(itteration)));
    ylim([-0.1 1.1]);
    title('Calculated symbols using Rolling Shutter effect');
    xlabel('Pixel Number (sample number)');
    ylabel('Signal Intensity');
    lgd = legend('show');
    lgd.FontSize = 15;
    title(lgd,'Frequency (Hz)')
    m=m+1;
end
grid on;
hold off;

%% Calculating number of samples when signal falls or rises

% count=zeros(56,15);
% for itteration= 1:1:56
%     num=1;
%     for i = 2:rows
%         if(samples(i-1,itteration) ~= samples(i,itteration))
%             count(itteration,num)=count(itteration,num)+1;
%         elseif((samples(i-1,itteration) == samples(i,itteration)) && count(itteration,num)~=0)
%             num=num+1;
%         end
%     end
% end
% 
% num=1;
% clear count2;
% for i=1:56
%     for j=1:15
%         if(count(i,j) ~= 0 && count(i,j) < 100)
%             count2(num) = count(i,j);
%             num=num+1;
%         end
%     end
% end
%     
% histogram(count2);