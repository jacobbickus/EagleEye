#include "DetectorConstruction.hh"

DetectorConstruction::DetectorConstruction()
        : G4VUserDetectorConstruction(), IntObj_rad(4.5*cm),
        radio_abundance(90*perCent), IntObj_Selection("Uranium"), intObjDensity(19.1*g/cm3),
        chopperDensity(19.1*g/cm3), intObj_x_pos(0*cm), intObj_y_pos(0*cm), intObj_z_pos(0*cm),
        chopperOn(false), chopper_thick(1*mm), chopper_z(10*cm), theAngle(120.0),
        water_size_x(60*cm),water_size_y(2.5908*m), water_size_z(40*cm),
        PMT_rmax(25.4*cm), nPMT(3), pc_mat("GaAsP"), detectorM(NULL)
{
        detectorM = new DetectorMessenger(this);
}

DetectorConstruction::~DetectorConstruction()
{
        delete detectorM;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
// Get nist material manager
        G4NistManager* nist = G4NistManager::Instance();

// Set materials
        G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
        G4Material *steel = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
        G4Material *Water = nist->FindOrBuildMaterial("G4_WATER");
        G4Material *tungsten = nist->FindOrBuildMaterial("G4_W");
        G4Material *lead = nist->FindOrBuildMaterial("G4_Pb");
        G4Element *elPb = new G4Element("Lead", "Pb", 82, 207.2*g/mole);
        G4Element *elN = new G4Element("Nitrogen", "N2", 7, 14.01*g/mole);
        G4Element *elO = new G4Element("Oxygen", "O2", 8, 16.0*g/mole);
        G4Element *elC = new G4Element("Carbon", "C", 6, 12.0*g/mole);
        G4Element *elH = new G4Element("Hydrogen", "H2", 1, 1.0*g/mole);


        G4Material *myAir = new G4Material("Air", 1.290*mg/cm3, 2);
        myAir->AddElement(elN, 0.7);
        myAir->AddElement(elO, 0.3);
        G4Material *myVacuum = new G4Material("Vacuum", 1.e-5*g/cm3, 1, kStateGas, 273.15, 2.e-2*bar);
        myVacuum->AddMaterial(myAir,1);
// technically PMT glass is a special borosilicate glass calle k-free glass
// but pyrex is close enough as a borosilicate glass
        G4Material* PMT_mat = nist->FindOrBuildMaterial("G4_Pyrex_Glass");
// Setting up weapons grade materials
        G4Isotope* Uranium235 = new G4Isotope("Uranium235", 92, 235, 235.04393*g/mole); // atomicnumber, number of nucleons, mass of mole
        G4Isotope* Uranium238 = new G4Isotope("Uranium238", 92, 238, 238.02891*g/mole);
        G4Isotope* Plutonium239 = new G4Isotope("Plutonium239",94, 239, 239.0521634*g/mole);
        G4Isotope* Plutonium240 = new G4Isotope("Plutonium240", 94, 240, 240.05381*g/mole);
        G4Element* U = new G4Element("WGU", "U", 2); // name, element symbol, #isotopes
        G4Element* WGPu = new G4Element("WGPu","Pu",2);
        G4Element* natU = new G4Element("NaturalU", "U",2);
        G4Element* Pu = new G4Element("Plutonium240","Pu",2);

// Set up Photocathode materials
        G4Element* elGa = new G4Element("Gallium", "Ga", 31, 69.723*g/mole);
        G4Element* elAs = new G4Element("Arsenic", "As", 33, 74.9216*g/mole);
        G4Element* elP = new G4Element("Phosphorous", "P", 15, 30.973762*g/mole);
        G4Material* GaAsP = new G4Material("GaAsP", 4.138*g/cm3, 3, kStateSolid, 293*kelvin, 1*pow(10,-6)*pascal);
        GaAsP->AddElement(elGa, 1);
        GaAsP->AddElement(elAs, 1);
        GaAsP->AddElement(elP, 1);
        G4Element* elCs = new G4Element("Cesium", "Cs", 55, 132.90545*g/mole);
        G4Element* elK = new G4Element("Potassium", "K", 19, 39.0983*g/mole);
        G4Element* elSb = new G4Element("Antimony", "Sb", 51, 121.76*g/mole);
        G4Material * bialkali = new G4Material("Bialkali", 3.29*g/cm3, 3, kStateSolid, 293*kelvin, 1*pow(10,-6)*pascal);
        bialkali->AddElement(elCs,1);
        bialkali->AddElement(elK,2);
        bialkali->AddElement(elSb,1);

        G4bool checkOverlaps = true;

        G4double world_size_x = 5.*m;
        G4double world_size_z = 10.*m;

        G4Box* solidWorld = new G4Box("World", world_size_x, world_size_x, world_size_z);

        G4LogicalVolume* logicWorld =
                new G4LogicalVolume(solidWorld, //its solid
                                    air, //its material
                                    "World"); //its name

// Make Physical volume ** NEVER CHANGE THIS **
        G4VPhysicalVolume* physWorld =
                new G4PVPlacement(0, //no rotation
                                  G4ThreeVector(), //at (0,0,0)
                                  logicWorld, //its logical volume
                                  "World", //its name
                                  0, //its mother  volume
                                  false, //no boolean operation
                                  0, //copy number
                                  false); //overlaps checking

// ********************World and Materials Complete ***********************//

// Set up Collimator
        G4double container_z_pos = container_z +water_size_x + 1.0*m;
        G4Box *solidCollimator = new G4Box("Collimator", 1*cm, water_size_y, container_z_pos - container_z);
        G4LogicalVolume * logicCollimator = new G4LogicalVolume(solidCollimator, lead, "Collimator");
        new G4PVPlacement(0, G4ThreeVector(-container_x, 0, 0),
                          logicCollimator, "CollimatorLeft", logicWorld,
                          false, 0, checkOverlaps);
        new G4PVPlacement(0, G4ThreeVector(container_x, 0, 0),
                          logicCollimator, "CollimatorRight", logicWorld,
                          false, 0, checkOverlaps);
// Set up shipping container environment (8ft wide and 8.5ft high)
        G4double c_thick = 0.1905*cm; // approx 0.075 inch thick
        G4Box *solidContainer = new G4Box("Container", container_x, container_y, container_z);
        G4LogicalVolume *logicContainer = new G4LogicalVolume(solidContainer, steel, "Container");
        new G4PVPlacement(0,
                          G4ThreeVector(0, 0, container_z_pos),
                          logicContainer, "Container",logicWorld,
                          false,0,checkOverlaps);

// make container hollow
        G4Box *hollowContainer = new G4Box("ContainerAir", container_x -c_thick, container_y -c_thick, container_z -c_thick);
        G4LogicalVolume *logicHollowC = new G4LogicalVolume(hollowContainer, air, "hollowContainer");
        new G4PVPlacement(0, G4ThreeVector(),
                          logicHollowC, "HollowContainer",logicContainer, false,0,checkOverlaps);

        G4Sphere* solidIntObj = new G4Sphere("InterogationObject", 0, IntObj_rad, 0, 2*pi, 0, pi);

        G4float U238_abundance = 100*perCent - radio_abundance;
        U->AddIsotope(Uranium235, radio_abundance);
        U->AddIsotope(Uranium238, U238_abundance);
        natU->AddIsotope(Uranium235, 0.0072);
        natU->AddIsotope(Uranium238, 1 - 0.0072);
        G4float Pu240_abundance = 100*perCent - radio_abundance;
        WGPu->AddIsotope(Plutonium239, radio_abundance);
        WGPu->AddIsotope(Plutonium240, Pu240_abundance);
        Pu->AddIsotope(Plutonium240, 0.9999);
        Pu->AddIsotope(Plutonium239, 1 - 0.9999);

        G4Material* intObjMat = new G4Material("IntObjMaterial", intObjDensity, 1);
        if(IntObj_Selection == "Uranium")
        {
                intObjMat->AddElement(U,1);
        }
        else if(IntObj_Selection == "Plutonium")
        {
                intObjMat->AddElement(WGPu,1);
        }
        else if(IntObj_Selection == "Lead")
        {
          intObjMat->AddElement(elPb,1);
        }
        else{std::cerr << "ERROR: Interogation Material not found."<<std::endl;}
        G4LogicalVolume* logicIntObj = new G4LogicalVolume(solidIntObj, intObjMat,"IntObjLogicVolume");

        setEndIntObj(container_z_pos, container_z);

        physIntObj = new G4PVPlacement(0,
                                       G4ThreeVector(intObj_x_pos, intObj_y_pos, intObj_z_pos),
                                       logicIntObj, "IntObjPhysical", logicHollowC, false,
                                       0, checkOverlaps);

// ************************** Begin Detector Construction **************************//

// Make Water Casing (Plexiglass)

G4Box* solidCasing = new G4Box("Encasing", water_size_x, water_size_y, water_size_z);
G4Material *plexiglass = new G4Material("Plexiglass", 1.19*g/cm3, 3);
plexiglass->AddElement(elC, 5);
plexiglass->AddElement(elH, 8);
plexiglass->AddElement(elO, 2);
G4LogicalVolume* logicCasing = new G4LogicalVolume(solidCasing, plexiglass, "Encasing");

G4double water_z_pos = container_z_pos - container_z;
G4double myangle = (180. - theAngle)*pi/180.;
G4double water_x_pos = tan(myangle)*(container_z_pos + intObj_z_pos - water_z_pos);
G4double detDistance = water_x_pos/sin(myangle) + water_size_z;
G4RotationMatrix* waterRot = new G4RotationMatrix;
waterRot->rotateY((180. - theAngle)*deg);
G4RotationMatrix* waterRot2 = new G4RotationMatrix;
waterRot2->rotateY((180. + theAngle)*deg);

new G4PVPlacement(waterRot,
G4ThreeVector(water_x_pos,0,water_z_pos), logicCasing,
"EncasingLeft", logicWorld, false, 0, checkOverlaps);
new G4PVPlacement(waterRot2,
G4ThreeVector(-1*water_x_pos,0,water_z_pos), logicCasing,
"EncasingRight", logicWorld, false, 0, checkOverlaps);

G4double plexiThickness = 0.18*mm; //0.18*mm;
// Make Teflon tape wrap
G4double tapeThick = 0.01*cm;
G4VSolid* solidTape = new G4Box("Tape", water_size_x-plexiThickness, water_size_y-plexiThickness, water_size_z-plexiThickness);
G4Material *teflonTape = nist->FindOrBuildMaterial("G4_TEFLON");
G4LogicalVolume* logicTape = new G4LogicalVolume(solidTape, teflonTape, "Tape");
physTape = new G4PVPlacement(0,G4ThreeVector(0, 0, 0), logicTape, "Tape", logicCasing, false, 0, checkOverlaps);

// Tub of water

G4Box* solidWater = new G4Box("Water", water_size_x - plexiThickness - tapeThick, water_size_y- plexiThickness - tapeThick, water_size_z- plexiThickness - tapeThick);
        G4LogicalVolume* logicWater =
                new G4LogicalVolume(solidWater, //its solid
                                    Water, //its material
                                    "Water"); //its name

physWater = new G4PVPlacement(0,         //no rotation
                                  G4ThreeVector(0,0,0),
                                  logicWater, //its logical volume
                                  "Water", //its name
                                  logicTape, //its mother logical volume
                                  false, //no boolean operation
                                  0, //copy number
                                  checkOverlaps); //overlaps checking

// Set up chopper wheel

        if(chopper_z > water_z_pos)
        {
                std::cerr << "ERROR: Chopper wheel location should be behind water detectors, exiting." << std::endl;
                exit(100);
        }
        G4Tubs *solidChopper = new G4Tubs("Chopper", 0*cm, 10*cm, chopper_thick, 0.*deg, 180.*deg);
        G4Material *chopperMat = new G4Material("chopperMaterial", chopperDensity, 1);
        if(chopperOn)
        {
          if(chopperDensity == 19.1*g/cm3)
          {
            chopperMat->AddElement(U,1);
          }
          else if(chopperDensity == 19.6*g/cm3)
          {
            chopperMat->AddElement(WGPu, 1);
          }
          else{std::cerr << "ERROR chopperDensity not found!" << std::endl; exit(100);}
        }
        else
        {
          if(chopperDensity == 19.1*g/cm3)
          {
            chopperMat->AddElement(natU,1);
          }
          else if(chopperDensity == 19.6*g/cm3)
          {
            chopperMat->AddElement(Pu,1);
          }
          else{std::cerr << "ERROR: Chopper Density not found." << std::endl; exit(100);}
        }
        logicChopper = new G4LogicalVolume(solidChopper, chopperMat, "Chopper");
        new G4PVPlacement(0, G4ThreeVector(0, -2.5*cm,chopper_z),
                          logicChopper, "Chopper", logicWorld, false,
                          0, checkOverlaps);

        G4double PMT_rmin = 0*cm;
        G4double PMT_z = 7.62*cm; // 3 in PMT
        G4Tubs* solidPMT = new G4Tubs("PMT", PMT_rmin, PMT_rmax, PMT_z, 0*deg, 360*deg);
        logicPMT = new G4LogicalVolume(solidPMT, PMT_mat, "PMT");
        G4double PMT_y_pos;
        std::vector<G4double> PMT_y_posv;
        if(nPMT>1)
        {
                if(PMT_rmax*2 > water_size_y/(nPMT+1))
                {
                        std::cerr << "ERROR Too many PMTs to fit on Water Surface!" << std::endl;
                        exit(10);
                }
                for(G4int i=1; i<=nPMT; i++)
                {
                        PMT_y_pos = 0. - water_size_y/2 + i*(water_size_y/(nPMT + 1));
                        PMT_y_posv.push_back(PMT_y_pos);
                }
        }
        else
        {
                PMT_y_posv.push_back(0);
        }

        for(G4int k=1; k<=nPMT; k++)
        {

                new G4PVPlacement(0,
                                  G4ThreeVector(0, PMT_y_posv[k-1], -water_size_x/2 + PMT_z - plexiThickness - tapeThick),
                                  logicPMT,
                                  "PMT",
                                  logicWater,
                                  false,
                                  k,
                                  checkOverlaps);
        }

// Make Solid PhotoCathode
        G4double PC_z = 20*nm;
        if(pc_mat == "GaAsP")
        {
                PC_mat = GaAsP;
        }
        else if(pc_mat == "Bialkali")
        {
                PC_mat = bialkali;
        }
        else exit(1);

        G4Tubs* solidPhotoCathode = new G4Tubs("PC", PMT_rmin, PMT_rmax, PC_z, 0*deg, 360.*deg);
        logicPC = new G4LogicalVolume(solidPhotoCathode, PC_mat, "PC");
        G4double PMT_window_thickness = 3*mm;
        physPC = new G4PVPlacement(0,
                                   G4ThreeVector(0,0,PMT_z-PMT_window_thickness),
                                   logicPC,
                                   "PC",
                                   logicPMT, // daughter of PMT logical
                                   false,
                                   0,
                                   checkOverlaps);

// Visualization Attributes
        G4VisAttributes *yellow= new G4VisAttributes( G4Colour(255/255.,255/255.,51/255. ));
        G4VisAttributes *red= new G4VisAttributes( G4Colour(255/255., 0/255., 0/255. ));
        G4VisAttributes *blue= new G4VisAttributes( G4Colour(0/255., 0/255.,  255/255. ));
        G4VisAttributes *grayc= new G4VisAttributes( G4Colour(128/255., 128/255.,  128/255. ));
        G4VisAttributes *lightGray= new G4VisAttributes( G4Colour(178/255., 178/255.,  178/255. ));
        G4VisAttributes *green= new G4VisAttributes( G4Colour(0/255., 255/255.,  0/255. ));
        G4VisAttributes *black = new G4VisAttributes(G4Colour(0.,0.,0.));
        G4VisAttributes *magenta = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0));
        G4VisAttributes *white = new G4VisAttributes(G4Colour(1.0,1.0,1.0));

        yellow->SetVisibility(true);
        yellow->SetForceWireframe(true);
        red->SetVisibility(true);
        red->SetForceSolid(true);
        blue->SetVisibility(true);
        blue->SetForceWireframe(true);
        green->SetVisibility(true);
        green->SetForceSolid(true);
        grayc->SetVisibility(false);
        lightGray->SetVisibility(true);
        lightGray->SetForceSolid(true);
        black->SetForceSolid(true);
        black->SetVisibility(true);
        magenta->SetForceWireframe(true);
        magenta->SetVisibility(true);
        white->SetForceWireframe(true);
        white->SetVisibility(true);

