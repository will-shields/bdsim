import MadX as _MadX
import numpy as _np
import matplotlib.pyplot as _plt

class Line:
    def __init__(self):
        self.x_current = 0
        self.y_current = 0
        self.theta     = _np.pi
        self.beamline  = []
        self.x_coords  = []
        self.y_coords  = []
        self.lentotal  = 0

    def Step(self,angle,length):
        self.theta += angle
        dx    = length * _np.cos(self.theta)
        dy    = length * _np.sin(self.theta)
        self.lentotal += length
        
        x_new = self.x_current - dx
        y_new = self.y_current - dy
        self.beamline.append([[self.x_current,x_new],[self.y_current,y_new]])
        self.x_current = x_new
        self.y_current = y_new
        self.x_coords.append(x_new)
        self.y_coords.append(y_new)

    def FinalDiff(self):
        dx = self.beamline[-1][0][1] - self.x_current
        dy = self.beamline[-1][1][1] - self.y_current
        print 'Final dx ',dx
        print 'Final dy ',dy


class BeamLine:
    def __init__(self):
        pass
    def Load(self,inputfilename):
        self.data = Line()
        
        self.file = _MadX.Tfs(inputfilename)
        
        i = 0
        for v in self.file.data['NAME']:
            self.data.Step(self.file.data['ANGLE'][i],self.file.data['L'][i])
            i += 1
            
        print 'Number of elements: ',i
            
    def Plot(self):
        
        _plt.figure()
        _plt.plot(self.data.x_coords,self.data.y_coords,'b.')
        _plt.plot(self.data.x_coords,self.data.y_coords,'b-')
        _plt.xlabel('X (m)')
        _plt.ylabel('Y (m)')
        _plt.show()
        
    
