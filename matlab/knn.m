X = 4 * rand(100,1);    % Use all data for fitting
Y = zeros(100,1); % Response data

for i=1:100
    if(X(i) < 2)
        Y(i) = '1';
    elseif (X(i) >= 2 && X(i) < 3)
        Y(i) = '2';
    else
      Y(i) = '3';
    end
end
    
Mdl = fitcknn(X,Y,'NumNeighbors',4);

% c1=1;
% c2=1;
% c3=1;
% for i=1:100
%     if(Y(i) == '1')
%         X1(c1) = X(i);
%         c1 = c1 + 1';
%     elseif Y(i) == '2'
%         X2(c2) = X(i);
%         c2 = c2 + 1;
%     else
%         X3(c3) = X(i);
%         c3 = c3 + 1;
%     end
% end

flwr = 2.1;
flwrClass = predict(Mdl,flwr);