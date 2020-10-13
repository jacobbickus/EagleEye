EagleEye -- Mantis with a MPI Twist
===================================
This program allows the user to run calorimetric NRF Cherenkov Detector cargo screening simulations with MPI allowing for parallelization. This simulation code is based off Mantis but with several simplifications to make the code more efficient to run in parallel. Please see Mantis' documentation for further details. The final output provided to the user contains two histograms. One histogram is the energy spectrum incident the photocathode and the other histogram is the energy spectrum "detected" by the photocathode based on a user's given quantum efficiency (determined intrinsically based off the photocathode material). 

Author: Jacob E Bickus

Creation time: 8/2020
Last Update: continous 

To Install
==

Dependencies
  * Installed Geant4 Libraries 
  * CERN ROOT Installed 
  * MPI (MPICH) Installed 
  * Cmake 
  
__Version__:  EagleEye and been built against and tested with Geant4 10.5, ROOT 6.14 and ROOT 6.16, and MPICH 3.3.2. Geant4 10.6 has important modifications that require changes in the code.

The Following Instructions should work once the Geant4, CERN ROOT, and MPICH Libraries are installed:

Copy MPI files over to local files

`> cp -rf /<geant4src>/examples/extended/parallel/MPI/ /user/local/file/to/hold/MPI`

There is a bug in G4MPIextraworker.cc 
To Fix the bug change #include "g4ios.hh" to #include "G4ios.hh"

`> cd /user/local/file/to/hold/MPI`
`> mkdir geant4MPI && cd geant4MPI && mkdir geant4MPI_build geant4MPI_install`
`> cd geant4MPI_build`

Assuming the directory where geant4 is installed on user's computer is /home/geant4
`> cmake -DCMAKE_INSTALL_PREFIX=../geant4MPI_install \
-DGeant4_DIR= ~/geant4/geant4-install/lib[64]/Geant4* \
~/user/local/file/to/hold/MPI/source && make -j[N] && make install`

To Build
==

Now go to geant4 application
`> mkdir <application_build> && cd <application_build>`
`> cmake -DG4mpi_DIR=/path/to/geant4MPI_build /path/to/application/source && make -j[N]`

To Run
==

mpiexec -n <number of cores> ./EagleEye eagle.in
  
Required Inputs
==

EagleEye Requires two inputs:
* eagle.in - The main input file that allows the user to change the geometry setup and materials as well as detector angles and sizes. This is also where the user can turn the Chopper Wheel State on/off
* brems_distributions.root - This is the input spectrum file that is read as long as the input/energy is less than 0. Reading from the input spectrum file is set as the default. The bremstraulung input and sampling distribution can be easily manipulated with Sampling.cc  

Output
==

The Code will generate two files. The file with the final-merged histograms is found in filename-merged.root. where filename is set in eagle.in.

