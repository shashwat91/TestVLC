close all;
clear;
img = imread('2_circle.jpg');
[r,c] = size(img);
values = img(:,floor(c/2));
[val1,pos2] = max(values);
[val2,pos2] = min(values);
avgVal1 = floor((val1+val2) / 2) %avg by max and min;
avgVal2 = floor(mean(values)) %overall average of image

thValues1 = zeros(1,r);
thValues2 = zeros(1,r);
for i=1:(r)
    %%% Calculating th based on max-min value
    if values(i) > avgVal1
        thValues1(i) = 255;
    else
        thValues1(i) = 0;
    end
    %%% Calculating th based on overall mean value
    if values(i) > avgVal2
        thValues2(i) = 100;
    else
        thValues2(i) = 0;
    end
    
end

%% Calculating width of each stripe
count1=0;
count2=0;
last1=0;
last2=0;
for i=2:r
    if thValues1(i) ~= thValues1(i-1)
        if last1 ~= 0
            count1 = count1 + 1;
            width(1,count1) = i-last1;
            last1=i;
        else
            last1=i;
        end
    end
    
    if thValues2(i) ~= thValues2(i-1)
        if last2 ~= 0
            count2 = count2 + 1;
            width(2,count2) = i-last2;
            last2=i;
        else
            last2=i;
        end
    end
end
%% Saving both threshold line images
% imwrite(thValues1, '1_line1.jpg');
% imwrite(thValues2, '1_line2.jpg');

%% plotting raw signal with th lines
figure('Name','Raw_signal with thresholds');
y=1:r:r+1; 
hold on
plot(values,'k-x')
plot(y, avgVal2*ones(1, length(y)), 'r--')
plot(y,avgVal1 * uint8(ones(1,length(y))), 'r-.')
legend('Raw values','Threshold2(mean)','Threshold1(max-min)');
hold off

%% plotting both threshold signals
% figure('Name','Thresholded signal output');
% hold on;
% %xlim([10 150])
% plot(thValues1,'r');
% plot(thValues2,'b');
% legend('Threshold1(max-min)','Threshold2(mean)');
% hold off;

%% Plotting histogram to see variation of widths
figure('Name','Histogram Thresholded1(max-min)');
histogram(width(1,:))
figure('Name','Histogram Thresholded2(mean)');
histogram(width(2,:))
