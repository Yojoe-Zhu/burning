* edge-detector.cpp
实现了 Roberts 算子、Sobel 算子等边缘检测算子，编译方法：

```shell
./build.sh edge-detector edge-detector.cpp
```

执行方法：

```
./edge-detector Bikesgray.jpg
```

* img-plt.py

用于将图像（S.png）映射成离散二元函数图像，用法（过程有点慢，需要等待约5~10s）：

```
python3 img-plt.py
```

