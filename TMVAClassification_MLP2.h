#ifndef TMVAClassification_MLP2_H
#define TMVAClassification_MLP2_H
// Class: ReadMLP2
// Automatically generated by MethodBase::MakeClass
//

/* configuration options =====================================================

#GEN -*-*-*-*-*-*-*-*-*-*-*- general info -*-*-*-*-*-*-*-*-*-*-*-

Method         : MLP::MLP2
TMVA Release   : 4.2.1         [262657]
ROOT Release   : 6.16/00       [397312]
Creator        : marcel
Date           : Tue May 28 09:00:11 2019
Host           : Linux SFT-ubuntu-1804-3 4.15.0-38-generic #41-Ubuntu SMP Wed Oct 10 10:59:38 UTC 2018 x86_64 x86_64 x86_64 GNU/Linux
Dir            : /home/marcel/workspace/tracking/macro
Training events: 600000
Analysis type  : [Classification]


#OPT -*-*-*-*-*-*-*-*-*-*-*-*- options -*-*-*-*-*-*-*-*-*-*-*-*-

# Set by User:
NCycles: "100" [Number of training cycles]
HiddenLayers: "15,5" [Specification of hidden layer architecture]
NeuronType: "tanh" [Neuron activation function type]
V: "False" [Verbose output (short form of "VerbosityLevel" below - overrides the latter one)]
VarTransform: "N" [List of variable transformations performed before training, e.g., "D_Background,P_Signal,G,N_AllClasses" for: "Decorrelation, PCA-transformation, Gaussianisation, Normalisation, each for the given class of events ('AllClasses' denotes all events of all classes, if no class indication is given, 'All' is assumed)"]
H: "False" [Print method-specific help message]
TestRate: "5" [Test for overtraining performed at each #th epochs]
UseRegulator: "False" [Use regulator to avoid over-training]
# Default:
RandomSeed: "1" [Random seed for initial synapse weights (0 means unique seed for each run; default value '1')]
EstimatorType: "CE" [MSE (Mean Square Estimator) for Gaussian Likelihood or CE(Cross-Entropy) for Bernoulli Likelihood]
NeuronInputType: "sum" [Neuron input function type]
VerbosityLevel: "Default" [Verbosity level]
CreateMVAPdfs: "False" [Create PDFs for classifier outputs (signal and background)]
IgnoreNegWeightsInTraining: "False" [Events with negative weights are ignored in the training (but are included for testing and performance evaluation)]
TrainingMethod: "BP" [Train with Back-Propagation (BP), BFGS Algorithm (BFGS), or Genetic Algorithm (GA - slower and worse)]
LearningRate: "2.000000e-02" [ANN learning rate parameter]
DecayRate: "1.000000e-02" [Decay rate for learning parameter]
EpochMonitoring: "False" [Provide epoch-wise monitoring plots according to TestRate (caution: causes big ROOT output file!)]
Sampling: "1.000000e+00" [Only 'Sampling' (randomly selected) events are trained each epoch]
SamplingEpoch: "1.000000e+00" [Sampling is used for the first 'SamplingEpoch' epochs, afterwards, all events are taken for training]
SamplingImportance: "1.000000e+00" [ The sampling weights of events in epochs which successful (worse estimator than before) are multiplied with SamplingImportance, else they are divided.]
SamplingTraining: "True" [The training sample is sampled]
SamplingTesting: "False" [The testing sample is sampled]
ResetStep: "50" [How often BFGS should reset history]
Tau: "3.000000e+00" [LineSearch "size step"]
BPMode: "sequential" [Back-propagation learning mode: sequential or batch]
BatchSize: "-1" [Batch size: number of events/batch, only set if in Batch Mode, -1 for BatchSize=number_of_events]
ConvergenceImprove: "1.000000e-30" [Minimum improvement which counts as improvement (<0 means automatic convergence check is turned off)]
ConvergenceTests: "-1" [Number of steps (without improvement) required for convergence (<0 means automatic convergence check is turned off)]
UpdateLimit: "10000" [Maximum times of regulator update]
CalculateErrors: "False" [Calculates inverse Hessian matrix at the end of the training to be able to calculate the uncertainties of an MVA value]
WeightRange: "1.000000e+00" [Take the events for the estimator calculations from small deviations from the desired value to large deviations only over the weight range]
##


#VAR -*-*-*-*-*-*-*-*-*-*-*-* variables *-*-*-*-*-*-*-*-*-*-*-*-

NVar 9
rz1                           rz1                           rz1                           rz1                                                             'F'    [31.4393234253,825.41192627]
phi1                          phi1                          phi1                          phi1                                                            'F'    [-3.14158201218,3.14154338837]
z1                            z1                            z1                            z1                                                              'F'    [-1083.39990234,1083.39990234]
rz2                           rz2                           rz2                           rz2                                                             'F'    [30.2561168671,1022.02819824]
phi2                          phi2                          phi2                          phi2                                                            'F'    [-3.14158201218,3.1415476799]
z2                            z2                            z2                            z2                                                              'F'    [-1225.5,1225.5]
f0                            f0                            f0                            f0                                                              'F'    [0.00042819121154,1.57076609135]
f1                            f1                            f1                            f1                                                              'F'    [0.00021225126693,1.5254689455]
log(score)                    log_score_                    log(score)                    log(score)                                                      'F'    [-9.67703723907,6.32791090012]
NSpec 0


============================================================================ */

