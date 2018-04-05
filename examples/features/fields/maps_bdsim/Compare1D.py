import pybdsim
import matplotlib.pyplot as plt

def Compare1D(original, nearest, linear, cubic):
    a = pybdsim.Field.Load(original)
    n = pybdsim.Field.Load(nearest)
    l = pybdsim.Field.Load(linear)
    c = pybdsim.Field.Load(cubic)

    plt.figure()
    plt.plot(a[:,0], a[:,1], 'bo', label='Original', mec='b', alpha=0.8)
    plt.plot(n[:,0], n[:,1], 'b:', label='Nearest')
    plt.plot(l[:,0], l[:,1], 'r--',label='Linear')
    plt.plot(c[:,0], c[:,1], '-',  label='Cubic', color='grey')
    plt.xlabel('X (cm)')
    plt.ylabel('Field (T)')
    plt.title('X-Component of B Field')
    plt.legend(numpoints=1)
    plt.xlim(-65,65)
    plt.ylim(-3.5,4.5)
    plt.tight_layout()
    plt.savefig('1d_interpolated_x.pdf')

    plt.figure()
    plt.plot(a[:,0], a[:,2], 'bo', label='Original', mec='b', alpha=0.8)
    plt.plot(n[:,0], n[:,2], 'b:', label='Nearest')
    plt.plot(l[:,0], l[:,2], 'r--',label='Linear')
    plt.plot(c[:,0], c[:,2], '-',  label='Cubic', color='grey')
    plt.xlabel('Y (cm)')
    plt.ylabel('Field (T)')
    plt.title('Y-Component of B Field')
    plt.legend(numpoints=1)
    plt.xlim(-65,65)
    plt.ylim(-2.5,2.5)
    plt.tight_layout()
    plt.savefig('1d_interpolated_y.pdf')

    plt.figure()
    plt.plot(a[:,0], a[:,3], 'bo', label='Original', mec='b', alpha=0.8)
    plt.plot(n[:,0], n[:,3], 'b:', label='Nearest')
    plt.plot(l[:,0], l[:,3], 'r--',label='Linear')
    plt.plot(c[:,0], c[:,3], '-',  label='Cubic', color='grey')
    plt.xlabel('Z (cm)')
    plt.ylabel('Field (T)')
    plt.title('Z-Component of B Field')
    plt.legend(numpoints=1)
    plt.xlim(-65,65)
    plt.ylim(-2.5,2.5)
    plt.tight_layout()
    plt.savefig('1d_interpolated_z.pdf')

    plt.draw()

def PlotOriginal():
    a = pybdsim.Field.Load("1dexample.tar.gz")

    plt.figure(figsize=(6,4))
    plt.plot(a[:,0], a[:,1], 'bo', label='X', mec='b')
    plt.plot(a[:,0], a[:,2], 'rs', label='Y', mec='r')
    plt.plot(a[:,0], a[:,3], 'g^', label='Z', mec='g')
    plt.xlabel('Distance (cm)')
    plt.ylabel('Field Component (T)')
    plt.tight_layout()
    plt.legend(numpoints=1, loc=2)
    plt.xlim(-35,30)
    plt.ylim(-4.8,5)
    plt.savefig('original_data.pdf')


def PlotNearest():
    a = pybdsim.Field.Load("1d_interpolated_nearest.dat")
    o = pybdsim.Field.Load("1dexample.tar.gz")

    plt.figure(figsize=(6,4))
    plt.plot(o[:,0], o[:,1], 'bo', alpha=0.2, mec='b')
    plt.plot(o[:,0], o[:,2], 'rs', alpha=0.2, mec='r')
    plt.plot(o[:,0], o[:,3], 'g^', alpha=0.2, mec='g')
    
    plt.plot(a[:,0], a[:,1], 'b', label='X')
    plt.plot(a[:,0], a[:,2], 'r', label='Y')
    plt.plot(a[:,0], a[:,3], 'g', label='Z')
    plt.xlabel('Distance (cm)')
    plt.ylabel('Field Component (T)')
    plt.tight_layout()
    plt.legend(numpoints=1, loc=2)
    plt.xlim(-50,40)
    plt.ylim(-4.8,5)
    plt.savefig('1d_nearest_xyz.pdf')

def PlotLinear():
    a = pybdsim.Field.Load("1d_interpolated_linear.dat")
    o = pybdsim.Field.Load("1dexample.tar.gz")

    plt.figure(figsize=(6,4))
    plt.plot(o[:,0], o[:,1], 'bo', alpha=0.2, mec='b')
    plt.plot(o[:,0], o[:,2], 'rs', alpha=0.2, mec='r')
    plt.plot(o[:,0], o[:,3], 'g^', alpha=0.2, mec='g')
    
    plt.plot(a[:,0], a[:,1], 'b', label='X')
    plt.plot(a[:,0], a[:,2], 'r', label='Y')
    plt.plot(a[:,0], a[:,3], 'g', label='Z')
    plt.xlabel('Distance (cm)')
    plt.ylabel('Field Component (T)')
    plt.tight_layout()
    plt.legend(numpoints=1, loc=2)
    plt.xlim(-50,40)
    plt.ylim(-4.8,5)
    plt.savefig('1d_linear_xyz.pdf')

def PlotCubic():
    a = pybdsim.Field.Load("1d_interpolated_cubic.dat")
    o = pybdsim.Field.Load("1dexample.tar.gz")

    plt.figure(figsize=(6,4))
    plt.plot(o[:,0], o[:,1], 'bo', alpha=0.2, mec='b')
    plt.plot(o[:,0], o[:,2], 'rs', alpha=0.2, mec='r')
    plt.plot(o[:,0], o[:,3], 'g^', alpha=0.2, mec='g')
    
    plt.plot(a[:,0], a[:,1], 'b', label='X')
    plt.plot(a[:,0], a[:,2], 'r', label='Y')
    plt.plot(a[:,0], a[:,3], 'g', label='Z')
    plt.xlabel('Distance (cm)')
    plt.ylabel('Field Component (T)')
    plt.tight_layout()
    plt.legend(numpoints=1, loc=2)
    plt.xlim(-50,40)
    plt.ylim(-4.8,5)
    plt.savefig('1d_cubic_xyz.pdf')
    