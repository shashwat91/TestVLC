clear s;

ports = seriallist;
s = serial('/dev/ttyUSB0');
set(s,'BaudRate',115200,'Name','VLC-serial');
set(s,'Parity','none');
set(s,'StopBits',1);
set(s,'Terminator','LF');
set(s,'Timeout',1);

try
    fopen(s);
catch
    fprintf('Can not open Serial port\n');
    return;
end
pause(6);

count=0;
while count < 10
    try
        fprintf(s,'A');
        fprintf(strcat(num2str(count),'\n'));
    catch
        fprintf('Error in writing on Serial port\n');
    end
    count = count+1;
    pause(0.5);
end

fclose(s);