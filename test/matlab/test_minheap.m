nq = 1000;
k = 10;

Nb = 100000;

niter = 500;
nb = Nb / niter;

Val = randn (Nb, nq, 'single');


% The min-heap should be initialized
bhv = inf * ones (k, nq, 'single');
bhidx = zeros (k, nq, 'uint32');

tic
for i = 1:niter
   idx = uint32((i-1) * nb + (1:nb)');
   val = Val (idx , :);

   [bhv,bhidx] = yael_minheap (bhv, bhidx, val, idx);

end
toc

% Re-order the vectors
tic
[v2,idx] = sort (bhv);
idx = bsxfun(@plus,idx,0:k:(nq*k)-1);
bhidx (:) = bhidx (idx);
bhv (:) = bhv (idx);

%bhidx = bhidx(i2);
toc

tic
[v2,idx2] = sort (Val);
v2 = v2(1:k,:);
idx2 = idx2(1:k, :);
toc


