import Tester as _Tester 

class BeamTester(_Tester.Tester) : 
    def __init__(self, particle, energy, distribution) :
        self.particle     = particle 
        self.energy       = energy 
        self.distribution = distribution
    
    def makeOptions(self) : 
        pass

    def makeBeam(self) : 
        pass 

    def makeLattice(self) : 
        pass

    def run(self) :
        pass
    
    def plotOutput(self) :
        pass

