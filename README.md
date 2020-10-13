# mantis with mpi

In order to use mpi the user must complete the following

- Build and install geant4 libraries

mkdir geant4
cd geant4
*Download geant4 version into geant4 directory

mkdir geant4-build geant4-install
cd geant4-build
cmake -DCMAKE_INSTALL_PREFIX=../geant4-install -DGEANT4_INSTALL_DATA=ON \
-DGEANT4_USE_OPENGL_X11=ON -DGEANT4_BUILD_MULTITHREADED=ON ../<geant4src>
*Once the geant4 libraries are built
cd ..
Copy MPI files over to local files
cp -rf ../../<geant4src>/examples/extended/parallel/MPI/ ~
There is a bug in G4MPIextraworker.cc 
To Fix the bug change #include "g4ios.hh" to #include "G4ios.hh"
cd ~
mkdir geant4MPI
cd geant4MPI
mkdir geant4MPI_build geant4MPI_install
cd geant4MPI_build
cmake -DCMAKE_INSTALL_PREFIX=../geant4MPI_install \
-DGeant4_DIR=../../geant4-install/lib[64]/Geant4* \
~/MPI/source && make -j[N] && make install 

*Now go to geant4 application
mkdir <application_build>
cd <application_build>
cmake -DG4mpi_DIR=/path/to/geant4MPI_build /path/to/application/source && make -j[N] 
