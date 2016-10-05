addpath ('../../matlab');

n = 10000000;
nq = 10; 
k = 10000; 

% Generate 10 sequences of 10 million distances
X = randn (n, nq, 'single');


% Sort
t0 = cputime;
[v1, s1] = sort (X);
s1 = s1(1:k, :); 
fprintf ('cputime = %.3fs\n', cputime - t0);

% 
t0 = cputime;
[v2, s2] = yael_kmin (X, k);
fprintf ('cputime = %.3fs\n', cputime - t0);
