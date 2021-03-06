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
Date           : Mon Jun  3 07:22:20 2019
Host           : Linux SFT-ubuntu-1804-3 4.15.0-38-generic #41-Ubuntu SMP Wed Oct 10 10:59:38 UTC 2018 x86_64 x86_64 x86_64 GNU/Linux
Dir            : /home/marcel/workspace/tracking/macro
Training events: 600000
Analysis type  : [Classification]


#OPT -*-*-*-*-*-*-*-*-*-*-*-*- options -*-*-*-*-*-*-*-*-*-*-*-*-

# Set by User:
NCycles: "500" [Number of training cycles]
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
abs(abs(phi1)-1.57079632679)   abs_abs_phi1__M_1.57079632679_ abs(abs(phi1)-1.57079632679)   abs(abs(phi1)-1.57079632679)                                      'F'    [1.62925573477e-07,1.57078564167]
abs(z1)                       abs_z1_                       abs(z1)                       abs(z1)                                                         'F'    [0.000503600982483,1083.39990234]
rz2                           rz2                           rz2                           rz2                                                             'F'    [30.2561168671,1022.02819824]
abs(abs(phi2)-1.57079632679)   abs_abs_phi2__M_1.57079632679_ abs(abs(phi2)-1.57079632679)   abs(abs(phi2)-1.57079632679)                                      'F'    [1.62925573477e-07,1.57079041004]
abs(z2)                       abs_z2_                       abs(z2)                       abs(z2)                                                         'F'    [2.86101999336e-06,1225.5]
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
      const char* inputVars[] = { "rz1", "abs(abs(phi1)-1.57079632679)", "abs(z1)", "rz2", "abs(abs(phi2)-1.57079632679)", "abs(z2)", "f0", "f1", "log(score)" };

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
      fVmax[4] = 0.99999988079071;
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
   fWeightMatrix0to1[0][0] = -6.87213486543346;
   fWeightMatrix0to1[1][0] = -17.5520406501389;
   fWeightMatrix0to1[2][0] = -0.138759977027908;
   fWeightMatrix0to1[3][0] = -20.5503480408503;
   fWeightMatrix0to1[4][0] = -0.507141310277937;
   fWeightMatrix0to1[5][0] = 0.644295193536623;
   fWeightMatrix0to1[6][0] = -9.62015889291577;
   fWeightMatrix0to1[7][0] = 3.59481469970514;
   fWeightMatrix0to1[8][0] = 2.30172322817873;
   fWeightMatrix0to1[9][0] = -1.69405691716145;
   fWeightMatrix0to1[10][0] = 2.29165475574181;
   fWeightMatrix0to1[11][0] = -9.15041864435719;
   fWeightMatrix0to1[12][0] = -4.55220658367398;
   fWeightMatrix0to1[13][0] = -8.28421016976475;
   fWeightMatrix0to1[14][0] = -2.42823054649716;
   fWeightMatrix0to1[0][1] = 0.116649360971888;
   fWeightMatrix0to1[1][1] = 0.0725803328387316;
   fWeightMatrix0to1[2][1] = -0.0437794898781421;
   fWeightMatrix0to1[3][1] = 0.000503262596171826;
   fWeightMatrix0to1[4][1] = -0.0920683927539157;
   fWeightMatrix0to1[5][1] = 0.0206187343142458;
   fWeightMatrix0to1[6][1] = -18.8971194333148;
   fWeightMatrix0to1[7][1] = 0.610206287103242;
   fWeightMatrix0to1[8][1] = -0.117982423806142;
   fWeightMatrix0to1[9][1] = 0.0482232816798229;
   fWeightMatrix0to1[10][1] = -0.171827522352269;
   fWeightMatrix0to1[11][1] = 17.4245949651197;
   fWeightMatrix0to1[12][1] = -0.0941711616783385;
   fWeightMatrix0to1[13][1] = 0.280545470514762;
   fWeightMatrix0to1[14][1] = -0.0900982697853289;
   fWeightMatrix0to1[0][2] = -1.05667900880211;
   fWeightMatrix0to1[1][2] = 6.19662448331925;
   fWeightMatrix0to1[2][2] = -2.379098889932;
   fWeightMatrix0to1[3][2] = 6.01786985937164;
   fWeightMatrix0to1[4][2] = 3.21138978116686;
   fWeightMatrix0to1[5][2] = 0.984549690441008;
   fWeightMatrix0to1[6][2] = -1.78391389539327;
   fWeightMatrix0to1[7][2] = -18.8828174500832;
   fWeightMatrix0to1[8][2] = -0.394665764711573;
   fWeightMatrix0to1[9][2] = -0.0113852480344594;
   fWeightMatrix0to1[10][2] = -0.949707342123545;
   fWeightMatrix0to1[11][2] = -1.70984660485682;
   fWeightMatrix0to1[12][2] = 6.93571720627569;
   fWeightMatrix0to1[13][2] = 30.6875922759295;
   fWeightMatrix0to1[14][2] = 0.307609114414725;
   fWeightMatrix0to1[0][3] = 5.72860421093532;
   fWeightMatrix0to1[1][3] = 20.1936007510857;
   fWeightMatrix0to1[2][3] = 3.37932566631346;
   fWeightMatrix0to1[3][3] = 32.8681450813721;
   fWeightMatrix0to1[4][3] = 2.87912646353156;
   fWeightMatrix0to1[5][3] = -2.02838706094876;
   fWeightMatrix0to1[6][3] = 11.9055082514324;
   fWeightMatrix0to1[7][3] = -4.87405859287052;
   fWeightMatrix0to1[8][3] = -6.73159389821071;
   fWeightMatrix0to1[9][3] = 0.796081879875614;
   fWeightMatrix0to1[10][3] = -6.867338336818;
   fWeightMatrix0to1[11][3] = 11.0606948522724;
   fWeightMatrix0to1[12][3] = -15.2907933348022;
   fWeightMatrix0to1[13][3] = 6.84728725038673;
   fWeightMatrix0to1[14][3] = -2.1092102602028;
   fWeightMatrix0to1[0][4] = -0.097418683626735;
   fWeightMatrix0to1[1][4] = -0.0451089011170155;
   fWeightMatrix0to1[2][4] = 0.00923386556097506;
   fWeightMatrix0to1[3][4] = 0.00132543963346109;
   fWeightMatrix0to1[4][4] = 0.13763038682498;
   fWeightMatrix0to1[5][4] = -0.00918866450717757;
   fWeightMatrix0to1[6][4] = 18.8743154583386;
   fWeightMatrix0to1[7][4] = -0.593048599021198;
   fWeightMatrix0to1[8][4] = 0.111473248951736;
   fWeightMatrix0to1[9][4] = -0.0199858663446158;
   fWeightMatrix0to1[10][4] = 0.236739381033123;
   fWeightMatrix0to1[11][4] = -17.4479587643652;
   fWeightMatrix0to1[12][4] = 0.0187388325080864;
   fWeightMatrix0to1[13][4] = -0.258410715082834;
   fWeightMatrix0to1[14][4] = 0.0854565649892042;
   fWeightMatrix0to1[0][5] = 0.6073241176417;
   fWeightMatrix0to1[1][5] = -7.36818827733367;
   fWeightMatrix0to1[2][5] = 2.8806134624764;
   fWeightMatrix0to1[3][5] = -8.44937450591381;
   fWeightMatrix0to1[4][5] = -5.60862254483832;
   fWeightMatrix0to1[5][5] = 0.509304883942417;
   fWeightMatrix0to1[6][5] = 1.90092924679087;
   fWeightMatrix0to1[7][5] = 23.8191042788911;
   fWeightMatrix0to1[8][5] = 0.917414207235189;
   fWeightMatrix0to1[9][5] = -0.271616932521629;
   fWeightMatrix0to1[10][5] = 4.01847457691816;
   fWeightMatrix0to1[11][5] = 1.83859975224411;
   fWeightMatrix0to1[12][5] = -7.32772034092248;
   fWeightMatrix0to1[13][5] = -32.4682178853362;
   fWeightMatrix0to1[14][5] = -0.502132923382214;
   fWeightMatrix0to1[0][6] = 0.301212380215232;
   fWeightMatrix0to1[1][6] = -10.5449470971337;
   fWeightMatrix0to1[2][6] = 5.83454209064278;
   fWeightMatrix0to1[3][6] = -25.1847759731648;
   fWeightMatrix0to1[4][6] = -0.0466884308909357;
   fWeightMatrix0to1[5][6] = 2.68057150708666;
   fWeightMatrix0to1[6][6] = 0.495095187353585;
   fWeightMatrix0to1[7][6] = -8.37096331666253;
   fWeightMatrix0to1[8][6] = -3.57088953790909;
   fWeightMatrix0to1[9][6] = -0.0868412178787534;
   fWeightMatrix0to1[10][6] = -11.0615111398672;
   fWeightMatrix0to1[11][6] = 0.436581492288087;
   fWeightMatrix0to1[12][6] = 5.40690714579456;
   fWeightMatrix0to1[13][6] = 4.80736224795465;
   fWeightMatrix0to1[14][6] = -0.269627552994944;
   fWeightMatrix0to1[0][7] = -0.0244739739032425;
   fWeightMatrix0to1[1][7] = -0.0891242405632198;
   fWeightMatrix0to1[2][7] = -6.23894852185139;
   fWeightMatrix0to1[3][7] = -0.238514557652964;
   fWeightMatrix0to1[4][7] = 5.7179324227359;
   fWeightMatrix0to1[5][7] = -4.34455729225544;
   fWeightMatrix0to1[6][7] = 0.0374200470011271;
   fWeightMatrix0to1[7][7] = -1.45800314715588;
   fWeightMatrix0to1[8][7] = -0.292780371234769;
   fWeightMatrix0to1[9][7] = -0.0175034113554144;
   fWeightMatrix0to1[10][7] = 1.45450555397987;
   fWeightMatrix0to1[11][7] = 0.0867797258568665;
   fWeightMatrix0to1[12][7] = -0.853116809766243;
   fWeightMatrix0to1[13][7] = 0.333718862237445;
   fWeightMatrix0to1[14][7] = -0.684378975009023;
   fWeightMatrix0to1[0][8] = -11.195279723413;
   fWeightMatrix0to1[1][8] = 0.313717373431872;
   fWeightMatrix0to1[2][8] = -0.372738527948388;
   fWeightMatrix0to1[3][8] = -0.635795366800104;
   fWeightMatrix0to1[4][8] = 0.157662461766212;
   fWeightMatrix0to1[5][8] = -0.0394269915349712;
   fWeightMatrix0to1[6][8] = -0.139755520835069;
   fWeightMatrix0to1[7][8] = -0.508607439926629;
   fWeightMatrix0to1[8][8] = -0.130770971511997;
   fWeightMatrix0to1[9][8] = 4.87418849636135;
   fWeightMatrix0to1[10][8] = 0.321942677916858;
   fWeightMatrix0to1[11][8] = -0.0298849827690222;
   fWeightMatrix0to1[12][8] = -0.0438163107051923;
   fWeightMatrix0to1[13][8] = -1.29403429814012;
   fWeightMatrix0to1[14][8] = 0.133364744138509;
   fWeightMatrix0to1[0][9] = 7.24923591077558;
   fWeightMatrix0to1[1][9] = -3.94515779221478;
   fWeightMatrix0to1[2][9] = -3.04285981192174;
   fWeightMatrix0to1[3][9] = -9.9143318486088;
   fWeightMatrix0to1[4][9] = 4.29697885161851;
   fWeightMatrix0to1[5][9] = 0.110576487238329;
   fWeightMatrix0to1[6][9] = 3.81579154629305;
   fWeightMatrix0to1[7][9] = -5.73312934257135;
   fWeightMatrix0to1[8][9] = -4.1621055694517;
   fWeightMatrix0to1[9][9] = -3.1408042565231;
   fWeightMatrix0to1[10][9] = 3.9294864326436;
   fWeightMatrix0to1[11][9] = 3.34694074267878;
   fWeightMatrix0to1[12][9] = -9.36957014250475;
   fWeightMatrix0to1[13][9] = 0.362375476283062;
   fWeightMatrix0to1[14][9] = -2.20216125304876;
   // weight matrix from layer 1 to 2
   fWeightMatrix1to2[0][0] = -0.0673112946578848;
   fWeightMatrix1to2[1][0] = 0.596287011886136;
   fWeightMatrix1to2[2][0] = -0.255009731162518;
   fWeightMatrix1to2[3][0] = -0.582195691931878;
   fWeightMatrix1to2[4][0] = 0.91068641124474;
   fWeightMatrix1to2[0][1] = 0.380507472766648;
   fWeightMatrix1to2[1][1] = 0.392710907851759;
   fWeightMatrix1to2[2][1] = 2.46314885193873;
   fWeightMatrix1to2[3][1] = -0.530076518703924;
   fWeightMatrix1to2[4][1] = 0.162331467110493;
   fWeightMatrix1to2[0][2] = -2.4244920369125;
   fWeightMatrix1to2[1][2] = -0.785085502127735;
   fWeightMatrix1to2[2][2] = -1.20350594482069;
   fWeightMatrix1to2[3][2] = -6.65465111011353;
   fWeightMatrix1to2[4][2] = -0.0810100920136157;
   fWeightMatrix1to2[0][3] = -0.68387831332526;
   fWeightMatrix1to2[1][3] = -0.918184708689561;
   fWeightMatrix1to2[2][3] = -0.492053823372702;
   fWeightMatrix1to2[3][3] = 1.51570988644646;
   fWeightMatrix1to2[4][3] = -0.100698099968597;
   fWeightMatrix1to2[0][4] = -1.11093963825306;
   fWeightMatrix1to2[1][4] = -0.996835360927341;
   fWeightMatrix1to2[2][4] = 0.198336298674144;
   fWeightMatrix1to2[3][4] = -2.26644845680154;
   fWeightMatrix1to2[4][4] = -0.148090218397599;
   fWeightMatrix1to2[0][5] = -1.253863331453;
   fWeightMatrix1to2[1][5] = -0.830615126504385;
   fWeightMatrix1to2[2][5] = 1.49585609235345;
   fWeightMatrix1to2[3][5] = -2.14075960736507;
   fWeightMatrix1to2[4][5] = -0.1742974843282;
   fWeightMatrix1to2[0][6] = -0.745823158922527;
   fWeightMatrix1to2[1][6] = -0.418890381733239;
   fWeightMatrix1to2[2][6] = 1.37906915150028;
   fWeightMatrix1to2[3][6] = -2.51117198761845;
   fWeightMatrix1to2[4][6] = -0.129178244670961;
   fWeightMatrix1to2[0][7] = -0.794375532333505;
   fWeightMatrix1to2[1][7] = -1.38523874043991;
   fWeightMatrix1to2[2][7] = -0.772878870091409;
   fWeightMatrix1to2[3][7] = -1.87266736678672;
   fWeightMatrix1to2[4][7] = -0.115526439538286;
   fWeightMatrix1to2[0][8] = -1.25519625639601;
   fWeightMatrix1to2[1][8] = 1.98326820695317;
   fWeightMatrix1to2[2][8] = -1.77515400193929;
   fWeightMatrix1to2[3][8] = 4.03272752128451;
   fWeightMatrix1to2[4][8] = 0.414335731871348;
   fWeightMatrix1to2[0][9] = -0.075750395647971;
   fWeightMatrix1to2[1][9] = -0.185765234612777;
   fWeightMatrix1to2[2][9] = -0.162356895145507;
   fWeightMatrix1to2[3][9] = 0.12241403482315;
   fWeightMatrix1to2[4][9] = -1.16469060960265;
   fWeightMatrix1to2[0][10] = -1.67702849289581;
   fWeightMatrix1to2[1][10] = 3.6433376624087;
   fWeightMatrix1to2[2][10] = -0.632861009195187;
   fWeightMatrix1to2[3][10] = -13.2728295102273;
   fWeightMatrix1to2[4][10] = -0.0373977626633283;
   fWeightMatrix1to2[0][11] = -0.800414973180615;
   fWeightMatrix1to2[1][11] = -0.384674485923128;
   fWeightMatrix1to2[2][11] = 1.38738139058183;
   fWeightMatrix1to2[3][11] = -2.31879067036249;
   fWeightMatrix1to2[4][11] = -0.155832783282423;
   fWeightMatrix1to2[0][12] = 1.53866750834591;
   fWeightMatrix1to2[1][12] = 0.0763162901925373;
   fWeightMatrix1to2[2][12] = 0.00431406643277863;
   fWeightMatrix1to2[3][12] = 0.068493647061988;
   fWeightMatrix1to2[4][12] = 0.0994791595082079;
   fWeightMatrix1to2[0][13] = 0.268989631620774;
   fWeightMatrix1to2[1][13] = -0.51825357719417;
   fWeightMatrix1to2[2][13] = -0.133065108249526;
   fWeightMatrix1to2[3][13] = 4.50163699860819;
   fWeightMatrix1to2[4][13] = -0.0590215434949789;
   fWeightMatrix1to2[0][14] = -0.578373594980653;
   fWeightMatrix1to2[1][14] = -0.850718320629043;
   fWeightMatrix1to2[2][14] = -3.07629539002972;
   fWeightMatrix1to2[3][14] = -0.670077533033451;
   fWeightMatrix1to2[4][14] = 0.0792875787167709;
   fWeightMatrix1to2[0][15] = 0.230945266462895;
   fWeightMatrix1to2[1][15] = -4.43349512231393;
   fWeightMatrix1to2[2][15] = -0.730589966610315;
   fWeightMatrix1to2[3][15] = 15.109509581861;
   fWeightMatrix1to2[4][15] = 1.44910948366494;
   // weight matrix from layer 2 to 3
   fWeightMatrix2to3[0][0] = -2.66569736265687;
   fWeightMatrix2to3[0][1] = 1.65950744648607;
   fWeightMatrix2to3[0][2] = 2.99055595184766;
   fWeightMatrix2to3[0][3] = -0.890852056572767;
   fWeightMatrix2to3[0][4] = 4.40209033218697;
   fWeightMatrix2to3[0][5] = -7.42499746981645;
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
   fMin_1[0][1] = 1.62925573477e-07;
   fMax_1[0][1] = 1.57078564167;
   fScal_1[0][1] = 2.0/(fMax_1[0][1]-fMin_1[0][1]);
   fOff_1[0][1] = fMin_1[0][1]*fScal_1[0][1]+1.;
   fMin_1[1][1] = 1.62925573477e-07;
   fMax_1[1][1] = 1.57078564167;
   fScal_1[1][1] = 2.0/(fMax_1[1][1]-fMin_1[1][1]);
   fOff_1[1][1] = fMin_1[1][1]*fScal_1[1][1]+1.;
   fMin_1[2][1] = 1.62925573477e-07;
   fMax_1[2][1] = 1.57078564167;
   fScal_1[2][1] = 2.0/(fMax_1[2][1]-fMin_1[2][1]);
   fOff_1[2][1] = fMin_1[2][1]*fScal_1[2][1]+1.;
   fMin_1[0][2] = 0.000503600982483;
   fMax_1[0][2] = 1083.39990234;
   fScal_1[0][2] = 2.0/(fMax_1[0][2]-fMin_1[0][2]);
   fOff_1[0][2] = fMin_1[0][2]*fScal_1[0][2]+1.;
   fMin_1[1][2] = 0.000503600982483;
   fMax_1[1][2] = 1083.39990234;
   fScal_1[1][2] = 2.0/(fMax_1[1][2]-fMin_1[1][2]);
   fOff_1[1][2] = fMin_1[1][2]*fScal_1[1][2]+1.;
   fMin_1[2][2] = 0.000503600982483;
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
   fMin_1[0][4] = 1.62925573477e-07;
   fMax_1[0][4] = 1.57079041004;
   fScal_1[0][4] = 2.0/(fMax_1[0][4]-fMin_1[0][4]);
   fOff_1[0][4] = fMin_1[0][4]*fScal_1[0][4]+1.;
   fMin_1[1][4] = 1.62925573477e-07;
   fMax_1[1][4] = 1.57078564167;
   fScal_1[1][4] = 2.0/(fMax_1[1][4]-fMin_1[1][4]);
   fOff_1[1][4] = fMin_1[1][4]*fScal_1[1][4]+1.;
   fMin_1[2][4] = 1.62925573477e-07;
   fMax_1[2][4] = 1.57079041004;
   fScal_1[2][4] = 2.0/(fMax_1[2][4]-fMin_1[2][4]);
   fOff_1[2][4] = fMin_1[2][4]*fScal_1[2][4]+1.;
   fMin_1[0][5] = 0.000503600982483;
   fMax_1[0][5] = 1225.5;
   fScal_1[0][5] = 2.0/(fMax_1[0][5]-fMin_1[0][5]);
   fOff_1[0][5] = fMin_1[0][5]*fScal_1[0][5]+1.;
   fMin_1[1][5] = 2.86101999336e-06;
   fMax_1[1][5] = 1225.5;
   fScal_1[1][5] = 2.0/(fMax_1[1][5]-fMin_1[1][5]);
   fOff_1[1][5] = fMin_1[1][5]*fScal_1[1][5]+1.;
   fMin_1[2][5] = 2.86101999336e-06;
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
