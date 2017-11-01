close all;
clear;
img = imread('1_circle.jpg');
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

figure(1)
y=1:r:r+1;
hold on
plot(values,'kx-')
plot(y, avgVal2*ones(1, length(y)), 'r--')
plot(y,avgVal1 * uint8(ones(1,length(y))), 'b-.')
legend('Raw values','mean threshold','max-min threshold');
hold off

figure(2)
hold on;
xlim([10 150])
plot(thValues1);
plot(thValues2);
hold off;
