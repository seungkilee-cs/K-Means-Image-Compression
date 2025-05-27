# Theory
The basic understanding of K Means Clustering from [StatQuest](https://www.youtube.com/watch?v=4b5d3muPQmA)

## What is K Means Cluster?
By determining best fitting mean "cluster" of the presented data observed on a given dimension, K Means Cluster can be used to accurately represent the original data with less resource. K represents the number of clusters we set to represent data.

## Steps
1. Randomly select K data point from the dataset. These will be the initial cluster.
2. Starting from the "first" data point of the dataset, find the distance between this point and the nearest cluster.
3. "Assign" the point of comparison to the nearest cluster. Move on to the next data point and repeat this process until we've assigned every data point in the set to a cluster.
4. Once evrey data point has been assigned to a cluster, calculate the mean of all points assigned to each cluster. These mean data points will be the new cluster.
5. Repeat this measure and cluster with newly calculated cluster.
6. Get **total variation** for each cluster we have. If the variance is unbalanced, start from a different set of points. The calculation will continue until the variation in each of the clusters no longer change while we repeat the clustering process.
7. Because this is a NP-hard, we need a heuristic to determine where to stop the algorithm. We tell the code to repeat this process X amount of times and then retrieve the solution with best distribution of variant.

## Concept
The key here is that we are looking to minimize Total Variation for the clustered data. Imagine that for N data points, we set K = N. In this case we will have 0 total variation because all data points will have its own cluster. Conversely, we can also have K = 1. In this case we will get the largest possible total variation because the entire distribution of the N data points is now in variation. 

You can see that smaller the K less the resource we need to represent the same data, but the variation will yield ineffective representation of the original data. On the other hand the larger K will give us better representation of the data but won't be most efficient in terms of resource.

Heuristic here is to find the sweet spot that gives us the best marginal return on K. You can empirically attain this by finding the relationship between the Reduction in Variance from increasing the number of clusters against the number of clusters, and finding the highest rate of change. We are looking for the shape of "Elbow Plot" where the marginal reduction in variation is the highest, and consequentially this is the local optimum for our K.

## Finding the Distance
You can see finding the distance between the set cluster and all of our data points is important operation. The distance here is Euclidean distance, represented by 

$$
d(p,q) = \sqrt{\Sigma^{n}_{i=1} {(q_{i} - p_{i})^2}}
$$

where 
- p, q are the points in Euclidean space 
- q_{i} and p_{i} are the Euclidean vectors from the initial points
- n is the dimension of the Euclidean space

on a 1D space, it's easy to find the distance between two points, as it is the difference of two points on a single coordinate. On a 2D space, we utilize the pythagorean theorem to get the distance.

For heatmaps, we calculate the distance between the axis as if we have a plotted points. Here we can apply the generalized formula above.

## Algorithm
1. Specify the number k of clusters to assign
2. Randomly initialize k centroids
3. Do While: The centroid positions do not change
4.  expectation: assign each point to its closest centroid
5.  maximization: compute the new centroid (mean) of each cluster