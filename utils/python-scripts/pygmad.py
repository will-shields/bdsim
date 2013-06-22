import ctypes as _ctypes
import numpy as _np
import matplotlib.pyplot as _plt

class Survey : 
    def __init__(self) : 
        self.x_current = 0 
        self.y_current = 0
        self.theta     = _np.pi
        self.names     = []
        self.beamline  = []
        self.x_coords  = [] 
        self.y_coords  = []
        self.lentotal  = 0

    def Load(self, fileName) : 
        self.file = Loader() 
        self.file.load(fileName) 
        
        nelement = self.file.parserLib.get_nelements()
        
        for i in range(0,nelement,1) : 
            name   = self.file.parserLib.get_name(i) 
            length = self.file.parserLib.get_length(i)
            angle  = self.file.parserLib.get_angle(i)
            self.names.append(name)
            self.Step(angle,length)
            i += 1    
               
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
        
    def Plot(self) : 
        _plt.figure()
        _plt.plot(self.x_coords,self.y_coords,'b.')
        _plt.plot(self.x_coords,self.y_coords,'b-')
        _plt.xlabel('X (m)')
        _plt.ylabel('Y (m)')
        _plt.show()

    def CompareMadX(self, fileName) : 
        import pymadx as _pymadx
        
        mxs = _pymadx.Survey()
        mxs.Load(fileName) 

        self.Plot()

        _plt.plot(mxs.x_coords,mxs.y_coords,'rx')
        _plt.plot(mxs.x_coords,mxs.y_coords,'r-')
        _plt.show()        
        
    def FindClosetElement(self, coord) : 
        arr = _np.arange(0,len(self.x_coords),1) 
        xdiff = _np.array(self.x_coords)-coord[0]
        ydiff = _np.array(self.y_coords)-coord[1]
        d     = _np.sqrt(xdiff**2+ydiff**2)
        print arr[d == min(d)]
        
        


class Loader :
    def __init__(self) :
        self.parserLibFileName = "/Users/sboogert/Physics/general/acc/bdsim/bdsim-mac/parser/libgmadShared.dylib"
        self.parserLib = _ctypes.cdll.LoadLibrary(self.parserLibFileName)
        
        self.parserLib.get_name.restype    = _ctypes.c_char_p
        self.parserLib.get_name.argtypes   = [_ctypes.c_int]
        self.parserLib.get_type.restype    = _ctypes.c_short
        self.parserLib.get_type.argtypes   = [_ctypes.c_int]    
        self.parserLib.get_length.restype  = _ctypes.c_double
        self.parserLib.get_length.argtypes = [_ctypes.c_int]
        self.parserLib.get_angle.restype   = _ctypes.c_double
        self.parserLib.get_angle.argtypes  = [_ctypes.c_int]
    
    def load(self, fileName) :
        self.parserLib.gmad_parser_c(fileName)
    
    def getElement(self, i) : 
        name   = self.parserLib.get_name(i) 
        type   = self.parserLib.get_type(i) 
        length = self.parserLib.get_length(i)
        angle  = self.parserLib.get_angle(i)
        
        return [name,type,length,angle]

    def getElementByName(self, name) :
        pass

    def parseLattice(self) : 
        return 

    
    def printToScreen(self) :
        nelement = self.parserLib.get_nelements()

        for i in range(0,nelement,1) : 
            name   = self.parserLib.get_name(i) 
            type   = self.parserLib.get_type(i) 
            length = self.parserLib.get_length(i)
            angle  = self.parserLib.get_angle(i)
            print i,name,type,length,angle        
    
