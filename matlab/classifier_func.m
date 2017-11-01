function [vec_high, vec_low] = classifier_func(vec_high,vec_low,img)
    [r,c] = size(img);
    values = double(img(:,floor(c/2)));
    max_val = max(values);
    min_val = min(values);
    avgValue = double((max_val+min_val) / 2); %avg by max and min;

	start_high = length(vec_high);
    start_low = length(vec_low);
    %% Calculating normalised values
    values_n = values/max_val; % Normalising the amplitude
    thValue_n = avgValue/max_val;
    
    %% Calculating width of each strip
    count_low=start_high;
    count_high=start_low;
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
    for i=start_high+1:length(vec_high)
        if vec_high(i,1) >= mean(vec_high(start_high+1:length(vec_high),1)) +3
            vec_high(i,3) = 2;
        else
            vec_high(i,3) = 1;
        end
    end

    for i=start_low+1:length(vec_low)
        if (vec_low(i,1) >= mean(vec_low(start_low+1:length(vec_low),1)) -3) && (vec_low(i,1) < mean(vec_low(start_low+1:length(vec_low),1)) +3)
            vec_low(i,3) = 2;
        elseif vec_low(i,1) < mean(vec_low(start_low+1:length(vec_low),1))
            vec_low(i,3) = 1;
        else
            vec_low(i,3) = 3;
        end
    end
    clear values;
    clear values_n;
end