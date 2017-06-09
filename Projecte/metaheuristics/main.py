import os
import time
import argparse
import subprocess
import sys
from io import StringIO
from DATParser import DATParser

if sys.version_info < (3, 3):
    print('This script only works for Python 3.3+')
    sys.exit(1)

def get_cpp_input(data):
    ret = ''
    ret += '%s %s %s\n'%(data.numCities, data.numTypes, data.numLocations)
    ret += ''.join('%.20f %.20f\n'%(a, b) for (a, b) in data.cityCoordinates)
    ret += ''.join('%.20f %.20f\n'%(a, b) for (a, b) in data.locationCoordinates)
    ret += ' '.join(str(x) for x in data.cityPopulation) + '\n'
    ret += str(data.d_center) + '\n'
    ret += ' '.join(str(x) for x in data.typeCapacity) + '\n'
    ret += ' '.join('%.20f'%x for x in data.typeDistance) + '\n'
    ret += ' '.join(str(x) for x in data.typeCost) + '\n'
    return ret

def main():
    parser = argparse.ArgumentParser(description="Runs a Metaheuristic algorithm on a given .dat file")
    parser.add_argument("file_name", help="Path to the .dat file")
    parser.add_argument("--algorithm", default="GRASP", help="Algorithm name (GRASP|BRKGA|BRUTEFORCE)")
    parser.add_argument("--time_limit", default=60, type=int, help="Time limit (in seconds)")

    args = parser.parse_args()

    if args.algorithm not in ['GRASP', 'BRKGA', 'BRUTEFORCE']:
        print ('--algorithm must be equal to GRASP, BRKGA, or BRUTEFORCE')
        sys.exit(1)

    data = DATParser.parse(args.file_name)

    data_input = get_cpp_input(data)

    cmd = ['./main', args.algorithm]
    pread, pwrite = os.pipe()
    os.write(pwrite, data_input.encode('utf-8'))
    os.close(pwrite)

    sys.exit(subprocess.call(cmd, stdin=pread, timeout=args.time_limit))



if __name__ == '__main__':
    main()
