#include "mpi.h"
#include <stdio.h>
#include <string>
#include <cmath>
#include <ctime>
#include <iostream>
#include <complex>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <sstream>

#include "Setup.h"
#include "Init.h"
#include "Random.h"
#include "FFT.h"
#include "Parameters.h"
#include "Matrix.h"
#include "Lattice.h"
#include "Evolution.h"
#include "Spinor.h"
#include "MyEigen.h"
#include "pretty_ostream.h"

#define _SECURE_SCL 0
#define _HAS_ITERATOR_DEBUGGING 0
using namespace std;

int readInput(Setup *setup, Parameters *param, int argc, char *argv[],
              int rank);
void display_logo();

// main program 1
int main (int argc, char *argv[]) {
    int rank;
    int size;

    int nev = 1;
    if (argc == 3) {
        nev = atoi(argv[2]);
    }

    //initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);  // get current process id
    MPI_Comm_size (MPI_COMM_WORLD, &size);  // get number of processes

    int h5Flag = 0;
    pretty_ostream messager;

    Parameters *param = new Parameters();
    param->setMPIRank(rank);
    param->setMPISize(size);

    Setup *setup;
    setup = new Setup();
    // read parameters from file
    readInput(setup, param, argc, argv, rank);

    // initialize random generator using time and seed from input file
    Random *random = new Random();
    unsigned long long int rnum;
    if (param->getUseSeedList() == 0) {
        if (param->getUseTimeForSeed() == 1) {
            rnum = time(0) + param->getSeed()*10000;
        } else {
            rnum = param->getSeed();
            messager << "Random seed = " << rnum+(rank*1000)
                     << " - entered directly +rank*1000.";
            messager.flush("info");
        }
        param->setRandomSeed(rnum + rank*1000);
        if (param->getUseTimeForSeed() == 1) {
            messager << "Random seed = " << param->getRandomSeed()
                     << " made from time "
                     << rnum-param->getSeed()-(rank*1000)
                     << " and argument (+1000*rank) "
                     << param->getSeed()+(rank*1000);
            messager.flush("info");
        }
        random->init_genrand64(rnum+rank*1000);
    } else {
        ifstream fin;
        fin.open("seedList");
        unsigned long long int seedList[size];
        if (fin) {
            for (int i = 0; i < size; i++) {
                if (!fin.eof()) {
                    fin >> seedList[i];
                } else {
                    cerr << "Error: Not enough random seeds for the number of "
                         << "processors selected. Exiting." << endl;
                    exit(1);
                }
            }
        } else {
            cerr << "Random seed file 'seedList' not found. Exiting." << endl;
            exit(1);
        }
        fin.close();
        param->setRandomSeed(seedList[rank]);
        random->init_genrand64(seedList[rank]);
        messager << "Random seed on rank " << rank << " = "
                 << seedList[rank] << " read from list.";
        messager.flush("info");
    }

    // event loop starts ...
    for (int iev = 0; iev < nev; iev++) {
        messager << "Generating event " << iev+1 << " out of "
                 << nev << " ...";
        messager.flush("info");
        // welcome
        if (rank == 0)
            display_logo();

        // initialize helper class objects
        MyEigen *myeigen = new MyEigen();

        param->setEventId(rank + iev*size);
        param->setSuccess(0);

        int nn[2];
        nn[0]=param->getSize();
        nn[1]=param->getSize();

        stringstream strup_name;
        strup_name << "usedParameters" << param->getEventId() << ".dat";
        string up_name;
        up_name = strup_name.str();
        ofstream fout1(up_name.c_str(),ios::app); 
        fout1 << "Random seed used on rank " << rank << ": "
              << param->getRandomSeed() << endl;
        fout1.close();

        // initialize init object
        Init *init = new Init(nn);

        // initialize group
        Group *group = new Group(param->getNc());

        // initialize Glauber class
        messager << "Init Glauber on rank " << param->getMPIRank() << " ... ";
        messager.flush("info");
        Glauber *glauber = new Glauber;
        glauber->initGlauber(param->getSigmaNN(), param->getTarget(),
                             param->getProjectile(), param->getb(), 100, rank);

        // measure and output eccentricity, triangularity
        // init->eccentricity(lat, group, param, random, glauber);

        // initialize evolution object
        Evolution *evolution = new Evolution(nn);

        // either read k_T spectrum from file or do a fresh start
        if (param->getReadMultFromFile() == 1) {
            evolution->readNkt(param);
        } else {
            // clean files
            // stringstream strNpartdNdy_name;
            // strNpartdNdy_name << "NpartdNdy" << rank << ".dat";
            // string NpartdNdy_name;
            // NpartdNdy_name = strNpartdNdy_name.str();

            // ofstream foutNN(NpartdNdy_name.c_str(),ios::out); 
            // foutNN.close();

            // stringstream strNpartdNdyH_name;
            // strNpartdNdyH_name << "NpartdNdyHadrons" << rank << ".dat";
            // string NpartdNdyH_name;
            // NpartdNdyH_name = strNpartdNdyH_name.str();

            // ofstream foutNNH(NpartdNdyH_name.c_str(),ios::out); 
            // foutNNH.close();

            // stringstream strNpartdEdy_name;
            // strNpartdEdy_name << "NpartdEdy" << param->getEventId() << ".dat";
            // string NpartdEdy_name;
            // NpartdEdy_name = strNpartdEdy_name.str();

            // ofstream foutE(NpartdEdy_name.c_str(),ios::out); 
            // foutE.close();

            // stringstream strdNdy_name;
            // strdNdy_name << "dNdy" << param->getEventId() << ".dat";
            // string dNdy_name;
            // dNdy_name = strdNdy_name.str();

            // ofstream foutN(dNdy_name.c_str(),ios::out); 
            // foutN.close();

            // stringstream strCorr_name;
            // strCorr_name << "Corr" << param->getEventId() << ".dat";
            // string Corr_name;
            // Corr_name = strCorr_name.str();

            // ofstream foutCorr(Corr_name.c_str(),ios::out); 
            // foutCorr.close();

            // stringstream strPhiMult_name;
            // strPhiMult_name << "MultPhi" << param->getEventId() << ".dat";
            // string PhiMult_name;
            // PhiMult_name = strPhiMult_name.str();

            // ofstream foutPhiMult(PhiMult_name.c_str(),ios::out); 
            // foutPhiMult.close();

            // stringstream strPhi2ParticleMult_name;
            // strPhi2ParticleMult_name << "MultPhi2Particle" << param->getEventId() << ".dat";
            // string Phi2ParticleMult_name;
            // Phi2ParticleMult_name = strPhi2ParticleMult_name.str();

            // ofstream foutPhi2ParticleMult(Phi2ParticleMult_name.c_str(),ios::out); 
            // foutPhi2ParticleMult.close();


            // stringstream strPhiMultHad_name;
            // strPhiMultHad_name << "MultPhiHadrons" << param->getEventId() << ".dat";
            // string PhiMultHad_name;
            // PhiMultHad_name = strPhiMultHad_name.str();

            // ofstream foutPhiMultHad(PhiMultHad_name.c_str(),ios::out); 
            // foutPhiMultHad.close();

            // stringstream strPhi2ParticleMultHad_name;
            // strPhi2ParticleMultHad_name << "MultPhiHadrons2Particle" << param->getEventId() << ".dat";
            // string Phi2ParticleMultHad_name;
            // Phi2ParticleMultHad_name = strPhi2ParticleMultHad_name.str();

            // ofstream foutPhi2ParticleMultHad(Phi2ParticleMultHad_name.c_str(),ios::out); 
            // foutPhi2ParticleMultHad.close();


            // stringstream strame_name;
            // strame_name << "AverageMaximalEpsilon" << param->getEventId() << ".dat";
            // string ame_name;
            // ame_name = strame_name.str();

            // ofstream foutEpsA(ame_name.c_str(),ios::out); 
            // foutEpsA.close();


            // stringstream strepsx_name;
            // strepsx_name << "eps-x" << param->getEventId() << ".dat";
            // string epsx_name;
            // epsx_name = strepsx_name.str();

            // ofstream foutEpsX(epsx_name.c_str(),ios::out); 
            // foutEpsX.close();


            // stringstream strdEdy_name;
            // strdEdy_name << "dEdy" << param->getEventId() << ".dat";
            // string dEdy_name;
            // dEdy_name = strdEdy_name.str();

            // ofstream foutdE(dEdy_name.c_str(),ios::out); 
            // foutdE.close();

            // stringstream straniso_name;
            // straniso_name << "anisotropy" << param->getEventId() << ".dat";
            // string aniso_name;
            // aniso_name = straniso_name.str();

            // ofstream foutAni(aniso_name.c_str(),ios::out); 
            // foutAni.close();

            //stringstream strecc_name;
            //strecc_name << "eccentricities" << param->getEventId() << ".dat";
            //string ecc_name;
            //ecc_name = strecc_name.str();

            //ofstream foutEcc(ecc_name.c_str(),ios::out); 
            //foutEcc.close();

            // stringstream strmult_name;
            // strmult_name << "multiplicity" << param->getEventId() << ".dat";
            // string mult_name;
            // mult_name = strmult_name.str();
            // ofstream foutmult(mult_name.c_str(),ios::out); 
            // foutmult.close();

            // stringstream strmult2_name;
            // strmult2_name << "multiplicityCorr" << param->getEventId() << ".dat";
            // string mult2_name;
            // mult2_name = strmult2_name.str();
            // ofstream foutmult2(mult2_name.c_str(),ios::out); 
            // foutmult2.close();

            // stringstream strmult3_name;
            // strmult3_name << "multiplicityCorrFromPhi" << param->getEventId() << ".dat";
            // string mult3_name;
            // mult3_name = strmult3_name.str();
            // ofstream foutmult3(mult3_name.c_str(),ios::out); 
            // foutmult3.close();

            // stringstream strmult4_name;
            // strmult4_name << "multiplicityCorrFromPhiHadrons" << param->getEventId() << ".dat";
            // string mult4_name;
            // mult4_name = strmult4_name.str();
            // ofstream foutmult4(mult4_name.c_str(),ios::out); 
            // foutmult4.close();
        }

        while (param->getSuccess() == 0) {
            param->setSuccess(0);
            // allocate lattice
            Lattice *lat;
            lat = new Lattice(param, param->getNc(), param->getSize());
            BufferLattice *bufferlat;
            bufferlat = new BufferLattice(param, param->getNc(),
                                          param->getSize());
            messager.info("Lattice generated.");


            // initialize gsl random number generator (used for non-Gaussian distributions)
            //random->gslRandomInit(rnum);

            // initialize U-fields on the lattice
            // init->init(lat, group, param, random, glauber);
            int READFROMFILE = 0;
            init->init(lat, group, param, random, glauber, READFROMFILE);
            messager.info("initialization done.");

            if (param->getSuccess()==0) {
                delete lat;
                delete bufferlat;
                continue;
            }

            delete init;
            delete glauber;

            messager.info("Start evolution");
            // do the CYM evolution of the initialized fields using parmeters in param
            evolution->run(lat, bufferlat,  group, param);
            delete bufferlat;
            delete lat;
        }

        MPI_Barrier(MPI_COMM_WORLD);

        messager.info("One event finished");
        if (param->getWriteOutputsToHDF5() == 1) {
            int status = 0;
            stringstream h5output_filename;
            h5output_filename << "RESULTS_rank" << rank;
            stringstream collect_command;
            collect_command << "python3 utilities/combine_events_into_hdf5.py ."
                            << " --output_filename "
                            << h5output_filename.str()
                            << " --event_id " << param->getEventId();
            status = system(collect_command.str().c_str());
            messager << "finished system call to python script with status: "
                     << status;
            messager.flush("info");
            h5Flag = 1;
        }
        delete group;
        delete evolution;
        delete myeigen;
    }

    delete random;
    delete setup;
    delete param;

    if (h5Flag == 1 && rank == 0) {
        int status = 0;
        stringstream collect_command;
        collect_command << "python3 utilities/combine_events_into_hdf5.py ."
                        << " --output_filename RESULTS"
                        << " --combine_hdf5_files_only";
        status = system(collect_command.str().c_str());
        messager << "finished system call to python script with status: "
                 << status;
        messager.flush("info");
    }

    MPI_Finalize();
    return 1;
}