#include <array>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>

#ifndef IClassifierReader__def
#define IClassifierReader__def

class IClassifierReader {

 public:

   // constructor
   IClassifierReader() : fStatusIsClean( true ) {}
   virtual ~IClassifierReader() {}

   // return classifier response
   virtual double GetMvaValue( const std::vector<double>& inputValues ) const = 0;

   // returns classifier status
   bool IsStatusClean() const { return fStatusIsClean; }

 protected:

   bool fStatusIsClean;
};

#endif

class ReadMLP2 : public IClassifierReader {

 public:

   // constructor
   ReadMLP2( std::vector<std::string>& theInputVars )
      : IClassifierReader(),
        fClassName( "ReadMLP2" ),
        fNvars( 9 )
   {
      // the training input variables
      const char* inputVars[] = { "rz1", "phi1", "z1", "rz2", "phi2", "z2", "f0", "f1", "log(score)" };

      // sanity checks
      if (theInputVars.size() <= 0) {
         std::cout << "Problem in class \"" << fClassName << "\": empty input vector" << std::endl;
         fStatusIsClean = false;
      }

      if (theInputVars.size() != fNvars) {
         std::cout << "Problem in class \"" << fClassName << "\": mismatch in number of input values: "
                   << theInputVars.size() << " != " << fNvars << std::endl;
         fStatusIsClean = false;
      }

      // validate input variables
      for (size_t ivar = 0; ivar < theInputVars.size(); ivar++) {
         if (theInputVars[ivar] != inputVars[ivar]) {
            std::cout << "Problem in class \"" << fClassName << "\": mismatch in input variable names" << std::endl
                      << " for variable [" << ivar << "]: " << theInputVars[ivar].c_str() << " != " << inputVars[ivar] << std::endl;
            fStatusIsClean = false;
         }
      }

      // initialize min and max vectors (for normalisation)
      fVmin[0] = -1;
      fVmax[0] = 1;
      fVmin[1] = -1;
      fVmax[1] = 1;
      fVmin[2] = -1;
      fVmax[2] = 1;
      fVmin[3] = -1;
      fVmax[3] = 1;
      fVmin[4] = -1;
      fVmax[4] = 1;
      fVmin[5] = -1;
      fVmax[5] = 1;
      fVmin[6] = -1;
      fVmax[6] = 0.99999988079071;
      fVmin[7] = -1;
      fVmax[7] = 1;
      fVmin[8] = -1;
      fVmax[8] = 1;

      // initialize input variable types
      fType[0] = 'F';
      fType[1] = 'F';
      fType[2] = 'F';
      fType[3] = 'F';
      fType[4] = 'F';
      fType[5] = 'F';
      fType[6] = 'F';
      fType[7] = 'F';
      fType[8] = 'F';

      // initialize constants
      Initialize();

      // initialize transformation
      InitTransform();
   }

   // destructor
   virtual ~ReadMLP2() {
      Clear(); // method-specific
   }

   // the classifier response
   // "inputValues" is a vector of input values in the same order as the
   // variables given to the constructor
   double GetMvaValue( const std::vector<double>& inputValues ) const override;

 private:

   // method-specific destructor
   void Clear();

   // input variable transformation

   double fOff_1[3][9];
   double fScal_1[3][9];
   void InitTransform_1();
   void Transform_1( std::vector<double> & iv, int sigOrBgd ) const;
   void InitTransform();
   void Transform( std::vector<double> & iv, int sigOrBgd ) const;

   // common member variables
   const char* fClassName;

   const size_t fNvars;
   size_t GetNvar()           const { return fNvars; }
   char   GetType( int ivar ) const { return fType[ivar]; }

   // normalisation of input variables
   double fVmin[9];
   double fVmax[9];
   double NormVariable( double x, double xmin, double xmax ) const {
      // normalise to output range: [-1, 1]
      return 2*(x - xmin)/(xmax - xmin) - 1.0;
   }

   // type of input variable: 'F' or 'I'
   char   fType[9];

   // initialize internal variables
   void Initialize();
   double GetMvaValue__( const std::vector<double>& inputValues ) const;

   // private members (method specific)

   double ActivationFnc(double x) const;
   double OutputActivationFnc(double x) const;

   double fWeightMatrix0to1[16][10];   // weight matrix from layer 0 to 1
   double fWeightMatrix1to2[6][16];   // weight matrix from layer 1 to 2
   double fWeightMatrix2to3[1][6];   // weight matrix from layer 2 to 3

};

