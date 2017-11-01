vec_high = zeros(1,3);
vec_low = zeros(1,3);
for i=0:7
    for j=1:50
        path = strcat('../Opencv_imagpros/data_images/',num2str(i),'/',num2str(i),'_',num2str(j),'_small_circle.jpg');
        img = imread(path);
        [vec_high, vec_low] = classifier_func(vec_high,vec_low,img);
    end
end