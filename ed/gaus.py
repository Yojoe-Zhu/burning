import math
import numpy as np

def gaus_kernel(m, n, sigma=1.4):
    kernel = np.arange(m*n, dtype=float).reshape(m, n)
    km = (m - 1)/2
    kn = (n - 1)/2

    for i in range(1, m+1):
        for j in range(1, n+1):
            kernel[i-1][j-1] = math.exp(-(pow(i - (km+1), 2) + pow(j - (kn+1), 2))/(2*sigma*sigma))/(2*math.pi*sigma*sigma)

    return kernel/kernel.sum()

kernel = gaus_kernel(3, 3)
print(kernel)