inline void ReadMLP2::Initialize()
{
   // build network structure
   // weight matrix from layer 0 to 1
   fWeightMatrix0to1[0][0] = -2.46450888840249;
   fWeightMatrix0to1[1][0] = 4.46056332607331;
   fWeightMatrix0to1[2][0] = 1.75690569456623;
   fWeightMatrix0to1[3][0] = 1.23579039932217;
   fWeightMatrix0to1[4][0] = 2.7368144134458;
   fWeightMatrix0to1[5][0] = 3.5797503550661;
   fWeightMatrix0to1[6][0] = 4.4725926534807;
   fWeightMatrix0to1[7][0] = 4.53977135706321;
   fWeightMatrix0to1[8][0] = -18.2517355431308;
   fWeightMatrix0to1[9][0] = -6.57919957704628;
   fWeightMatrix0to1[10][0] = -7.96908676026644;
   fWeightMatrix0to1[11][0] = -12.1005608887741;
   fWeightMatrix0to1[12][0] = 0.656518745839077;
   fWeightMatrix0to1[13][0] = 3.16502974014903;
   fWeightMatrix0to1[14][0] = -1.36498060820931;
   fWeightMatrix0to1[0][1] = -0.189352770011459;
   fWeightMatrix0to1[1][1] = -0.427373861862679;
   fWeightMatrix0to1[2][1] = -0.311269136724133;
   fWeightMatrix0to1[3][1] = 0.084546473868888;
   fWeightMatrix0to1[4][1] = -24.5656253937506;
   fWeightMatrix0to1[5][1] = -7.91073419688389;
   fWeightMatrix0to1[6][1] = -0.345439814548831;
   fWeightMatrix0to1[7][1] = 8.3926667787424;
   fWeightMatrix0to1[8][1] = 0.412506563398763;
   fWeightMatrix0to1[9][1] = 0.301354514354946;
   fWeightMatrix0to1[10][1] = -0.57473136916902;
   fWeightMatrix0to1[11][1] = 5.76668948921788;
   fWeightMatrix0to1[12][1] = -2.82700092784097;
   fWeightMatrix0to1[13][1] = -40.3716862612805;
   fWeightMatrix0to1[14][1] = 0.290239405008699;
   fWeightMatrix0to1[0][2] = 0.69066570416007;
   fWeightMatrix0to1[1][2] = -1.07583741160894;
   fWeightMatrix0to1[2][2] = 29.2849234211058;
   fWeightMatrix0to1[3][2] = -0.504245866414008;
   fWeightMatrix0to1[4][2] = -0.264227547866406;
   fWeightMatrix0to1[5][2] = 0.132007503708651;
   fWeightMatrix0to1[6][2] = 0.262317840931083;
   fWeightMatrix0to1[7][2] = 0.33380822481891;
   fWeightMatrix0to1[8][2] = 1.71305195544602;
   fWeightMatrix0to1[9][2] = 0.462612922483944;
   fWeightMatrix0to1[10][2] = 0.123823410166386;
   fWeightMatrix0to1[11][2] = 1.72043922133605;
   fWeightMatrix0to1[12][2] = 0.154860901139588;
   fWeightMatrix0to1[13][2] = 0.126416906402396;
   fWeightMatrix0to1[14][2] = 29.776972404896;
   fWeightMatrix0to1[0][3] = 0.681503918447103;
   fWeightMatrix0to1[1][3] = 2.58621514159623;
   fWeightMatrix0to1[2][3] = -1.97902100642028;
   fWeightMatrix0to1[3][3] = -1.05117405992789;
   fWeightMatrix0to1[4][3] = -3.70138623814064;
   fWeightMatrix0to1[5][3] = -15.71340862034;
   fWeightMatrix0to1[6][3] = 10.5271788068111;
   fWeightMatrix0to1[7][3] = -7.69208003993212;
   fWeightMatrix0to1[8][3] = 11.4534868874524;
   fWeightMatrix0to1[9][3] = -0.880533461200895;
   fWeightMatrix0to1[10][3] = -1.87265139674489;
   fWeightMatrix0to1[11][3] = -6.62816725869344;
   fWeightMatrix0to1[12][3] = -12.6214018773342;
   fWeightMatrix0to1[13][3] = -4.1315294562498;
   fWeightMatrix0to1[14][3] = 1.58783812999772;
   fWeightMatrix0to1[0][4] = 0.246309098858396;
   fWeightMatrix0to1[1][4] = 0.429507168107006;
   fWeightMatrix0to1[2][4] = 0.240379356547397;
   fWeightMatrix0to1[3][4] = -0.188143546738731;
   fWeightMatrix0to1[4][4] = 24.5700254595731;
   fWeightMatrix0to1[5][4] = 8.00544355604119;
   fWeightMatrix0to1[6][4] = 0.383682872890099;
   fWeightMatrix0to1[7][4] = -8.28429552993255;
   fWeightMatrix0to1[8][4] = -0.318728143533987;
   fWeightMatrix0to1[9][4] = -0.117047588906695;
   fWeightMatrix0to1[10][4] = 0.601298450291084;
   fWeightMatrix0to1[11][4] = -5.72841595573763;
   fWeightMatrix0to1[12][4] = 2.7542520094586;
   fWeightMatrix0to1[13][4] = 40.3067967450457;
   fWeightMatrix0to1[14][4] = -0.267891195722691;
   fWeightMatrix0to1[0][5] = -0.432222409389554;
   fWeightMatrix0to1[1][5] = 0.874613853941551;
   fWeightMatrix0to1[2][5] = -41.2346876721908;
   fWeightMatrix0to1[3][5] = 0.359820128993488;
   fWeightMatrix0to1[4][5] = 0.106438299712134;
   fWeightMatrix0to1[5][5] = -0.0635068707223302;
   fWeightMatrix0to1[6][5] = -0.138472486869347;
   fWeightMatrix0to1[7][5] = -0.111850515255564;
   fWeightMatrix0to1[8][5] = -1.26187983106656;
   fWeightMatrix0to1[9][5] = -0.216768693618899;
   fWeightMatrix0to1[10][5] = -0.128908378812458;
   fWeightMatrix0to1[11][5] = -1.43862309277741;
   fWeightMatrix0to1[12][5] = -0.200241918080557;
   fWeightMatrix0to1[13][5] = -0.0594259138448734;
   fWeightMatrix0to1[14][5] = -41.7927869108442;
   fWeightMatrix0to1[0][6] = 0.417843846859074;
   fWeightMatrix0to1[1][6] = 7.76704786986549;
   fWeightMatrix0to1[2][6] = 0.179167201868329;
   fWeightMatrix0to1[3][6] = -7.99026388823523;
   fWeightMatrix0to1[4][6] = -2.15556338626331;
   fWeightMatrix0to1[5][6] = 18.6334613843875;
   fWeightMatrix0to1[6][6] = -11.1279826083347;
   fWeightMatrix0to1[7][6] = 1.47888947402061;
   fWeightMatrix0to1[8][6] = -0.813476933132939;
   fWeightMatrix0to1[9][6] = -0.926852904170096;
   fWeightMatrix0to1[10][6] = 0.0494794146118686;
   fWeightMatrix0to1[11][6] = -0.0642282965028196;
   fWeightMatrix0to1[12][6] = -6.26467437344079;
   fWeightMatrix0to1[13][6] = -0.0790933672728478;
   fWeightMatrix0to1[14][6] = -0.237364074857004;
   fWeightMatrix0to1[0][7] = -0.240937247553477;
   fWeightMatrix0to1[1][7] = -7.7277208829503;
   fWeightMatrix0to1[2][7] = -2.17599613806627;
   fWeightMatrix0to1[3][7] = -2.03121137517953;
   fWeightMatrix0to1[4][7] = -0.232258394326608;
   fWeightMatrix0to1[5][7] = -6.04210263143453;
   fWeightMatrix0to1[6][7] = -14.9038381012915;
   fWeightMatrix0to1[7][7] = -6.27629451762082;
   fWeightMatrix0to1[8][7] = 0.293601313395047;
   fWeightMatrix0to1[9][7] = 0.0176857424824234;
   fWeightMatrix0to1[10][7] = -0.000600271439361879;
   fWeightMatrix0to1[11][7] = 0.265490056004092;
   fWeightMatrix0to1[12][7] = -0.692184643484334;
   fWeightMatrix0to1[13][7] = 0.136986741246234;
   fWeightMatrix0to1[14][7] = 2.36251402358914;
   fWeightMatrix0to1[0][8] = -15.2475468948478;
   fWeightMatrix0to1[1][8] = 0.298469901523208;
   fWeightMatrix0to1[2][8] = -0.215312007774286;
   fWeightMatrix0to1[3][8] = -0.246142238219623;
   fWeightMatrix0to1[4][8] = -0.283111435142677;
   fWeightMatrix0to1[5][8] = -0.430395908277012;
   fWeightMatrix0to1[6][8] = -0.0965050203009593;
   fWeightMatrix0to1[7][8] = -0.178701056178746;
   fWeightMatrix0to1[8][8] = -18.8700177460753;
   fWeightMatrix0to1[9][8] = 3.09216089004828;
   fWeightMatrix0to1[10][8] = 0.583002961182269;
   fWeightMatrix0to1[11][8] = 0.25669409597075;
   fWeightMatrix0to1[12][8] = 1.43977892199579;
   fWeightMatrix0to1[13][8] = 0.166082342933176;
   fWeightMatrix0to1[14][8] = 0.140221501347881;
   fWeightMatrix0to1[0][9] = 11.51161962579;
   fWeightMatrix0to1[1][9] = 8.51890132062826;
   fWeightMatrix0to1[2][9] = -2.41723994807841;
   fWeightMatrix0to1[3][9] = -2.42556397264282;
   fWeightMatrix0to1[4][9] = -1.09495125652265;
   fWeightMatrix0to1[5][9] = 0.734711419286916;
   fWeightMatrix0to1[6][9] = -8.96274383080528;
   fWeightMatrix0to1[7][9] = -6.15798201135136;
   fWeightMatrix0to1[8][9] = 9.04874308372792;
   fWeightMatrix0to1[9][9] = -5.18613301010506;
   fWeightMatrix0to1[10][9] = 1.61771503700657;
   fWeightMatrix0to1[11][9] = -8.47910352779529;
   fWeightMatrix0to1[12][9] = -11.8681592333745;
   fWeightMatrix0to1[13][9] = -2.36418664997268;
   fWeightMatrix0to1[14][9] = 2.50937160475803;
   // weight matrix from layer 1 to 2
   fWeightMatrix1to2[0][0] = 12.4419831826113;
   fWeightMatrix1to2[1][0] = -0.15506999130701;
   fWeightMatrix1to2[2][0] = 2.93291954931374;
   fWeightMatrix1to2[3][0] = 1.43091158048489;
   fWeightMatrix1to2[4][0] = -0.27833114435678;
   fWeightMatrix1to2[0][1] = 3.0552345806179;
   fWeightMatrix1to2[1][1] = 1.80373292104509;
   fWeightMatrix1to2[2][1] = -0.259952285799147;
   fWeightMatrix1to2[3][1] = -2.42037764584489;
   fWeightMatrix1to2[4][1] = -0.990996395585558;
   fWeightMatrix1to2[0][2] = 0.0770916275117291;
   fWeightMatrix1to2[1][2] = -1.72902362289839;
   fWeightMatrix1to2[2][2] = 2.20764566620565;
   fWeightMatrix1to2[3][2] = 0.714261502842754;
   fWeightMatrix1to2[4][2] = 0.98686136979433;
   fWeightMatrix1to2[0][3] = 0.239781036183321;
   fWeightMatrix1to2[1][3] = -0.202615356104618;
   fWeightMatrix1to2[2][3] = 0.773229689585729;
   fWeightMatrix1to2[3][3] = -6.49142791980136;
   fWeightMatrix1to2[4][3] = 2.57863226349776;
   fWeightMatrix1to2[0][4] = 1.62436467034317;
   fWeightMatrix1to2[1][4] = 1.06854976285753;
   fWeightMatrix1to2[2][4] = 0.830080091629629;
   fWeightMatrix1to2[3][4] = -1.68859973792059;
   fWeightMatrix1to2[4][4] = 1.68709929567287;
   fWeightMatrix1to2[0][5] = 0.818858081215269;
   fWeightMatrix1to2[1][5] = 1.03824657529972;
   fWeightMatrix1to2[2][5] = -0.125769619120637;
   fWeightMatrix1to2[3][5] = 6.5606755643594;
   fWeightMatrix1to2[4][5] = -0.444749060632209;
   fWeightMatrix1to2[0][6] = 0.029447652660134;
   fWeightMatrix1to2[1][6] = -0.0596246754698843;
   fWeightMatrix1to2[2][6] = 0.117607776272636;
   fWeightMatrix1to2[3][6] = -1.79451144000672;
   fWeightMatrix1to2[4][6] = 0.249945566960945;
   fWeightMatrix1to2[0][7] = -0.321965753908639;
   fWeightMatrix1to2[1][7] = -0.419729372289906;
   fWeightMatrix1to2[2][7] = 0.890878868778403;
   fWeightMatrix1to2[3][7] = 2.38438313118907;
   fWeightMatrix1to2[4][7] = -0.267451939509871;
   fWeightMatrix1to2[0][8] = 0.710821305589339;
   fWeightMatrix1to2[1][8] = 8.88800011298585;
   fWeightMatrix1to2[2][8] = 0.281705167198763;
   fWeightMatrix1to2[3][8] = -0.569597419518194;
   fWeightMatrix1to2[4][8] = 8.32390127976361;
   fWeightMatrix1to2[0][9] = -0.931743077243935;
   fWeightMatrix1to2[1][9] = -4.21461632585888;
   fWeightMatrix1to2[2][9] = -0.32768617576429;
   fWeightMatrix1to2[3][9] = 7.03470688176895;
   fWeightMatrix1to2[4][9] = -0.840671241088571;
   fWeightMatrix1to2[0][10] = -1.65580125328061;
   fWeightMatrix1to2[1][10] = -2.1420347027846;
   fWeightMatrix1to2[2][10] = -0.438423965253024;
   fWeightMatrix1to2[3][10] = -8.04361714545202;
   fWeightMatrix1to2[4][10] = -0.30993237407588;
   fWeightMatrix1to2[0][11] = -2.32372015399356;
   fWeightMatrix1to2[1][11] = -2.25537843615063;
   fWeightMatrix1to2[2][11] = 0.222789923199057;
   fWeightMatrix1to2[3][11] = 7.5779516071566;
   fWeightMatrix1to2[4][11] = 1.88442513966125;
   fWeightMatrix1to2[0][12] = 1.39872972755585;
   fWeightMatrix1to2[1][12] = -3.24087167380121;
   fWeightMatrix1to2[2][12] = -0.270525421869627;
   fWeightMatrix1to2[3][12] = 0.291792597943834;
   fWeightMatrix1to2[4][12] = 0.349052254225546;
   fWeightMatrix1to2[0][13] = -2.94762324516753;
   fWeightMatrix1to2[1][13] = -1.18728298759647;
   fWeightMatrix1to2[2][13] = -0.760311869824445;
   fWeightMatrix1to2[3][13] = 3.1010465882265;
   fWeightMatrix1to2[4][13] = -6.84876329758236;
   fWeightMatrix1to2[0][14] = 0.024558218714042;
   fWeightMatrix1to2[1][14] = 1.60485585118672;
   fWeightMatrix1to2[2][14] = -2.17463370379436;
   fWeightMatrix1to2[3][14] = -0.784622178235392;
   fWeightMatrix1to2[4][14] = -0.981987646415146;
   fWeightMatrix1to2[0][15] = -16.61605310895;
   fWeightMatrix1to2[1][15] = 0.0397313243213716;
   fWeightMatrix1to2[2][15] = 0.39070442049665;
   fWeightMatrix1to2[3][15] = -3.15709364943875;
   fWeightMatrix1to2[4][15] = -15.7282878344682;
   // weight matrix from layer 2 to 3
   fWeightMatrix2to3[0][0] = 0.776599737814204;
   fWeightMatrix2to3[0][1] = 2.49653518158259;
   fWeightMatrix2to3[0][2] = 1.97873428012128;
   fWeightMatrix2to3[0][3] = -1.98983756902696;
   fWeightMatrix2to3[0][4] = 1.07076839329884;
   fWeightMatrix2to3[0][5] = -4.56517068604864;
}

