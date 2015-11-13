clear all
close all

%Download the files for today
%NOTE: This may cause the program not to get any new information !!!!!!
%date=datestr(now,'mm-dd');
     date='09-07';
     
datarecord_length = 26;
checksum_position = 24;
rawdata=[];%zeros(1,33);
valide_lines =0;
checksum_passed = 0;

%Flags at the begining and end of each data record
init_flag = '!';
final_flag = '\n';
flags = strcat(final_flag,init_flag);

%Start time - 
start_time = '4h and ';

%Frequencies and modulations arrays
mod_values = ['OFDM' 'QPS' 'BPSK' 'ASK'];
freq_values = [445 446 447 448];

% Download files
%Url for payload 5 data
url =  'http://laspace.lsu.edu/hasp/groups/2015/data/data.php?pname=Payload_05&py=2015';


try
    % Downloading bar
    wait=waitbar(0,'Downloading');
    html=urlread(url);
    
    
    [start_filename,ignoreThisOutput]=regexp(html,'\/sp5');
    [ignoreThisOutput,end_filename]=regexp(html,'raw>');
    
    
    
    % Wait bar 
    for i=1:length(start_filename)
        wait=waitbar(i/length(start_filename),wait,'Downloading');
        filenames=html(start_filename(i)+1:end_filename(i)-1);
        if size(findstr(filenames,date))>0
            
            urlwrite(strcat('http://laspace.lsu.edu/hasp/groups/2015/data/Payload_05/',filenames),filenames);
            
            
        end;
    end;
    delete(wait)
    
catch time_out
    fprintf(' ___________________________________________________\n');
    fprintf(' !!!! Internet not available, proccessing old data !!!! \n');
    fprintf(' ___________________________________________________\n');
    delete(wait)
    
end


%% Extract data from files

%Select all files in the directory .raw
files=dir('*.raw')

