## 常见数值计算方法

- [非线性方程的迭代求解](./NonLinearEquationSlover.cpp)

利用**二分法**、**牛顿法**、**弦截法**迭代地求解非线性方程的根。

- 线性方程组的求解([直接法](./LinearEquationsSlover.cpp)、[迭代法](./SOR.cpp))

利用**Gauss消元**或者**Doolittle直接分解法**求解线性方程组、利用**SOR迭代**的方法求解线性方程组的数值解。

- 求解矩阵的特征值([幂法与反幂法](./PowerMethod.cpp)、[Jacobi方法](./Jacobi.cpp))

**幂法**与**反幂法**求解矩阵的最大最小特征值、**Jacobi方法**求解实对称矩阵的所有特征值。