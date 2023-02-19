%%
K = 1;
wn = 20*2*pi;
xi = 0.8;
Num = [K*wn*wn];
Den = [1  2*xi*wn wn*wn]; 
sys = tf(Num,Den);


%%
T = 0.001;
sys_d = c2d(sys,T,'tustin');
[numd,dend]=tfdata(sys_d,'v');

bode(sys,sys_d);


%%
%FPGA �����õ��Ƕ�������32λ������λΪ12�����Ա�ʾ-2048��+2048֮�������������9.5367E-7
numd_1 = numd.*2^32
dend_1 = dend.*2^32