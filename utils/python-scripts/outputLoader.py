import numpy as _np

class ascii : 
    def __init__(self) : 
        self.keys = dict() 
        self.keys['part']   = 0
        self.keys['energy'] = 1
        self.keys['x']      = 2
        self.keys['y']      = 3
        self.keys['z']      = 4 
        self.keys['xp']     = 5
        self.keys['yp']     = 6

    def Load(self,filename):
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
            dataSet.append(map(_np.double,line.strip("\n").split()))
            
        dataSet=_np.array(dataSet)

        self.dataSet = dataSet

        # rounded z sampler locations
        z = self['z']
        round = 7
        self.sampzround = _np.round(z,round) 
        self.sampz      = _np.unique(self.sampzround)
        self.nsamp      = len(self.sampz)

    def findSamplers(self, isamp = 0) : 
        '''Return array of true/false for a given sampler'''
        return self.sampzround == self.sampz[isamp]
    
    def getSamplerData(self, isamp = 0) : 
        '''Return slice of data for a given sampler'''
        if isamp < self.nsamp and isamp > -1 : 
            c = self.findSamplers(isamp) 
            return self.dataSet[c,:]
        else :
            raise IndexError('Index {0} is out of bounds for size {1}'.format(isamp,self.nsamp))

    def __getitem__(self, key) : 
        '''Get a array of data''' 
        return self.dataSet[:,self.keys[key]]

    
    
