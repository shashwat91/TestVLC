close all;
clear all;
img = imread('../Opencv_imagpros/data_images/4/4_3_small_circle.jpg');
f=1000;
tr = 10.5*1e-6;
[r,c] = size(img);
values = double(img(:,floor(c/2)));
values_n = values/max(values);
%figure(1);
%plot(values,'x-');

th_sig=zeros(1,r);
for i=1:r
    if(values_n(i)>0.6)
        th_sig(i)=1;
    else
        th_sig(i)=0;
    end
end

%figure(2)
%plot(th_sig);

figure(3)
ax1 = subplot(2,1,1)
plot(ax1,values,'x-');
xlabel(ax1, 'Sample number');
ylabel(ax1, 'Pixel value');
ax2 = subplot(2,1,2)
plot(ax2,th_sig);
ylim(ax2,[-0.2 1.2])
N_bits = r*f*tr