void display_logo() {
    cout << endl;
    cout << "-----------------------------------------------------------------------------" << endl;
    cout << "| Classical Yang-Mills evolution with IP-Glasma initial configurations v1.4 |" << endl;
    cout << "-----------------------------------------------------------------------------" << endl;
    cout << "| References:                                                               |" << endl;
    cout << "| B. Schenke, P. Tribedy, R. Venugopalan                                    |" << endl;
    cout << "| Phys. Rev. Lett. 108, 252301 (2012) and Phys. Rev. C86, 034908 (2012)     |" << endl;
    cout << "-----------------------------------------------------------------------------" << endl;

    cout << "This version uses Qs as obtained from IP-Sat using the sum over proton T_p(b)" << endl;
    cout << "This is a simple MPI version that runs many events in one job. No communication." << endl;

    cout << "Run using large lattices to improve convergence of the root finder in initial condition. "
         << "Recommended: 600x600 using L=30fm" << endl;
    cout << endl;
}

int readInput(Setup *setup, Parameters *param, int argc, char *argv[], int rank)
{
  // the first given argument is taken to be the input file name
  // if none is given, that file name is "input"
  //cout << "Opening input file ... " << endl;
  string file_name;
  if (argc>1)
    {
      file_name = argv[1];
      if(rank==0)
	cout << "Using file name \"" << file_name << "\"." << endl;
    }
  else
    {
      file_name = "input";
      if(rank==0)
	cout << "No input file name given. Using default \"" << file_name << "\"." << endl;
    }
  
 
  // read and set all the parameters in the "param" object of class "Parameters"
  if(rank==0)
    cout << "Reading parameters from file ... ";
  param->setNucleusQsTableFileName(setup->StringFind(file_name,"NucleusQsTableFileName"));
  param->setNucleonPositionsFromFile(setup->IFind(file_name,"nucleonPositionsFromFile"));
  param->setTarget(setup->StringFind(file_name,"Target"));
  param->setProjectile(setup->StringFind(file_name,"Projectile"));
  param->setMode(setup->IFind(file_name,"mode"));
  param->setRunningCoupling(setup->IFind(file_name,"runningCoupling"));
  param->setL(setup->DFind(file_name,"L"));
  param->setLOutput(setup->DFind(file_name,"LOutput"));
  param->setBG(setup->DFind(file_name,"BG"));
  param->setBGq(setup->DFind(file_name,"BGq"));
  param->setMuZero(setup->DFind(file_name,"muZero"));
  param->setc(setup->DFind(file_name,"c"));
  param->setSize(setup->IFind(file_name,"size"));
  param->setSizeOutput(setup->IFind(file_name,"sizeOutput"));
  param->setEtaSizeOutput(setup->IFind(file_name,"etaSizeOutput"));
  param->setDetaOutput(setup->DFind(file_name,"detaOutput"));
  param->setUseFluctuatingx(setup->IFind(file_name,"useFluctuatingx"));
  param->setNc(setup->IFind(file_name,"Nc"));
  param->setInverseQsForMaxTime(setup->IFind(file_name,"inverseQsForMaxTime"));
  param->setSeed(setup->ULLIFind(file_name,"seed"));
  param->setUseSeedList(setup->IFind(file_name,"useSeedList"));
  param->setNy(setup->IFind(file_name,"Ny"));
  param->setRoots(setup->DFind(file_name,"roots"));
  param->setNu(setup->DFind(file_name,"tDistNu"));
  param->setUseFatTails(setup->IFind(file_name,"useFatTails"));
  param->setg(setup->DFind(file_name,"g"));
  param->setm(setup->DFind(file_name,"m"));
  param->setJacobianm(setup->DFind(file_name,"Jacobianm"));
  param->setSigmaNN(setup->DFind(file_name,"SigmaNN"));
  param->setRmax(setup->DFind(file_name,"rmax"));
  param->setUVdamp(setup->DFind(file_name,"UVdamp"));
  param->setbmin(setup->DFind(file_name,"bmin"));
  param->setbmax(setup->DFind(file_name,"bmax"));
  param->setQsmuRatio(setup->DFind(file_name,"QsmuRatio"));
  param->setUsePseudoRapidity(setup->DFind(file_name,"usePseudoRapidity"));
  param->setRapidity(setup->DFind(file_name,"Rapidity"));
  param->setUseNucleus(setup->IFind(file_name,"useNucleus"));
  param->setUseGaussian(setup->IFind(file_name,"useGaussian"));
  param->setlightNucleusOption(setup->IFind(file_name,"lightNucleusOption"));
  param->setg2mu(setup->DFind(file_name,"g2mu"));
  param->setMaxtime(setup->DFind(file_name,"maxtime"));
  param->setdtau(setup->DFind(file_name,"dtau"));
  // param->setxExponent(setup->DFind(file_name,"xExponent")); //  is now obsolete
  param->setRunWithQs(setup->IFind(file_name,"runWith0Min1Avg2MaxQs"));
  param->setRunWithkt(setup->IFind(file_name,"runWithkt"));
  param->setRunWithLocalQs(setup->IFind(file_name,"runWithLocalQs"));
  param->setRunWithThisFactorTimesQs(setup->DFind(file_name,"runWithThisFactorTimesQs"));
  param->setxFromThisFactorTimesQs(setup->DFind(file_name,"xFromThisFactorTimesQs"));
  param->setLinearb(setup->IFind(file_name,"samplebFromLinearDistribution"));
  param->setWriteOutputs(setup->IFind(file_name,"writeOutputs"));
  param->setWriteOutputsToHDF5(setup->IFind(file_name,"writeOutputsToHDF5"));
  param->setWriteEvolution(setup->IFind(file_name,"writeEvolution"));
  param->setWriteInitialWilsonLines(setup->IFind(file_name, "writeInitialWilsonLines"));
  param->setAverageOverNuclei(setup->IFind(file_name,"averageOverThisManyNuclei"));
  param->setUseTimeForSeed(setup->IFind(file_name,"useTimeForSeed"));
  param->setUseFixedNpart(setup->IFind(file_name,"useFixedNpart"));
  param->setSmearQs(setup->IFind(file_name,"smearQs"));
  param->setSmearingWidth(setup->DFind(file_name,"smearingWidth"));
  param->setGaussianWounding(setup->IFind(file_name,"gaussianWounding"));
  param->setReadMultFromFile(setup->IFind(file_name,"readMultFromFile"));
  param->setProtonAnisotropy(setup->DFind(file_name,"protonAnisotropy"));
  param->setUseConstituentQuarkProton(setup->DFind(file_name,"useConstituentQuarkProton"));
  param->setUseSmoothNucleus(setup->IFind(file_name,"useSmoothNucleus"));
  param->setShiftConstituentQuarkProtonOrigin(setup->DFind(file_name,"shiftConstituentQuarkProtonOrigin"));
  if(rank==0)
    cout << "done." << endl;

// write the used parameters into file "usedParameters.dat" as a double check for later
  time_t rawtime = time(0);
  stringstream strup_name;
  strup_name << "usedParameters" << param->getEventId() << ".dat";
  string up_name;
  up_name = strup_name.str();
  
  fstream fout1(up_name.c_str(),ios::out); 
  char * timestring = ctime(&rawtime);
  fout1 << "File created on " << timestring << endl;
  fout1 << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << endl;
  fout1 << "Used parameters by IP-Glasma v1.3" << endl;
  fout1 << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ " << endl;
  fout1 << " " << endl;
  fout1 << " Output by readInput in main.cpp: " << endl;
  fout1 << " " << endl;
  fout1 << "Program run in mode " << param->getMode() << endl;
  fout1 << "Nc " << param->getNc() << endl;
  fout1 << "size " << param->getSize() << endl;
  fout1 << "lattice spacing a " << param->getL()/static_cast<double>(param->getSize()) << " fm " << endl;
  fout1 << "Ny " << param->getNy() << endl;
  fout1 << "Projectile " << param->getProjectile() << endl;
  fout1 << "Target " << param->getTarget() << endl;
  if (param->getUseConstituentQuarkProton()>0)
  {
    fout1 << "Nucleons consists of " << param->getUseConstituentQuarkProton() << " constituent quarks" << endl;
    if (param->getShiftConstituentQuarkProtonOrigin())
      fout1 << "... constituent quark center of mass moved to origin" << endl;
  }
  fout1 << "Smooth nucleus " << param->getUseSmoothNucleus() << endl;
  fout1 << "Gaussian wounding " << param->getGaussianWounding() << endl;
  fout1 << "Using fluctuating x=Qs/root(s) " << param->getUseFluctuatingx() << endl;
  if( param->getRunWithkt()==0)
    fout1 << "Using local Qs to run " << param->getRunWithLocalQs() << endl;
  else
    fout1 << "running alpha_s with k_T" << endl;
  fout1 << "QsmuRatio " << param->getQsmuRatio() << endl;
  fout1 << "smeared mu " << param->getSmearQs() << endl;
  fout1 << "m " << param->getm() << endl;
  fout1 << "rmax " << param->getRmax() << endl;
  fout1 << "UVdamp " << param->getUVdamp() << endl;
  if (param->getSmearQs()==1)
    {
      fout1 << "smearing width " << param->getSmearingWidth() << endl;
    }
  fout1 << "Using fat tailed distribution " << param->getUseFatTails() << endl;
  fout1.close();

  return 0;
}
