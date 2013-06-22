import ctypes as _ctypes

class gmadLoader :
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
                   
    def view(self) :
        nelement = self.parserLib.get_nelements()

#        name = _ctypes.create_string_buffer('',64)        
        for i in range(0,nelement,1) : 
            name   = self.parserLib.get_name(i) 
            type   = self.parserLib.get_type(i) 
            length = self.parserLib.get_length(i)
            angle  = self.parserLib.get_angle(i)
            print i,name,type,length,angle        
    
