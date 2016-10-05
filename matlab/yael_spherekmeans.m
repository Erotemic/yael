% Rv spherical k-means (efficient)
% - assume that vectors on input are normalized
% - handle void cluster
% 
% Usage:  [C, idx] = sphere_kmeans (X, k)
%         [C, idx] = sphere_kmeans (X, k, niter)
%         [C, idx] = sphere_kmeans (X, k, niter, callback)
% 
% Example:
% k = 256;    % number of clusters
% d = 128;    % dimensionality
% n = 10000;  % number of input vectors
% 
% X = randn (d, n, 'single');
% X = bsxfun (@times, X, (sum (X.^2)) .^ (-0.5));  % normalize
% 
% tic; [C1, ~] = sphere_kmeans(Xl, k, 20); toc
function [C, idx] = sphere_kmeans (X, k, niter, callback)

slice = 256;
n = size (X, 2);

if ~exist('niter','var'), niter = 10; end

if ~exist('callback','var')
  tic ; h = waitbar (0, 'spherical k-means');
  callback =  @(i,C,X,G) (waitbar (i/niter, h, sprintf('spherical k-means: iter %d: %.3fs', i, toc)));
end
    
% Initialization with random vectors
C = X(:, randperm(n, k));

for i = 1:niter
  
  % Assign step: per slice to limit memory overhead
  m1 = -inf * ones(1,n);
  idx = zeros (1, n);
  
  for q = 1:slice:k;
    idC = q:min(q+slice-1, k);
    [m2, id2] = max (C(:,idC)' * X);
    id2 = idC (id2);  
    T = m1 < m2;
    idx = (1-T) .* idx + T .* id2;
    m1  = max (m1, m2);
  end
  
  % Identify how to group these vectors into clusters
  G = accumarray(idx',1:n,[k 1],@(x){x});
  
  % find the empty cells and re-assign them if necessary
  zeropos = find(cellfun (@numel, G) == 0);
  nzeros = numel(zeropos);
  
    if nzeros > 0 
    fprintf ('Warning: %d empty clusters\n', nzeros);
    G (zeropos) = mat2cell(randperm (k, nzeros)', ones(nzeros, 1));
  end
  
    
  % Update stage
  for j=1:k
      C(:,j) = sum(X(:,G{j}), 2);
  end
   
  % Normalize the centroids
  C = bsxfun (@times, C, (sum (C.^2)) .^ (-0.5)); 
 
  callback (i, C, X, G);
end

if exist ('h','var') ; close (h); end