inline double ReadMLP2::GetMvaValue__( const std::vector<double>& inputValues ) const
{
   if (inputValues.size() != (unsigned int)9) {
      std::cout << "Input vector needs to be of size " << 9 << std::endl;
      return 0;
   }

   std::array<double, 16> fWeights1 {{}};
   std::array<double, 6> fWeights2 {{}};
   std::array<double, 1> fWeights3 {{}};
   fWeights1.back() = 1.;
   fWeights2.back() = 1.;

   // layer 0 to 1
   for (int o=0; o<15; o++) {
      std::array<double, 10> buffer; // no need to initialise
      for (int i = 0; i<10 - 1; i++) {
         buffer[i] = fWeightMatrix0to1[o][i] * inputValues[i];
      } // loop over i
      buffer.back() = fWeightMatrix0to1[o][9];      for (int i=0; i<10; i++) {
         fWeights1[o] += buffer[i];
      } // loop over i
    } // loop over o
   for (int o=0; o<15; o++) {
      fWeights1[o] = ActivationFnc(fWeights1[o]);
   } // loop over o
   // layer 1 to 2
   for (int o=0; o<5; o++) {
      std::array<double, 16> buffer; // no need to initialise
      for (int i=0; i<16; i++) {
         buffer[i] = fWeightMatrix1to2[o][i] * fWeights1[i];
      } // loop over i
      for (int i=0; i<16; i++) {
         fWeights2[o] += buffer[i];
      } // loop over i
    } // loop over o
   for (int o=0; o<5; o++) {
      fWeights2[o] = ActivationFnc(fWeights2[o]);
   } // loop over o
   // layer 2 to 3
   for (int o=0; o<1; o++) {
      std::array<double, 6> buffer; // no need to initialise
      for (int i=0; i<6; i++) {
         buffer[i] = fWeightMatrix2to3[o][i] * fWeights2[i];
      } // loop over i
      for (int i=0; i<6; i++) {
         fWeights3[o] += buffer[i];
      } // loop over i
    } // loop over o
   for (int o=0; o<1; o++) {
      fWeights3[o] = OutputActivationFnc(fWeights3[o]);
   } // loop over o

   return fWeights3[0];
}

