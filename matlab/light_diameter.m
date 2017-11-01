d = pixels(1,:);
s_actual = pixels(2,:);
s_calc_e = zeros(1,length(s_actual));
s_calc_p = zeros(1,length(s_actual));

for i=1:length(d)
    s_calc_e(i) = 666.2*exp(-0.0065*d(i)) + 114.4*exp(-0.00066*d(i));
    s_calc_p(i) = 74300 * d(i)^(-1.058) + 8.074;
end

hold on;
plot(s_actual,d,'x-');
plot(s_calc_e,d,'x-');
plot(s_calc_p,d,'x-');
hold off;
legend('Actual','Exponential fit','Power fit'); 