# dbscan_algorithm_cpp
#### Simple dbscan algorithm implementation for c++ with openCV type compatibility [soon]
---

Usage:
```
...
std::vector<Point> p{...};
unsigned int min_points = 10;
float epsilon = 0.7;
dbs::DBScan d{p, min_points, epsilon};
std::vector<Point> out;
p.apply(out);
...
