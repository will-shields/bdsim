import numpy as np

def Load(filename):
    """

    Parses raw ASCII output from BDSIM into a numpy array.

    Example:
    Load('../path/to/my/file.txt')
    returns numpy array
    """

    dataSet = []

    f = open(filename,'r')
    lines = f.readlines()
    f.close()

    for line in lines[3:]:
        dataSet.append(map(np.double,line.strip("\n").split()))

    dataSet=np.array(dataSet)

    return dataSet

