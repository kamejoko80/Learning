PCA 中维数太大：假设 x 为 `m * n` 的 matrix, `m >> n`

`S = x * x'`, PCA需要计算S最大的k个特征值以及对应的特征向量。但是 m\*m 维度太大，无法直接求解。
Let `C = x‘*x, Cv = av`, a is eigenvalue, v is eigenvector, then `x*Cv = x*x'*xv = ax*v, ie, S*(xv) = a*(xv)`
所以假设(a v)是C的eigenvalue and eigenvector，那么(a, xv) 是S的eigenvalue and eigenvector

e.g. m = 1000000, there are 1000 pictures, n = 1000，
then what we need is compute the eigenvalue and eigenvector of the 1000 \* 1000 matrix.
