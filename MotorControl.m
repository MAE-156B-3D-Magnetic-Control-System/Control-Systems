% clear
% clc

%supress the next line after running once
a=arduino('com8','mega2560'); % or ('com3','uno') for arduino uno


%% 
maxcurrent=9.75;
inputvalue=9.75; %this will be the slider value
count=0; %can be removed in GUI
ammeter=readVoltage(a,'A0'); %if changing the current direction changes the sign of ammeter just take absolute value
% value=1;
Kp=.025;
KpGain=1+Kp;
KpLoss=1-Kp;
if exist('previousvalue','var')==1 
    if (previousvalue==inputvalue) %won't go here unless it's an exact match; could make a new variable that is defined at the beginning of the loop
        writePWMDutyCycle(a, 'D3',value/10);
        disp('fruit')
    elseif (previousvalue<=inputvalue) %PWM needs to increase
        value=previousvalue;
        while (inputvalue<=maxcurrent)&&(ammeter<=inputvalue)
            ammeter=readVoltage(a,'A0'); %this will need to be a separate function
            if (ammeter>=maxcurrent)||(value>=maxcurrent)||(value>=inputvalue) % last two comparators will be unecessary when ammeter is included; need to inclue (ammeter>inputvalue)
                previousvalue=value;
                break
            else
                value=value*KpGain;
                writePWMDutyCycle(a, 'D3',value/10);
                pause(0.1)
                count=count+1; %can be removed in GUI
                disp('apple') %can be removed in GUI
            end
        end
    elseif (previousvalue>inputvalue) %PWM needs to decrease
        value=previousvalue;
        while (inputvalue<maxcurrent)&&(ammeter<inputvalue)
            ammeter=readVoltage(a,'A0'); %this will need to be a separate function
            if (ammeter>maxcurrent)||(value<=inputvalue) %last comparator will be unecessary when ammeter is working; need to include (ammeter<inputvalue) when ammeter works
                previousvalue=value;
                break
            else
                value=value*KpLoss;
                writePWMDutyCycle(a, 'D3',value/10);
                pause(0.1)
                count=count+1; %can be removed in GUI
                disp('orange') %can be removed in GUI
            end
        end
    elseif (inputvalue>maxvalue)
        %writePWMvalue(max value) % need to compute max value
    end
        
elseif exist('previousvalue','var')==0
    if inputvalue>1
        value=1;
        while (inputvalue<=maxcurrent)&&(ammeter<=inputvalue)
            ammeter=readVoltage(a,'A0'); %this will need to be a separate function
            if (ammeter>=maxcurrent)||(ammeter>=inputvalue)||(value>=maxcurrent)||(value>=inputvalue) % last two comparators will be unecessary when ammeter is included
                previousvalue=value;
                break
            else
                value=value*KpGain;
                writePWMDutyCycle(a, 'D3',value/10);
                pause(0.1)
                count=count+1; %can be removed in GUI
                disp('berry')  %can be removed in GUI
            end
        end
    elseif inputvalue<=1
        value=0.1;
        while (ammeter<=inputvalue)
            ammeter=readVoltage(a,'A0'); %this will need to be a separate function
            if (ammeter>=inputvalue)||(value>=inputvalue)
                previousvalue=value;
                break
            else
                value=value*KpGain;
                writePWMDutyCycle(a, 'D3',value/10);
                pause(0.1)
                count=count+1; %can be removed in GUI
                disp('pear')   %can be removed in GUI
            end
        end
    end
end

                
        
    
disp('done')    



                

%% 



%%
% inputvalue=9;
% if inputvalue>3
%     writePWMDutyCycle(a, 'D3',3/10);
%     value=3;
%     while (value<9.8)&&(value<inputvalue)
%         value=value*1.01;
%         writePWMDutyCycle(a, 'D3',value/10);
%         pause(0.1)
%         if (value>9.8)||(value>inputvalue)
%             break
%         end
%     end
% end


%% this stuff kind of works
% while (inputvalue<maxcurrent)&&(ammeter<inputvalue)
%     
%     ammeter=readVoltage(a,'A0'); %this will need to be a separate function
%     if (ammeter>maxcurrent)||(ammeter>inputvalue)||(value>=maxcurrent)||(value>inputvalue) % last two comparators will be unecessary when ammeter is included
%         previousvalue=value;
%         break
%     else
%         value=value*1.025
%         writePWMDutyCycle(a, 'D3',value/10);
%         pause(0.1)
%         count=count+1;
%     end
%     
% %     ammeter=ammeter*1.01;
% %     writePWMDutyCycle(a, 'D3',ammeter/10);
% %     pause(0.1)
% %     if (ammeter>maxcurrent)||(ammeter>inputvalue)
% %         break
% %     end
% end

%% ammeter control system
% maxcurrent=9.8;
% inputvalue=9;
% if inputvalue<=3
%     writePWMDutyCycle(a, 'D3',3/10);
% elseif inputvalue>3
%     writePWMDutyCycle(a, 'D3',3/10);
%     ammeter=readVoltage(a,'A0');
%     while (inputvalue<maxcurrent)&&(ammeter<inputvalue)
%         ammeter=readVoltage(a,'A0');
%         ammeter=ammeter*1.01;
%         writePWMDutyCycle(a, 'D3',ammeter/10);
%         pause(0.1)
%         if (ammeter>maxcurrent)||(ammeter>inputvalue)
%             break
%         end
%     end
% end
    





%%
% ammeter=readVoltage(a,'A0')
% i=1;
% while ammeter<=4.5
%     ammeter=readVoltage(a,'A0');
%     writePWMDutyCycle(a, 'D3',ammeter/10);
%     i=i+1
%     pause(0.1)
%     if ammeter>4
%         break
%     end
% end


% writePWMDutyCycle(a, 'D6', 0) % motor speed ranges from 0 to 1
% writePWMDutyCycle(a, 'D7', 1) % 0 is for clockwise; 1 is for counterclockwise
%% testing with LEDs
% for i=1:100
%     potentiometer(i)=readVoltage(a,'A0');
%     writePWMDutyCycle(a, 'D3', potentiometer(i)/5.0); %divide potentiometer value by 5 when potentiometer is connected to 5V power source
%     pause(0.1)
% end

% a=7;
% if a<=3||a>=7
%     disp(3)
% else
%     disp(4)
% end
% 
% %%
% 
% for i=1:10
%     x(i+1)=i+x(i)
%     while x(i+1)<10
%         



% 
%    writeDigitalPin(a, 'D3', 0);
%    pause(2);
%    writeDigitalPin(a, 'D3', 1);
%    pause(2);
%    writeDigitalPin(a, 'D3', 0);