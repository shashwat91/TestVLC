close all;

clear('vec_high','vec_low');
img = imread('../Opencv_imagpros/data_images/5/5_51_small_circle.jpg');
[r,c] = size(img);
values = double(img(:,floor(c/2)));
max_val = max(values);
min_val = min(values);
avgValue = double((max_val+min_val) / 2); %avg by max and min;

values_n = values/max_val; % Normalising the amplitude
thValue_n = avgValue/max_val;

% Variables for classification data
% vec_high -> for 1/11
% vec_low -> for 0/00/000
%% Calculating width of each strip
count_low=0;
count_high=0;
last=0;
for i=2:r
    if  values_n(i-1) < thValue_n && values_n(i) >= thValue_n
        if(last ~= 0)
            count_low = count_low + 1;
            vec_low(count_low,1) = i-last;
            vec_low(count_low,2) = min(values_n(last:i));
        end
        last = i;
    elseif values_n(i-1) >= thValue_n && values_n(i) < thValue_n
        if(last ~= 0)
            count_high = count_high + 1;
            vec_high(count_high,1) = i-last;
            vec_high(count_high,2) = max(values_n(last:i));
        end
        last = i;
    end
end

%% plotting raw signal with th lines
figure('Name','Raw_signal with thresholds');
y=1:r:r+1; 
hold on
plot(values_n,'k-x');
plot(y, thValue_n*ones(1, length(y)), 'r--')
hold off;

clear('img','r','c','','max_val','min_val','avgValue','values_n');
clear('thValue_n','count_low','count_high','last','i');