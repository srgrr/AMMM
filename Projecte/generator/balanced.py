import sys
import numpy as np

def main():
    num_cities, num_locations, seed = map(int, sys.argv[1:])
    np.random.seed(seed)
    print('numCities = %d;'%num_cities)
    print('numTypes = 3;')
    print('numLocations = %d;'%num_locations)
    print('cityCoordinates = [')
    x = np.random.rand(num_cities, 2)
    for p in x:
        print('[%.02f %.02f]'%(p[0], p[1]))
    print('];')
    print('locationCoordinates = [')
    x = np.random.rand(num_locations, 2)
    for p in x:
        print('[%.02f %.02f]'%(p[0], p[1]))
    print('];')
    print('d_center = 0.05;')
    x = np.random.randint(10, 21, num_cities)
    print('cityPopulation = [')
    print(' '.join(str(k) for k in x))
    print('];')
    print('typeCapacity = [100 200 400];')
    print('typeDistance = [0.1 0.2 0.4];')
    print('typeCost = [1 2 4];')
if __name__ == '__main__':
    main()
