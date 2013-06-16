import pylab as pl 
def RingBuilder(ndipole = 10, ldipole = 0.5, clength = 10.0, cell = []) : 
    '''Build a random ring
    ndipole : number of dipole magnets
    ldipole : length of dipole 
    clength : length of other components
    cell    : components and specificiations of each dipole cell
    '''

    line = []
    
    for i in range(0,ndipole,1) : 
        name = 'dipole.'+str(i)
        type = 'sben' 
        angl = 2*pl.pi/ndipole
        leng = ldipole 
        
        line.append([name,type,leng,angl])

        name = 'drift.'+str(i)
        type = 'drif' 
        leng = clength-ldipole 
        line.append([name,type,leng])
        

    totalbl = 0.0
    totall  = 0.0 
    for e in line : 
        # total bending length
        if e[1] == 'sben' : 
            totalbl += e[2]
            
        # sum all component lengths
        totall += e[2]

        print e

    print 'RingBuilder> total bength length',totalbl
    print 'RingBuilder> total length',totall


    f = open("RandomRing.gmad","w")

    # write components
    for e in line : 
        wl = ''
        if e[1] == 'sben' : 
            wl = '{0} : {1}, l={2}, angle={3};\n'.format(e[0],e[1],e[2],e[3])
        elif e[1] == 'drif' : 
            wl = '{0} : {1}, l={2};\n'.format(e[0],e[1],e[2])         
        f.write(wl)

    # write line  
    l = ''
    last = False 
    for e in line :
        if e == line[-1] : 
            last = True

        l += e[0]
        if not last : 
            l += ','

    l = 'lat : line = ('+l+');\n'
    
    f.write(l)
        
    f.close()
