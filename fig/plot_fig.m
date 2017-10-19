%% ----------------- Acceptacne Rate -------------

Delta=[0.1 0.3 0.7 1 1.5 2 3 4 5];
Rate =[0.94226 0.8308 0.6318 0.51108 0.368 0.278420 0.18678 0.140 0.111637];

figure ;
plot(Delta,Rate,'*-');
ylabel('Acceptance Rate');
xlabel('\Delta');
legend('\omega \epsilon=0.25');

%% ----------------- Correlation --------------------

%fname = 'output/Stat_100_0628.out';
fname = 'output/Stat_40000_2001.out';
fid = fopen(fname,'r');
A = fscanf(fid,'%f',[3,15]);

figure ;
errorbar(A(1,:),A(2,:),A(3,:),'.-','CapSize',2);

ylabel('<x(\tau+\tau_0) x(\tau_0)>');
xlabel('\tau');
%axis('tight');
%legend('\omega \epsilon=0.25 N_t = 1000');
t = annotation('textbox',...
               'String',{'\omega\epsilon=0.25','N_t=1000',...
                   'N_{mc}=40000','N_{sweeps}=100','\Delta=1.0'},...
               'LineStyle','none',...
               'Position',[0.7 0.7 0.2 0.2]);
fclose(fid);

%% --------------- Autocorrelation ----------------
figure;
fname = 'autocorrelation.out';
fid = fopen(fname,'r');
A = fscanf(fid,'%f',[2,inf]);

plot(A(1,:),A(2,:),'.-');
legend('random');
xlabel('N_{sweeps}');
ylabel('<x(0)x(5)> autocorrelation');
fclose(fid);