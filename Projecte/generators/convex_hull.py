import sys
import numpy as np
from scipy.spatial import ConvexHull



def main():
    num_points, seed = map(int, sys.argv[1:])
    np.random.seed(seed)
    points = np.random.rand(num_points, 2)
    hull = ConvexHull(points)
    num_locations = hull.vertices.shape[0]
    num_cities = num_points - num_locations
    print('numCities = %d;'%(num_cities))
    print('numTypes = 3;')
    print('numLocations = %d;'%hull.vertices.shape[0])

    print('cityCoordinates = [')
    for i in range(num_points):
        if i in hull.vertices: continue
        print('[%.02f %.02f]'%(points[i, 0], points[i, 1]))
    print('];')

    print('locationCoordinates = [')
    for i in range(num_points):
        if not i in hull.vertices: continue
        print('[%.02f %.02f]'%(points[i, 0], points[i, 1]))
    print('];')

    city_population = np.random.randint(10, 21, num_cities)

    print('cityPopulation = [')
    print(' '.join(str(x) for x in city_population))
    print('];')

    d_center = 1.5 * min( np.linalg.norm(points[i] - points[i-1]) for i in hull.vertices)

    print('d_center = %.02f;'%d_center)

    print('typeCapacity = [100 200 400];')

    min_d = -1.0
    for i in range(num_points):
        for j in range(num_points):
            if i in hull.vertices and not j in hull.vertices:
                d = np.linalg.norm(points[i] - points[j])
                if min_d < 0.0 or d < min_d:
                    min_d = d

    print('typeDistance = [%.02f %.02f %.02f];'%(4.0*min_d, 8.0*min_d, 16.0*min_d))
    print('typeCost = [1 2 4];')

if __name__ == '__main__':
    main()