// Set Visual colors
        logicTape->SetVisAttributes(yellow);
        logicCasing->SetVisAttributes(magenta);
        logicWater->SetVisAttributes(blue);
        logicPMT->SetVisAttributes(green);
        logicPC->SetVisAttributes(red);
        logicIntObj->SetVisAttributes(lightGray);
        logicContainer->SetVisAttributes(yellow);
        logicHollowC->SetVisAttributes(grayc);
        logicCollimator->SetVisAttributes(magenta);
        logicChopper->SetVisAttributes(black);


//
// ------------ Generate & Add Material Properties Table ------------
//

// ************************** WATER ********************************//
        G4double photonEnergy[] =
        { 2.034*eV, 2.068*eV, 2.103*eV, 2.139*eV,
          2.177*eV, 2.216*eV, 2.256*eV, 2.298*eV,
          2.341*eV, 2.386*eV, 2.433*eV, 2.481*eV,
          2.532*eV, 2.585*eV, 2.640*eV, 2.697*eV,
          2.757*eV, 2.820*eV, 2.885*eV, 2.954*eV,
          3.026*eV, 3.102*eV, 3.181*eV, 3.265*eV,
          3.353*eV, 3.446*eV, 3.545*eV, 3.649*eV,
          3.760*eV, 3.877*eV, 4.002*eV, 4.136*eV };

        const G4int nEntries = sizeof(photonEnergy)/sizeof(G4double);

        G4double photonEnergy2[] =
        {1.72*eV, 1.746197183*eV, 1.771142857*eV, 1.796811594*eV,1.823235294*eV,
         1.850447761*eV, 1.878484848*eV, 1.907384615*eV, 1.9371875*eV,
         1.967936508*eV, 1.999677419*eV, 2.032459016*eV, 2.066333333*eV,
         2.101355932*eV,2.137586207*eV, 2.175087719*eV, 2.213928571*eV, 2.254181818*eV,
         2.295925926*eV, 2.339245283*eV, 2.384230769*eV, 2.430980392*eV, 2.4796*eV,
         2.530204082*eV, 2.582916667*eV, 2.63787234*eV, 2.695217391*eV,
         2.755111111*eV, 2.817727273*eV, 2.883255814*eV, 2.951904762*eV,
         3.023902439*eV, 3.0995*eV, 3.178974359*eV, 3.262631579*eV,
         3.542285714*eV, 4.132666667*eV, 4.508363636*eV, 4.9592*eV
        };

