dist = [30,50,60,70,80,90,100,150,200,250,300];
pixels = [1760,1300,1180,780,676,588,548,368,280,234,184];
freq = 500:500:3000;
N_bits = zeros(length(freq),length(dist));
tr = 10.5*1e-6;

for f = 1:length(freq)
    for i = 1:length(pixels)
        N_bits(f,i) = floor(freq(f) * pixels(i) * tr);
    end
end

figure('Name','Number of symbols read at different distances');
hold on;

for i=1:length(dist)
    plot(freq, N_bits(:,i),'x-','DisplayName',num2str(dist(i)));
end
hold off;

title('Number of symbols in image VS frequency');
xlabel('Transmitting frequency (Hz)');
ylabel('Number of symbols in image');
lgd = legend('show');
lgd.FontSize = 15;
title(lgd,'Distance from light(cm)')