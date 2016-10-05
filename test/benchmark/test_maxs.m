addpath ('../../matlab');

n = 10*1000*1000;
nq = 10; 

for k = [1 2 5 10 20 50 100 200 500 1000 2000 5000 10000 20000 50000 ...
    100000 200000 500000 1000000 2000000 5000000] 

% Generate 10 sequences of 10 million distances
X = randn (n, nq, 'single');

% Sort
t0 = cputime;
[v1, s1] = sort (X);
s1 = s1(1:k, :); 
t1 = cputime;

% Yael
[v2, s2] = yael_kmin (X, k);
t2 = cputime;

fprintf ('n= %d  k= %d  sort: %.3f   yael: %.3f\n', n, k, (t1-t0)/nq, (t2-t1)/nq); 

end

% Compare the cost of computing distances versus find smallest
n = 1000000;
nq = 10;
k = 100;

B = randn(128, n, 'single');
Bt = B';
Q = randn(128, nq, 'single');

t0 = cputime;

D = Bt * Q;
t1 = cputime;

[v1, s1] = sort (D);
s1 = s1(1:k, :); 
t2 = cputime

[v2, s2] = yael_kmin (D, k);
t3 = cputime;

fprintf ('n= %d  k= %d\n distances: %.3f\n sort: %.3f\n yael: %.3f\n', n, k, (t1-t0)/nq, (t2-t1)/nq, (t3-t2)/nq); 