// Water need to add rayleigh scattering! -- added automatically if material name = Water
//
        G4double refractiveIndex1[] =
        { 1.3435, 1.344,  1.3445, 1.345,  1.3455,
          1.346,  1.3465, 1.347,  1.3475, 1.348,
          1.3485, 1.3492, 1.35,   1.3505, 1.351,
          1.3518, 1.3522, 1.3530, 1.3535, 1.354,
          1.3545, 1.355,  1.3555, 1.356,  1.3568,
          1.3572, 1.358,  1.3585, 1.359,  1.3595,
          1.36,   1.3608};

        assert(sizeof(refractiveIndex1) == sizeof(photonEnergy));

        G4double absorption[] =
        {0.812347685*m, 1.209189843*m, 1.602564103*m, 1.937984496*m, 2.150537634*m,
         2.277904328*m, 2.43902439*m, 2.941176471*m, 3.217503218*m, 3.429355281*m,
         3.629764065*m, 3.78214826*m, 4.496402878*m, 7.4019245*m, 11.16071429*m, 14.38848921*m,
         16.15508885*m, 17.69911504*m, 21.09704641*m, 23.04147465*m, 24.44987775*m,
         30.76923077*m, 49.01960784*m,66.66666667*m,78.74015748*m,94.33962264*m,
         102.145046*m, 108.4598698*m, 157.480315*m, 202.020202*m, 220.2643172*m,
         211.4164905*m, 150.8295626*m, 117.5088132*m, 87.95074758*m,
         60.60606061*m,18.11594203*m,11.79245283*m,9.652509653*m};

        assert(sizeof(absorption) == sizeof(photonEnergy2));

        G4double scintilFast[] =
        { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
          1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
          1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
          1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
          1.00, 1.00, 1.00, 1.00 };

        assert(sizeof(scintilFast) == sizeof(photonEnergy));

        G4double scintilSlow[] =
        { 0.01, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00,
          7.00, 8.00, 9.00, 8.00, 7.00, 6.00, 4.00,
          3.00, 2.00, 1.00, 0.01, 1.00, 2.00, 3.00,
          4.00, 5.00, 6.00, 7.00, 8.00, 9.00, 8.00,
          7.00, 6.00, 5.00, 4.00 };

        assert(sizeof(scintilSlow) == sizeof(photonEnergy));

        G4MaterialPropertiesTable* waterMPT = new G4MaterialPropertiesTable();

        waterMPT->AddProperty("RINDEX", photonEnergy, refractiveIndex1,nEntries)->SetSpline(true);
        waterMPT->AddProperty("ABSLENGTH",photonEnergy2, absorption, nEntries)->SetSpline(true);
        waterMPT->AddProperty("FASTCOMPONENT",photonEnergy, scintilFast, nEntries)->SetSpline(true);
        waterMPT->AddProperty("SLOWCOMPONENT",photonEnergy, scintilSlow, nEntries)->SetSpline(true);

        waterMPT->AddConstProperty("SCINTILLATIONYIELD",50./MeV);
        waterMPT->AddConstProperty("RESOLUTIONSCALE",1.0);
        waterMPT->AddConstProperty("FASTTIMECONSTANT", 1.*ns);
        waterMPT->AddConstProperty("SLOWTIMECONSTANT",10.*ns);
        waterMPT->AddConstProperty("YIELDRATIO",0.8);

        G4double energy_water[] = {
                1.56962*eV, 1.58974*eV, 1.61039*eV, 1.63157*eV,
                1.65333*eV, 1.67567*eV, 1.69863*eV, 1.72222*eV,
                1.74647*eV, 1.77142*eV, 1.7971 *eV, 1.82352*eV,
                1.85074*eV, 1.87878*eV, 1.90769*eV, 1.93749*eV,
                1.96825*eV, 1.99999*eV, 2.03278*eV, 2.06666*eV,
                2.10169*eV, 2.13793*eV, 2.17543*eV, 2.21428*eV,
                2.25454*eV, 2.29629*eV, 2.33962*eV, 2.38461*eV,
                2.43137*eV, 2.47999*eV, 2.53061*eV, 2.58333*eV,
                2.63829*eV, 2.69565*eV, 2.75555*eV, 2.81817*eV,
                2.88371*eV, 2.95237*eV, 3.02438*eV, 3.09999*eV,
                3.17948*eV, 3.26315*eV, 3.35134*eV, 3.44444*eV,
                3.54285*eV, 3.64705*eV, 3.75757*eV, 3.87499*eV,
                3.99999*eV, 4.13332*eV, 4.27585*eV, 4.42856*eV,
                4.59258*eV, 4.76922*eV, 4.95999*eV, 5.16665*eV,
                5.39129*eV, 5.63635*eV, 5.90475*eV, 6.19998*eV
        };

        const G4int numentries_water = sizeof(energy_water)/sizeof(G4double);

        G4double energy_ray_water[] = {
                1.56962*eV, 1.58974*eV, 1.61039*eV, 1.63157*eV,
                1.65333*eV, 1.67567*eV, 1.69863*eV, 1.72222*eV,
                1.74647*eV, 1.77142*eV, 1.7971 *eV, 1.82352*eV,
                1.85074*eV, 1.87878*eV, 1.90769*eV, 1.93749*eV,
                1.96825*eV, 1.99999*eV, 2.03278*eV, 2.06666*eV,
                2.10169*eV, 2.13793*eV, 2.17543*eV, 2.21428*eV,
                2.25454*eV, 2.29629*eV, 2.33962*eV, 2.38461*eV,
                2.43137*eV, 2.47999*eV, 2.53061*eV, 2.58333*eV
        }; // 32 entries



        const G4int numentries_water_ray = sizeof(energy_ray_water)/sizeof(G4double);

        G4double mie_water[] = {
                167024.4*m, 158726.7*m, 150742  *m,
                143062.5*m, 135680.2*m, 128587.4*m,
                121776.3*m, 115239.5*m, 108969.5*m,
                102958.8*m, 97200.35*m, 91686.86*m,
                86411.33*m, 81366.79*m, 76546.42*m,
                71943.46*m, 67551.29*m, 63363.36*m,
                59373.25*m, 55574.61*m, 51961.24*m,
                48527.00*m, 45265.87*m, 42171.94*m,
                39239.39*m, 36462.50*m, 33835.68*m,
                31353.41*m, 29010.30*m, 26801.03*m,
                24720.42*m, 22763.36*m, 20924.88*m,
                19200.07*m, 17584.16*m, 16072.45*m,
                14660.38*m, 13343.46*m, 12117.33*m,
                10977.70*m, 9920.416*m, 8941.407*m,
                8036.711*m, 7202.470*m, 6434.927*m,
                5730.429*m, 5085.425*m, 4496.467*m,
                3960.210*m, 3473.413*m, 3032.937*m,
                2635.746*m, 2278.907*m, 1959.588*m,
                1675.064*m, 1422.710*m, 1200.004*m,
                1004.528*m, 833.9666*m, 686.1063*m
        };

        assert(sizeof(mie_water) == sizeof(energy_water));

