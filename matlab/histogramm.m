img = imread('test2.jpg');
[r,c] = size(img);
values = img(:,floor(c/2));
[val1,pos2] = max(img(:));
[val2,pos2] = min(img(:));
avgVal1 = (val1+val2) / 2 %avg by max and min;

thValues = zeros(1,r);
for i=1:(r)
    if values(i) > avgVal1
            thValues(i) = 255;
        else
            thValues(i) = 0;
        end
    
end

% imwrite(thValues, 'line2.jpg');
hold on
yyaxis right
ylim([0 500])
plot(thValues,'r')
yyaxis left
plot(values,'b')
plot(y, avgVal2*ones(size(y)), 'g-')
hold off