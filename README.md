### Overview

A simple benchmark on how well the
[Hilbert Curve Sort](https://github.com/CarbonicSoda/hilbert-curve-sort) performs on the TSPLIB
dataset in both C++ and JavaScript. Particularly, the famous [pla85900.tsp](pla85900.tsp) set
containing 85900 nodes.

The result is quite fascinating to me. This algorithm that is not designed for TSP problems,
achieved a distance of 188465250, only a rough x1.32 of the mathematically optimal
distance 142382641.

That might seem to be terrible, but many dedicated approximation algorithms do no better. This
algorithm is also more efficient and is able to complete sorting in ~30ms on my 11th Gen Intel(R)
Core(TM) i5-11320H @ 3.20GHz laptop (C++ version).

A slight flaw of using the algorithm to approximate TSP solutions is that TSP asks for a _cycle_,
not a simple tour. The algorithm will not minimize the distance between the first and the last node,
adding an overhead to the resultant distance, but is usually minor.

### Visualization

#### Optimal

![Optimal](assets/pla85900_optimal.png)

#### Hilbert Curve

![Hilbert Curve](assets/pla85900_hilbert_curve.png)