// gforward, gbackward, forward backward ratio
        G4double mie_water_const[3]={0.99,0.99,0.8};

        waterMPT->AddProperty("MIEHG",energy_water,mie_water,numentries_water)->SetSpline(true);
        waterMPT->AddConstProperty("MIEHG_FORWARD",mie_water_const[0]);
        waterMPT->AddConstProperty("MIEHG_BACKWARD",mie_water_const[1]);
        waterMPT->AddConstProperty("MIEHG_FORWARD_RATIO",mie_water_const[2]);

        G4double rayleigh_water[] = {
                2335.5*m, 2210.5*m, 2090.8*m, 1976.4*m, 1866.8*m, 1762.1*m,
                1662.1*m, 1566.6*m, 1475.4*m, 1388.5*m, 1305.6*m, 1224.6*m,
                1146.8*m, 1075.6*m, 1007.8*m, 941.23*m, 880.95*m, 821.12*m,
                766.36*m, 714.50*m, 665.40*m, 618.97*m, 575.11*m, 533.69*m,
                493.46*m, 457.10*m, 421.51*m, 389.06*m, 358.57*m, 329.98*m,
                303.17*m, 277.43*m

        }; // 32 entries

// Note to Self: Never have spline set to true if rayleigh water goes to zero. Creates seg fault
        assert(sizeof(rayleigh_water) == sizeof(energy_ray_water));
        waterMPT->AddProperty("RAYLEIGH",energy_ray_water,rayleigh_water,numentries_water_ray)->SetSpline(true);
        Water->SetMaterialPropertiesTable(waterMPT);
