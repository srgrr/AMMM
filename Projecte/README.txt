################################
        INTRODUCTION
################################
The requirements for running the meta-heuristics programs are the following:
- Python 3.3+
- A C++ compiler that supports c++11
- OpenMP is optional, but it improves the meta-heuristics performance.

################################
          DATA FORMAT
################################
The problem data is represented in DAT format. The variables (and their format)
are the following:

--------------------------------------------------------------------------------
numCities = integer;
numTypes = integer;
numLocations = integer;

// coordinates are floats
cityCoordinates = [
  city[1].x city[1].y
  ...
  city[numCities].x city[numCities].y
];

locationCoordinates = [
  location[1].x location[1].y
  location[numLocations].x location[numLocations].y
];

// integers
cityPopulation = [ city[1].population ... city[numCities].population ];

d_center = a float;

// integers
typeCapacity = [type[1].capacity ... type[numTypes].capacity];

// floats
typeDistance = [type[1].distance ... type[numTypes].distance];

// integers
typeCost = [type[1].cost ... type[numTypes].cost];
--------------------------------------------------------------------------------

It is recommended to work with small floats and to not impose very precise
distance constraints. Floating point precision is limited and here operations
like the euclidean distance between two points are performed (which is not
very precision-friendly). Some examples can be found in the data/ folder.

################################
   INTEGER LINEAR PROGRAMMING
################################
All Integer Linear Programming materials can be found in the ILP folder.
The contents are the following:

ilp_model.mod: The ILP model per se. It only contains the objective function and
the constraints definition. Data must be declared in an additional DAT file.

main.mod: A script that runs the model on a given .DAT file and generates
an output that can be feed to the solution_plotter.py script.

The run configurations are the following:
sample: contains the ILP model. Can solve single instances of the problem
if added to the configuration.
main: runs the main.mod script

solution_plotter.py: A python script that gets the output generated by main.mod
and draws a plot of the found solution.

################################
      INSTANCE GENERATORS
################################
The instance generators can be found in the folder generators/ and they
are the following:

balanced.py: It takes three arguments: numCities, numLocations, and seed.
It has some fixed parameters:
numTypes = 3;
d_center = 0.05;
typeCapacity = [100 200 400];
typeDistance = [0.1 0.2 0.4];
typeCost = [1 2 4];
All coordinates are numbers between zero and one and all populations are
between 10 and 20. Types are defined that way to avoid having a "safe" type
(that is, a type that it is clearly better than any other type).

convex_hull.py: It takes two arguments: numPoints, and seed.
It generates numPoints random 2d points with coordinates between 0 and 1,
and treats the points of the convex hull as locations and the points inside
the convex hull as cities. City populations, type capacities, and type costs
are defined exactly the same way as balanced.py
Distances are defined as follow:
d_center = 1.5 * minimum_distance_between_two_consecutive_points_of_the_hull
type_distance = multiples (4, 8 and 16) of the minimum distance between a
city and a location
This generator creates instances with a very small amount of locations, making
it harder for the meta-heuristics algorithms to find good initial solutions.
Scipy is required to run this generator.


################################
        METAHEURISTICS
################################
Meta-heuristics materials can be found in the metaheuristics folder.
Before running the meta-heuristics solver program, it must be compiled.
This can be done by typing "make" inside the metaheuristics folder.
Once we have compiled our program, we can run it as follows:
python3 main.py [-h] [--algorithm ALGORITHM] [--time_limit TIME_LIMIT] file_name

The parameters are the following:
--algorithm [GRASP|BRKGA]. Its default value is GRASP. This is the algorithm
that will be run.
--time_limit SECONDS. The amount of seconds we allow the program to run before
being aborted. Its default value is 3600
file_name is the absolute or relative path to a .DAT file that follows the same
format as
