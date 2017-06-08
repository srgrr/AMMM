import matplotlib.pyplot as plt

numCities = int(input())
cityCoordinates = []
for _ in range(numCities):
    cityCoordinates.append(list(map(float, input().split(" "))))

numLocations = int(input())
locationCoordinates = []
for _ in range(numLocations):
    locationCoordinates.append(list(map(float, input().split(" "))))

numTypes = int(input())
typeDistance = []
for _ in range(numTypes):
    typeDistance.append(float(input()))

typeInLocation = []
for _ in range(numLocations):
    typeInLocation.append(int(input())-1)

primaryCenters = []
for _ in range(numCities):
    primaryCenters.append(int(input())-1)

secondaryCenters = []
for _ in range(numCities):
    secondaryCenters.append(int(input())-1)

for i in range(numCities):
    primary_loc = primaryCenters[i]
    seg1 = cityCoordinates[i][0], locationCoordinates[primary_loc][0]
    seg2 = cityCoordinates[i][1], locationCoordinates[primary_loc][1]
    plt.plot(seg1, seg2, color = 'y')
    secondary_loc = secondaryCenters[i]
    seg1 = cityCoordinates[i][0], locationCoordinates[secondary_loc][0]
    seg2 = cityCoordinates[i][1], locationCoordinates[secondary_loc][1]
    plt.plot(seg1, seg2, color = 'b')

for i in range(numLocations):
    type_in = typeInLocation[i]
    if type_in >= 0:
        seg1 = locationCoordinates[i][0], locationCoordinates[i][0] + typeDistance[type_in]
        seg2 = locationCoordinates[i][1], locationCoordinates[i][1]
        plt.plot(seg1, seg2, color = 'm')
        cr = plt.Circle(locationCoordinates[i], typeDistance[type_in], color='k', fill=False)
        plt.gcf().gca().add_artist(cr)


plt.scatter(*zip(*cityCoordinates), color = 'r')
plt.scatter(*zip(*locationCoordinates), color = 'g')


plt.xlim([0.0, 1.0])
plt.ylim([0.0, 1.0])

plt.plot()
plt.savefig('sol.png')