inline double ReadMLP2::ActivationFnc(double x) const {
   // fast hyperbolic tan approximation
   if (x > 4.97) return 1;
   if (x < -4.97) return -1;
   float x2 = x * x;
   float a = x * (135135.0f + x2 * (17325.0f + x2 * (378.0f + x2)));
   float b = 135135.0f + x2 * (62370.0f + x2 * (3150.0f + x2 * 28.0f));
   return a / b;
}
inline double ReadMLP2::OutputActivationFnc(double x) const {
   // sigmoid
   return 1.0/(1.0+exp(-x));
}

// Clean up
inline void ReadMLP2::Clear()
{
}
   inline double ReadMLP2::GetMvaValue( const std::vector<double>& inputValues ) const
   {
      // classifier response value
      double retval = 0;

      // classifier response, sanity check first
      if (!IsStatusClean()) {
         std::cout << "Problem in class \"" << fClassName << "\": cannot return classifier response"
                   << " because status is dirty" << std::endl;
         retval = 0;
      }
      else {
            std::vector<double> iV(inputValues);
            Transform( iV, -1 );
            retval = GetMvaValue__( iV );
      }

      return retval;
   }

//_______________________________________________________________________
inline void ReadMLP2::InitTransform_1()
{
   double fMin_1[3][9];
   double fMax_1[3][9];
   // Normalization transformation, initialisation
   fMin_1[0][0] = 31.4393234253;
   fMax_1[0][0] = 825.274047852;
   fScal_1[0][0] = 2.0/(fMax_1[0][0]-fMin_1[0][0]);
   fOff_1[0][0] = fMin_1[0][0]*fScal_1[0][0]+1.;
   fMin_1[1][0] = 31.4393234253;
   fMax_1[1][0] = 825.41192627;
   fScal_1[1][0] = 2.0/(fMax_1[1][0]-fMin_1[1][0]);
   fOff_1[1][0] = fMin_1[1][0]*fScal_1[1][0]+1.;
   fMin_1[2][0] = 31.4393234253;
   fMax_1[2][0] = 825.41192627;
   fScal_1[2][0] = 2.0/(fMax_1[2][0]-fMin_1[2][0]);
   fOff_1[2][0] = fMin_1[2][0]*fScal_1[2][0]+1.;
   fMin_1[0][1] = -3.14158201218;
   fMax_1[0][1] = 3.14154338837;
   fScal_1[0][1] = 2.0/(fMax_1[0][1]-fMin_1[0][1]);
   fOff_1[0][1] = fMin_1[0][1]*fScal_1[0][1]+1.;
   fMin_1[1][1] = -3.14158201218;
   fMax_1[1][1] = 3.14153790474;
   fScal_1[1][1] = 2.0/(fMax_1[1][1]-fMin_1[1][1]);
   fOff_1[1][1] = fMin_1[1][1]*fScal_1[1][1]+1.;
   fMin_1[2][1] = -3.14158201218;
   fMax_1[2][1] = 3.14154338837;
   fScal_1[2][1] = 2.0/(fMax_1[2][1]-fMin_1[2][1]);
   fOff_1[2][1] = fMin_1[2][1]*fScal_1[2][1]+1.;
   fMin_1[0][2] = -1083.39990234;
   fMax_1[0][2] = 1083.39990234;
   fScal_1[0][2] = 2.0/(fMax_1[0][2]-fMin_1[0][2]);
   fOff_1[0][2] = fMin_1[0][2]*fScal_1[0][2]+1.;
   fMin_1[1][2] = -1083.39990234;
   fMax_1[1][2] = 1083.39990234;
   fScal_1[1][2] = 2.0/(fMax_1[1][2]-fMin_1[1][2]);
   fOff_1[1][2] = fMin_1[1][2]*fScal_1[1][2]+1.;
   fMin_1[2][2] = -1083.39990234;
   fMax_1[2][2] = 1083.39990234;
   fScal_1[2][2] = 2.0/(fMax_1[2][2]-fMin_1[2][2]);
   fOff_1[2][2] = fMin_1[2][2]*fScal_1[2][2]+1.;
   fMin_1[0][3] = 38.47605896;
   fMax_1[0][3] = 1019.93017578;
   fScal_1[0][3] = 2.0/(fMax_1[0][3]-fMin_1[0][3]);
   fOff_1[0][3] = fMin_1[0][3]*fScal_1[0][3]+1.;
   fMin_1[1][3] = 30.2561168671;
   fMax_1[1][3] = 1022.02819824;
   fScal_1[1][3] = 2.0/(fMax_1[1][3]-fMin_1[1][3]);
   fOff_1[1][3] = fMin_1[1][3]*fScal_1[1][3]+1.;
   fMin_1[2][3] = 30.2561168671;
   fMax_1[2][3] = 1022.02819824;
   fScal_1[2][3] = 2.0/(fMax_1[2][3]-fMin_1[2][3]);
   fOff_1[2][3] = fMin_1[2][3]*fScal_1[2][3]+1.;
   fMin_1[0][4] = -3.1415784359;
   fMax_1[0][4] = 3.1415476799;
   fScal_1[0][4] = 2.0/(fMax_1[0][4]-fMin_1[0][4]);
   fOff_1[0][4] = fMin_1[0][4]*fScal_1[0][4]+1.;
   fMin_1[1][4] = -3.14158201218;
   fMax_1[1][4] = 3.14153790474;
   fScal_1[1][4] = 2.0/(fMax_1[1][4]-fMin_1[1][4]);
   fOff_1[1][4] = fMin_1[1][4]*fScal_1[1][4]+1.;
   fMin_1[2][4] = -3.14158201218;
   fMax_1[2][4] = 3.1415476799;
   fScal_1[2][4] = 2.0/(fMax_1[2][4]-fMin_1[2][4]);
   fOff_1[2][4] = fMin_1[2][4]*fScal_1[2][4]+1.;
   fMin_1[0][5] = -1225.5;
   fMax_1[0][5] = 1225.5;
   fScal_1[0][5] = 2.0/(fMax_1[0][5]-fMin_1[0][5]);
   fOff_1[0][5] = fMin_1[0][5]*fScal_1[0][5]+1.;
   fMin_1[1][5] = -1225.5;
   fMax_1[1][5] = 1225.5;
   fScal_1[1][5] = 2.0/(fMax_1[1][5]-fMin_1[1][5]);
   fOff_1[1][5] = fMin_1[1][5]*fScal_1[1][5]+1.;
   fMin_1[2][5] = -1225.5;
   fMax_1[2][5] = 1225.5;
   fScal_1[2][5] = 2.0/(fMax_1[2][5]-fMin_1[2][5]);
   fOff_1[2][5] = fMin_1[2][5]*fScal_1[2][5]+1.;
   fMin_1[0][6] = 0.00042819121154;
   fMax_1[0][6] = 1.51090121269;
   fScal_1[0][6] = 2.0/(fMax_1[0][6]-fMin_1[0][6]);
   fOff_1[0][6] = fMin_1[0][6]*fScal_1[0][6]+1.;
   fMin_1[1][6] = 0.000944022962358;
   fMax_1[1][6] = 1.57076609135;
   fScal_1[1][6] = 2.0/(fMax_1[1][6]-fMin_1[1][6]);
   fOff_1[1][6] = fMin_1[1][6]*fScal_1[1][6]+1.;
   fMin_1[2][6] = 0.00042819121154;
   fMax_1[2][6] = 1.57076609135;
   fScal_1[2][6] = 2.0/(fMax_1[2][6]-fMin_1[2][6]);
   fOff_1[2][6] = fMin_1[2][6]*fScal_1[2][6]+1.;
   fMin_1[0][7] = 0.00021225126693;
   fMax_1[0][7] = 1.37590432167;
   fScal_1[0][7] = 2.0/(fMax_1[0][7]-fMin_1[0][7]);
   fOff_1[0][7] = fMin_1[0][7]*fScal_1[0][7]+1.;
   fMin_1[1][7] = 0.000546852941625;
   fMax_1[1][7] = 1.5254689455;
   fScal_1[1][7] = 2.0/(fMax_1[1][7]-fMin_1[1][7]);
   fOff_1[1][7] = fMin_1[1][7]*fScal_1[1][7]+1.;
   fMin_1[2][7] = 0.00021225126693;
   fMax_1[2][7] = 1.5254689455;
   fScal_1[2][7] = 2.0/(fMax_1[2][7]-fMin_1[2][7]);
   fOff_1[2][7] = fMin_1[2][7]*fScal_1[2][7]+1.;
   fMin_1[0][8] = -9.67703723907;
   fMax_1[0][8] = 6.26013422012;
   fScal_1[0][8] = 2.0/(fMax_1[0][8]-fMin_1[0][8]);
   fOff_1[0][8] = fMin_1[0][8]*fScal_1[0][8]+1.;
   fMin_1[1][8] = -8.97263240814;
   fMax_1[1][8] = 6.32791090012;
   fScal_1[1][8] = 2.0/(fMax_1[1][8]-fMin_1[1][8]);
   fOff_1[1][8] = fMin_1[1][8]*fScal_1[1][8]+1.;
   fMin_1[2][8] = -9.67703723907;
   fMax_1[2][8] = 6.32791090012;
   fScal_1[2][8] = 2.0/(fMax_1[2][8]-fMin_1[2][8]);
   fOff_1[2][8] = fMin_1[2][8]*fScal_1[2][8]+1.;
}

