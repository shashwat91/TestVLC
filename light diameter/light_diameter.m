%d = pixels(2,:);
%s_actual = pixels(1,:);
s_actual = [1760,1300,1080,780,676,588,548,368,280,234,184];
d=[30    50    60    70    80    90   100   150   200   250   300];

s_calc_e = zeros(1,length(s_actual));
s_calc_p1 = zeros(1,length(s_actual));
s_calc_p2 = zeros(1,length(s_actual));
err_calc_e = zeros(1,length(s_actual));
err_calc_p1 = zeros(1,length(s_actual));
err_calc_p2 = zeros(1,length(s_actual));

for i=1:length(d)
    s_calc_e(i) = floor(3098*exp(-0.0241*d(i)) + 309.7*exp(-0.0014*d(i)));
    s_calc_p1(i) = floor(30470 * d(i)^(-0.805) - 154.2);
    s_calc_p2(i) = floor(47060*d(i)^(-0.9525));
end

for i=1:length(d)
    err_calc_e(i) = s_calc_e(i) - s_actual(i);
    err_calc_p1(i) = s_calc_p1(i) - s_actual(i);
    err_calc_p2(i) = s_calc_p2(i) - s_actual(i);
end

figure(1)
hold on;
grid on
grid minor
plot(d,s_actual,'x-');
plot(d,s_calc_e,'x-');
plot(d,s_calc_p1,'x-');
plot(d,s_calc_p2,'x-');
hold off;
legend('Actual','Exponential fit','Power fit (ax^b+c)','Power fit (ax^b)');

figure(2)
hold on;
stem(d,err_calc_e);
stem(d,err_calc_p1);
stem(d,err_calc_p2);
hold off;
legend('Exponential fit','Power fit (ax^b+c)','Power fit (ax^b)');