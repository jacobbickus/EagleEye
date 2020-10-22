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

Be sure to untar the NRF Database using 
`> tar xfz NRF_Database.tar.gz`

Export the path to the untarred NRF Database for example in the user's bash:

`export G4NRFGAMMADATA=/path/to/Database/Database1.1`

Also be sure to export the ROOT Cmake path in the directory where the user built ROOT for example in user's bash:

`export ROOTCMAKEPATH=/path/to/root_build_directory/etc/cmake`

Lastly some path issues may occur without the following lines in the user's bash:

`export G4MPICMAKEPATH=/path/to/geant4MPI/geant4MPI_build`

`source /path/to/root_build_directory/bin/thisroot.sh`

`source /path/to/geant4-install/bin/geant4.sh`

`source /path/to/geant4-install/share/Geant4-10.5.1/geant4make/geant4make.sh`

`PATH=$HOME/bin:$PATH`

`PATH=$PATH:/bin/mpiexec`

`PATH=/path/to/mpich-install/bin:$PATH`

`export PATH`

To Build
==

Now go to geant4 application
`> mkdir <application_build> && cd <application_build>`
`> cmake -DG4mpi_DIR=/path/to/geant4MPI_build /path/to/application/source && make -j[N]`

Preparing the Run
==
The input bremstrahulung spectrum for the run can be prepared through the Sampling.cc located in the source directory.
Sampling.cc can be run using CERN ROOT with the command:
`> root -b -q 'Sampling("rootInput.root", BremSpectrumEnergyMax)'`
The Sampling.cc file takes a root file with a bremstrahulung distribution and the distribution's maximum energy and outputs a file named brem_distributions.root and brems_distributions.png consisting of the bremstrahulung input distributions and an importance sampling distribution for NRF for the bremstrahulung distribution. 


To Run
==

mpiexec -n <number of cores> ./EagleEye eagle.in <MasterSeed> <number of cores>

The number of cores set with the flag -n for mpiexec should be the same as the number of cores inputed to EagleEye. Inputing the incorrect number of cores will not crash program but will give you an unpredictable seed for all cores where a seed is not assigned. 
  
Required Inputs
==

EagleEye Requires two inputs:
* eagle.in - The main input file that allows the user to change the geometry setup and materials as well as detector angles and sizes. This is also where the user can turn the Chopper Wheel State on/off
* brems_distributions.root - This is the input spectrum file that is read as long as the input/energy is less than 0. Reading from the input spectrum file is set as the default. The bremstraulung input and sampling distribution can be easily manipulated with Sampling.cc  

Output
==

The Code will generate two files. The file with the final-merged histograms is found in filename-merged.root. where filename is set in eagle.in. EagleEye does not permit visualizations. In order to visualize Changes made via eagle.in simply apply the same changes in mantis.in and run ./mantis -m vis_save.mac to create a .wrl file or in interactive mode ./mantis to use OPENGL. 

Analysis
==

The PostAnalysis.cc file analyzes Chopper On/Off root files for a given experimental simulation. The PostAnalysis.cc file can be run in CERN ROOT with the command:
`> root -b -q 'PostAnalysis("ChopperOn.root","ChopperOff.root")'`

The File outputs the On/Off States respective: Detected Entries, Detected Weighted Sum, NRF Incident Water Weighted Sum. It also outputs the percent difference between the Chopper On/Off detected weighted sums. 