//_______________________________________________________________________
inline void ReadMLP2::Transform_1( std::vector<double>& iv, int cls) const
{
   // Normalization transformation
   if (cls < 0 || cls > 2) {
   if (2 > 1 ) cls = 2;
      else cls = 2;
   }
   const int nVar = 9;

   // get indices of used variables

   // define the indices of the variables which are transformed by this transformation
   static std::vector<int> indicesGet;
   static std::vector<int> indicesPut;

   if ( indicesGet.empty() ) {
      indicesGet.reserve(fNvars);
      indicesGet.push_back( 0);
      indicesGet.push_back( 1);
      indicesGet.push_back( 2);
      indicesGet.push_back( 3);
      indicesGet.push_back( 4);
      indicesGet.push_back( 5);
      indicesGet.push_back( 6);
      indicesGet.push_back( 7);
      indicesGet.push_back( 8);
   }
   if ( indicesPut.empty() ) {
      indicesPut.reserve(fNvars);
      indicesPut.push_back( 0);
      indicesPut.push_back( 1);
      indicesPut.push_back( 2);
      indicesPut.push_back( 3);
      indicesPut.push_back( 4);
      indicesPut.push_back( 5);
      indicesPut.push_back( 6);
      indicesPut.push_back( 7);
      indicesPut.push_back( 8);
   }

   std::vector<double> dv;
   dv.resize(nVar);
   for (int ivar=0; ivar<nVar; ivar++) dv[ivar] = iv[indicesGet.at(ivar)];
   for (int ivar=0;ivar<9;ivar++) {
      double offset = fOff_1[cls][ivar];
      double scale  = fScal_1[cls][ivar];
      iv[indicesPut.at(ivar)] = scale*dv[ivar]-offset;
   }
}

//_______________________________________________________________________
inline void ReadMLP2::InitTransform()
{
   InitTransform_1();
}

//_______________________________________________________________________
inline void ReadMLP2::Transform( std::vector<double>& iv, int sigOrBgd ) const
{
   Transform_1( iv, sigOrBgd );
}
#endif