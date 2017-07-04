% for i=1:size(vec_high1,1)
%     if(vec_high1(i,1) == 0)
%         vec_high1(i,:) = [];
%     end
% end

one_count=0;
two_count=0;
three_count=0;
for i=1:length(vec_low1)
    if vec_low1(i,3) == 1
        one_count = one_count + 1;
        low_one(one_count,1)=vec_low1(i,1);
        low_one(one_count,2)=i;
    elseif vec_low1(i,3) == 2
        two_count = two_count + 1;
        low_two(two_count,1)=vec_low1(i,1);
        low_two(two_count,2)=i;
    else
        three_count = three_count + 1;
        low_three(three_count,1)=vec_low1(i,1);
        low_three(three_count,2)=i;
    end
end

% for i=1:length(vec_low1)
%     if vec_low1(i,1) > 25
%         vec_low1(i,3) = 3;
%     elseif vec_low1(i,1) > 15
%         vec_low1(i,3) = 2;
%     else
%         vec_low1(i,3) = 1;
%     end
% end