// Set the Birks Constant for the Water scintillator
        Water->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

// APPLY OPTICAL QUALITIES

/* ************************************************************************* */
// Water Tank Properties

G4MaterialPropertiesTable* casingMPT = new G4MaterialPropertiesTable();
G4MaterialPropertiesTable* casingOPMPT = new G4MaterialPropertiesTable();
// add all of the properties here for plexiglass
G4double ephotonPlexi[] = {1.1808067*eV,1.2336786*eV,1.298269*eV,1.3699967*eV,1.4501135*eV,1.5401826*eV,1.6421815*eV,1.7586482*eV,1.8928962*eV,2.049334*eV,
                           2.2339586*eV,2.4551426*eV, 2.7249385*eV,2.850223*eV,3.0*eV,4.0*eV};
const G4int numPlexi = sizeof(ephotonPlexi)/sizeof(G4double);
G4double plexiReflectivity[] = {0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02}; // transparent
G4double plexiTrans[] = {.98,.98,.98,.98,.98,.98,.98,.98,.98,.98,.98,.98,.98,.98,.98,.98};
G4double plexiIOF[] = {1.4813,1.482,1.4827,1.4832,1.4837,1.4842,1.485,1.4862,1.488,1.4904,1.4933,1.4962,1.4996,1.5027,1.5027,1.5027};
G4double plexiEff[] = {0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
G4double plexiAbs[] = {1000.0*m,1000.0*m,1000.0*m,1000.0*m,1000.0*m,1000.0*m,1000.0*m,1000.0*m,30.0*m,30.0*m,11.4*m,11.4*m,7.3*m,7.3*m, 5.0*m, 1.0*mm}; // pretty much transparent until UV energy
assert(sizeof(plexiIOF) == sizeof(ephotonPlexi));
assert(sizeof(plexiReflectivity) == sizeof(ephotonPlexi));
assert(sizeof(plexiTrans) == sizeof(ephotonPlexi));
assert(sizeof(plexiEff) == sizeof(ephotonPlexi));
assert(sizeof(plexiAbs) == sizeof(ephotonPlexi));
G4OpticalSurface *casing_opsurf = new G4OpticalSurface("casingSurface", glisur, polished, dielectric_dielectric);
casingOPMPT->AddProperty("REFLECTIVITY", ephotonPlexi, plexiReflectivity, numPlexi);
casingOPMPT->AddProperty("TRANSMITTANCE", ephotonPlexi, plexiTrans, numPlexi);
casingMPT->AddProperty("RINDEX", ephotonPlexi, plexiIOF, numPlexi);
casingOPMPT->AddProperty("RINDEX", ephotonPlexi, plexiIOF, numPlexi);
casingOPMPT->AddProperty("EFFICIENCY", ephotonPlexi, plexiEff, numPlexi);
casingMPT->AddProperty("ABSLENGTH", ephotonPlexi, plexiAbs, numPlexi);
casingOPMPT->AddProperty("ABSLENGTH", ephotonPlexi, plexiAbs, numPlexi);
plexiglass->SetMaterialPropertiesTable(casingMPT);
casing_opsurf->SetMaterialPropertiesTable(casingOPMPT);
new G4LogicalSkinSurface("casing_surf", logicCasing, casing_opsurf);

// Add properties for Teflon Tape
G4MaterialPropertiesTable* tapeMPT = new G4MaterialPropertiesTable();
G4MaterialPropertiesTable* tapeOPMPT = new G4MaterialPropertiesTable();
G4double ephotonTape[] = {1.1427161*eV,1.2915073*eV,1.4586435*eV,1.6984206*eV,2.0160114*eV,2.5303*eV,3.262755*eV, 4.0*eV};
const G4int numTape = sizeof(ephotonTape)/sizeof(G4double);
G4double tapeReflectivity[] = {.98,.98,.98,.98,.98,.98,.98,.98};
G4double tapeTrans[] = {0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02};
G4double tapeEff[] = {0.,0.,0.,0.,0.,0.,0.,0.};
G4double tapeIOF[] = {1.32,1.32,1.32,1.32,1.32,1.32,1.33,1.33};
G4double tapeAbs[] = {1000*m, 1000*m,1000*m,1000*m,1000*m,1000*m,1000*m,1000*m}; // setting absorption length arbitrarily large, don't want teflon to kill opt photons
assert(sizeof(tapeReflectivity) == sizeof(ephotonTape));
assert(sizeof(tapeEff) == sizeof(ephotonTape));
assert(sizeof(tapeIOF) == sizeof(ephotonTape));
assert(sizeof(tapeAbs) == sizeof(ephotonTape));
assert(sizeof(tapeTrans) == sizeof(ephotonTape));
G4OpticalSurface *tape_opsurf = new G4OpticalSurface("tapeSurface",glisur, polishedfrontpainted, dielectric_dielectric);
tapeOPMPT->AddProperty("REFLECTIVITY", ephotonTape, tapeReflectivity, numTape);
tapeOPMPT->AddProperty("TRANSMITTANCE", ephotonTape, tapeTrans, numTape);
tapeMPT->AddProperty("RINDEX", ephotonTape, tapeIOF, numTape);
tapeOPMPT->AddProperty("EFFICIENCY", ephotonTape, tapeEff, numTape);
tapeMPT->AddProperty("ABSLENGTH", ephotonTape, tapeAbs, numTape);
tape_opsurf->SetMaterialPropertiesTable(tapeOPMPT);
teflonTape->SetMaterialPropertiesTable(tapeMPT);
new G4LogicalBorderSurface("tape_surf", physWater, physTape, tape_opsurf);

// *************************** Detector (PMT and PC) ***************************//
// PMT
// wavelengths 2.5, 2.0,1.5 1, .75, .5 ,.45, .4, .35, .30
        G4double ephotonPMT[] = {0.4959388*eV, 0.6199*eV, 0.8265*eV, 1.239*eV, 1.653*eV, 2.480*eV,
                                 2.755*eV, 3.10*eV, 3.54*eV, 4.133*eV};
        const G4int num = sizeof(ephotonPMT)/sizeof(G4double);
        G4double ephotonPMT2[] = {1.7711*eV, 1.9074*eV, 2.0663*eV, 2.2542*eV, 2.4796*eV,
                                  2.7551*eV, 3.0995*eV, 3.5423*eV, 4.133*eV, 4.428*eV};
        const G4int num2 = sizeof(ephotonPMT2)/sizeof(G4double);
        G4double ephotonPMT3[] = {1.823*eV, 1.8644316*eV, 1.907457*eV, 1.952515*eV,
                                  2.0*eV, 2.049*eV, 2.101*eV, 2.156*eV, 2.2140125*eV, 2.2749*eV, 2.339*eV,
                                  2.407*eV, 2.480*eV,2.56*eV,2.6379*eV, 2.725*eV, 2.817*eV, 2.917*eV,
                                  3.024017*eV, 3.138853*eV, 3.262755*eV};
        const G4int num3 = sizeof(ephotonPMT3)/sizeof(G4double);


// PMT Surface Properties
        G4double reflectivity[] = {0.03822, 0.0392,  0.040,  0.041,
                                   0.0415,  0.0428,  0.0433,  0.0440, 0.0451, 0.0469 };
G4double transmittance[] = {0.962, 0.961, 0.96, 0.959, 0.959, 0.958, 0.957,0.956,0.955, 0.954};
        assert(sizeof(reflectivity) == sizeof(ephotonPMT));
assert(sizeof(transmittance) == sizeof(ephotonPMT2));
        G4double efficiency[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
        assert(sizeof(efficiency) == sizeof(ephotonPMT));
        G4double pmt_iof[] = {1.486, 1.4945, 1.5013, 1.5075, 1.5118, 1.5214, 1.5253, 1.5308, 1.5392, 1.5528};
        assert(sizeof(pmt_iof) == sizeof(ephotonPMT));
        G4double abslength[] = {1000*m,1000*m,1000*m,1000*m,1000*m,1000*m,1000*m,1000*m,1000*m, 1000*m}; // treating glass as transparent
        assert(sizeof(abslength) == sizeof(ephotonPMT));
        G4OpticalSurface* PMT_opsurf = new G4OpticalSurface("PMTSurface",unified,polished,dielectric_metal);
        G4MaterialPropertiesTable* PMTopt = new G4MaterialPropertiesTable();
        G4MaterialPropertiesTable* PMTphysabs = new G4MaterialPropertiesTable();
        PMTopt->AddProperty("REFLECTIVITY", ephotonPMT, reflectivity, num)->SetSpline(true);
PMTopt->AddProperty("TRANSMITTANCE", ephotonPMT2, transmittance, num2);
        PMTopt->AddProperty("EFFICIENCY", ephotonPMT, efficiency, num);
        PMTopt->AddProperty("RINDEX", ephotonPMT, pmt_iof, num)->SetSpline(true);
        PMTphysabs->AddProperty("RINDEX", ephotonPMT, pmt_iof, num)->SetSpline(true);
        PMTphysabs->AddProperty("ABSLENGTH", ephotonPMT, abslength, num);
        PMT_opsurf->SetMaterialPropertiesTable(PMTopt);
        PMT_mat->SetMaterialPropertiesTable(PMTphysabs);

// Photocathode surface properties
        G4double Ga_As_photocath_EFF[]={0.25*perCent,36.246*perCent,39.8*perCent,40.0*perCent,36.0*perCent,30.0*perCent,
                                        24.0*perCent, 15.0*perCent,4.8*perCent, 0.6*perCent}; //Enables 'detection' of photons
        assert(sizeof(Ga_As_photocath_EFF) == sizeof(ephotonPMT2));
        G4double Ga_As_photocath_ReR[]={3.346,3.3588,3.3877,3.4857,3.6209,4.0919,4.6678,4.6102,3.8200, 3.7322};
        assert(sizeof(Ga_As_photocath_ReR) == sizeof(ephotonPMT));
        G4double Ga_As_photocath_ImR[]={0.0056,0.0071,0.0096,0.01523, 0.106, 0.41247, 1.0094, 2.0488, 2.0051, 1.9908};
        assert(sizeof(Ga_As_photocath_ImR) == sizeof(ephotonPMT));
        G4double bialkali_photocath_EFF[] = {0.015*perCent, 0.2*perCent, 3.0*perCent, 7.0*perCent, 18.5*perCent, 25.0*perCent,
                                             28.0*perCent, 26.0*perCent, 6.0*perCent, 0.19*perCent};
        assert(sizeof(bialkali_photocath_EFF) == sizeof(ephotonPMT2));
        G4double bialkali_photocath_ReR[] = {2.96,2.95,2.95,2.95,2.96, 2.98, 3.01, 3.06,
                                             3.12, 3.20, 3.26, 3.09, 3.0,3.0,3.0,2.87,2.7,2.61,2.38,2.18,1.92};
        assert(sizeof(bialkali_photocath_ReR) == sizeof(ephotonPMT3));
        G4double bialkali_photocath_ImR[] = {0.33,0.34,0.34,0.35,0.37,0.38,0.42,0.46,
                                             0.53,0.63,0.86,1.05,1.06,1.11,1.34,1.44,1.50,1.53,1.71,1.69,1.69};
        assert(sizeof(bialkali_photocath_ImR) == sizeof(ephotonPMT3));
        G4MaterialPropertiesTable* photocath_mt = new G4MaterialPropertiesTable();
        if(pc_mat == "GaAsP")
        {
                photocath_mt->AddProperty("EFFICIENCY", ephotonPMT2,Ga_As_photocath_EFF, num2)->SetSpline(true);
                photocath_mt->AddProperty("REALRINDEX", ephotonPMT,Ga_As_photocath_ReR,num)->SetSpline(true);
                photocath_mt->AddProperty("IMAGINARYRINDEX", ephotonPMT, Ga_As_photocath_ImR, num)->SetSpline(true);
        }
        else if(pc_mat == "Bialkali")
        {
                photocath_mt->AddProperty("EFFICIENCY", ephotonPMT2,bialkali_photocath_EFF, num)->SetSpline(true);
                photocath_mt->AddProperty("REALRINDEX", ephotonPMT3,bialkali_photocath_ReR,num3)->SetSpline(true);
                photocath_mt->AddProperty("IMAGINARYRINDEX", ephotonPMT3, bialkali_photocath_ImR, num3)->SetSpline(true);
        }

        G4OpticalSurface* photocath_opsurf= new G4OpticalSurface("photocath_opsurf");
        photocath_opsurf->SetType(dielectric_metal);
        photocath_opsurf->SetFinish(polished);
        photocath_opsurf->SetPolish(1.0);
        photocath_opsurf->SetModel(glisur);
        photocath_opsurf->SetMaterialPropertiesTable(photocath_mt);

        new G4LogicalSkinSurface("photocath_surf", logicPC, photocath_opsurf); // name, physical volume of surface, phsical volume of world?, G4optical surface
new G4LogicalSkinSurface("PMT_surf", logicPMT, PMT_opsurf);
// Air
        G4double refractiveIndex2[] =
        { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
          1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
          1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
          1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
          1.00, 1.00, 1.00, 1.00 };

        G4MaterialPropertiesTable* airMPT = new G4MaterialPropertiesTable();
        airMPT->AddProperty("RINDEX", photonEnergy, refractiveIndex2, nEntries);

        air->SetMaterialPropertiesTable(airMPT);
//
//always return the physical World!!!
//
        return physWorld;
}