%Check validity of everyline
for i=1:length(files)
    wait=waitbar(i/length(files));
    
    %Read the file
    fprintf(files(i).name); fprintf('\n');
    fid = fopen(files(i).name);
    data = fread(fid);
    fclose(fid);
    
    %Find the starting point of every line
    [~,lines]=regexp(char(data)', flags);
    [~,modes]=regexp(char(data)','mode');
    
    for j=1:(length(lines)-1)
        line=data(lines(j):lines(j+1)-1);
        % Check that the line has the expecte length
        if (length(line)==datarecord_length)
            valide_lines=valide_lines+1;
            checksum=0;
            %Validate checksum:
            %Calculate checksum
            for k=1:datarecord_length-3
                checksum=bitxor(checksum,line(k));
%               fprintf('%i ',checksum);
            end
            
            %Compare with sent checksum
            if checksum==line(checksum_position)
                checksum_passed=checksum_passed+1;
                %Add line to the valid rawdata array
                rawdata=[rawdata; line'];
            end
        end
    end
end
close(wait);

%% Extract each value from datarecord
% Data record structure:
% <Head><STmp1>< STmp2>< SHdty>< SPressure>< SLight><SCamera> <SComm> < A1> <A2><fcom><Time><Checksum><Footer>
% 
%   Head = ! = 33
%   Time (4 samples) = [milliseconds]
%   Footer = \n
%
% Raw data:
%   all lines (columns)
%   data samples from 1 to end -2
rawdata=rawdata(:,2:end-2);

%Camera states: first two samples
%cam1on=(rawdata(:,1)==1);
%cam2on=(rawdata(:,2)==1);

%Debug - mock case with 
%   3 sensors


%Vmppt1=(rawdata(:,3) + bitshift(rawdata(:,4),8))*5.0/1023;
%Byte 1 of data - position 1 and 2
temp1 = (rawdata(:, 1) + bitshift(rawdata(:,2),8));
%Byte 2 of data - position 2 and 3
tempHum = (rawdata(:,3)+ bitshift(rawdata(:,4),8));
humidity = (rawdata(:,5)+ bitshift(rawdata(:,6),8));
pressure = (rawdata(:,7)+ bitshift(rawdata(:,8),8));
light = (rawdata(:,9)+ bitshift(rawdata(:,10),8));
camera = (rawdata(:,11)); % bitshift(rawdata(:,12),8));
tempState = rawdata(:, 12);
currentArduino = (rawdata(:,13)+ bitshift(rawdata(:,14),8));
currentVCO = (rawdata(:,15)+ bitshift(rawdata(:,16),8));

frequencies = rawdata(:,17);
vcoON = (rawdata(:,18));

time=rawdata(:,19)+bitshift(rawdata(:,20),8)+bitshift(rawdata(:,21),16)+bitshift(rawdata(:,22),24);

% Treat currentVCO values as signed bytes
for i=1:length(currentVCO)
    if currentVCO(i) > 127
        currentVCO(i) = currentVCO(i) - 256;
    end
end

%% Plot the results
%Plot time: in case there is a missing sample
figure;
plot(time);

%Plot ON/OFF and state values for VCO and Camera
figure;
subplot(2,2,1);
i = 1;

scatter(time/60000,(frequencies-188)/6 +446, 'b');
xlabel('time (minutes)');
legend('Frequency [MHz]');
title('VCO Frequency state');

subplot(2,2,2);
scatter(time/60000,vcoON, 'g');
xlabel('time [minutes]');
legend('ON/OFF VCO');
title('VCO Power state');


subplot(2,2,3);
scatter(time/60000,camera,'r'); 
xlabel('time [minutes]');
legend('ON/OFF Camera');
title('Camera state');

subplot(2,2,4);
scatter(time/60000,tempState,'y'); 
xlabel('time [minutes]');
legend('Cold -  Hot - normal temperature');
title('Temperature state');

%Plot sensor values
figure;
%Temperature
subplot(2,2,1);
i = 1;
for i =  1:length(temp1)
    if (temp1(i) == 0)
        temp1(i) = temp1(i-1);
    end
end
plot(time/1000,temp1 , 'r', time/1000,tempHum,'b' );
title('Temperature values');
xlabel('time [seconds]');
legend('temp1 [°C]','temp (humidity) [°C]');

%Humidity
subplot(2,2,2);
plot(time/1000, humidity,'m');
xlabel('time [seconds]');
legend('humidity [%]');
title ('Humidity values');

%Pressure
subplot(2,2,3);
plot(time/1000, pressure,'g');
legend('pressure [mbar]');
title ('Pressure values');

%Light
subplot(2,2,4);
plot(time/1000, light, 'y');
xlabel('time [seconds]');
legend('light');
title('Light sensor values');

%Compensate temp
% Temp1 makeup
for i=1:length(temp1);
    if( temp1(i) > 32767 )
        temp1(i) = temp1(i) - 65535; 
    end
end


% tempHum makeup
for i=1:length(tempHum);
    if( tempHum(i) > 32767 )
        tempHum(i) = tempHum(i) - 65535; 
    end
end
%Plot copensated temp values
figure;
plot(time/1000,temp1, 'r', time/1000,tempHum,'b' );
xlabel ('t [min]');
legend('temp1 comp','temp (humidity sensor) comp');
title('Temperature values');



%Compensate the pressure overflow
pressure_2msB = zeros(length(pressure),1);
pressure_total = zeros(length(pressure),1);

for i=2:length(pressure)
    pressure_2msB(i) = pressure_2msB(i-1);
    if( (pressure(i) < 50) && (pressure(i-1) > 200 ) )
        disp(pressure(i));
        pressure_2msB(i) = pressure_2msB(i-1)+1;
    elseif( (pressure(i) > 200) && (pressure(i-1) < 50 ) )
        disp(pressure(i));
        pressure_2msB(i) = pressure_2msB(i-1)-1;
    end
end

for i=2:length(pressure)
    pressure_2msB(i) = pressure(i);
    if( (pressure(i) < 50) && (pressure(i-1) > 2310 ) )
        disp(pressure(i));
        pressure_2msB(i) = pressure_2msB(i-1)+1;
    elseif( (pressure(i) > 2310) && (pressure_2msB(i-1) < 50 ) )
        disp(pressure(i));
        pressure_2msB(i) = 1;
    end
end

pressure_total = pressure + 256*pressure_2msB;
figure;
plot(pressure_2msB', 'y');
xlabel ('t[min]');
legend('Pressure comp [mbar]');
title('Pressure values');

%Compare current and values
%Current
figure;
subplot(2,1,1);
plot(time/60000,currentArduino, 'b', time/60000,currentVCO,'g' );
xlabel('time [min]');
legend('current Arduino [mA]', 'current VCO [mA]');
title('Comparison: current vs temperatue');
subplot(2,1,2);
plot (currentArduino, 'b');
xlabel('time [seconds]');
legend('current Arduino[mA]');

%Light
figure;
subplot(1,2, 1);
plot (time/3600000, light, 'y', time/3600000, tempHum, 'r');
xlabel('time [hours]');
legend('Light', 'Temp');
title('Comparison: light sensor vs temperature');

subplot(1,2, 2);
plot (time/60000, currentArduino, 'b', time/60000, currentVCO, 'g',time/60000, tempHum, 'r');
xlabel('time (seconds)');
legend('Arduino [mA]', 'VCO [mA]', 'Temp [°C]');
title('Comparison: current vs temperature');


figure;
plot (time/60000, light-40 + tempHum*89/50, 'y');
xlabel('time [seconds]');
legend('light');
title('Light sensor values compensated');




