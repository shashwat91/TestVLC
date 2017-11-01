close all;

img = imread('../Opencv_imagpros/data_images/2/2_7_small_circle.jpg');
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

%% Classifing vec_low and vec_high in symbol widths
for i=1:length(vec_high)
    if vec_high(i,1) >= mean(vec_high(:,1)) +3
        vec_high(i,3) = 2;
    else
        vec_high(i,3) = 1;
    end
end

for i=1:length(vec_low)
    if (vec_low(i,1) >= mean(vec_low(:,1)) -3) && (vec_low(i,1) < mean(vec_low(:,1)) +3)
        vec_low(i,3) = 2;
    elseif vec_low(i,1) < mean(vec_low(:,1))
        vec_low(i,3) = 1;
    else
        vec_low(i,3) = 3;
    end
end

%% plotting raw signal with th lines
figure('Name','Raw_signal with thresholds');
y=1:r:r+1; 
hold on
plot(values_n,'k-x');
plot(y, thValue_n*ones(1, length(y)), 'r--')
hold off;

%% plotting distribution of widths
figure(2)
hold on;
histogram(vec_high(:,1));
histogram(vec_low(:,1));
legend('High','Low');
hold off;