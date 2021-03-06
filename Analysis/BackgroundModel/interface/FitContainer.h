#ifndef Analysis_BackgroundModel_FitContainer_h
#define Analysis_BackgroundModel_FitContainer_h 1

#include <vector>
#include <string>
#include <memory>
#include "TH1.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "RooCurve.h"
#include "RooAbsPdf.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooWorkspace.h"
#include "RooFitResult.h"
#include "Analysis/BackgroundModel/interface/HistContainer.h"
#include "Analysis/BackgroundModel/interface/TreeContainer.h"
#include "Analysis/BackgroundModel/interface/ParamModifier.h"


namespace analysis {
  namespace backgroundmodel {

    class FitContainer {
    public:
      enum class Type { signal, background };
      inline static std::string toString(const Type& type) {
        switch (type) {
        case Type::signal: return "signal";
        case Type::background: return "background";
        };
        return "";              // to silence compiler
      };

      FitContainer(const TH1& data, const TH1& signal, const TH1& background,
		   const std::string& outputDir = defaultOutputDir_);
      FitContainer(TTree& data, const std::string& outputDir = defaultOutputDir_);
      FitContainer(const HistContainer& container,
		   const std::string& outputDir = defaultOutputDir_);
      FitContainer(const TreeContainer& container,
		   const std::string& outputDir = defaultOutputDir_);
      virtual ~FitContainer();
      void initialize();

      FitContainer(const FitContainer&) = default;
      FitContainer& operator=(const FitContainer&) = default;
      FitContainer(FitContainer&&) = default;
      FitContainer& operator=(FitContainer&&) = default;

      FitContainer& verbosity(int level);
      FitContainer& fitRangeMin(float min);
      FitContainer& fitRangeMax(float max);

      inline static const std::vector<std::string>& availableModels() {
        return availableModels_; };
      void setModel(const Type& type, const std::string& model);
      void setModel(const Type& type, const std::string& model,
                    const std::vector<ParamModifier>& modifiers);
      std::unique_ptr<RooFitResult> backgroundOnlyFit();
      void profileModel(const Type& type);
      void showModels() const;

    private:
      // methods to set the fit model
      static std::unique_ptr<RooArgList>
      getCoefficients_(const int numCoeffs, const std::string& name);
      void setNovosibirsk_(const Type& type);
      void setCrystalBall_(const Type& type);
      void setNovoEffProd_(const Type& type);
      void setCBEffProd_(const Type& type);
      void setExpEffProd_(const Type& type);
      void setDoubleCB_(const Type& type);
      void setExpGausExp_(const Type& type);
      void setExpBWExp_(const Type& type);
      void setBukin_(const Type& type);
      void setBernstein_(const Type& type, const int numCoeffs);
      void setChebychev_(const Type& type, const int numCoeffs);
      void setBernEffProd_(const Type& type, const int numCoeffs);
      void setChebEffProd_(const Type& type, const int numCoeffs);
      static const std::vector<std::string> availableModels_;

      // internal methods
      static void prepareCanvas_(TCanvas& raw);
      static void prepareFrame_(RooPlot& raw);
      std::string getOutputPath_(const std::string& subdirectory = "");
      double getPeakStart_(const Type& type);
      double getPeakStart_(const Type& type, double max);
      double getMaxPosition_(const RooAbsData& data);
      int getNonZeroBins_(const RooAbsData& data);
      double chiSquare_(const RooAbsData& data, const RooCurve& fit);
      bool applyModifiers_(RooAbsPdf& pdf,
                           const std::vector<ParamModifier>& modifiers);

      // data member
      static const int defaultNumberOfCoefficients_;
      static const std::string defaultOutputDir_;
      bool initialized_;
      std::string outputDir_;
      std::string plotDir_;
      std::string workspaceDir_;
      std::string fullRangeId_;
      std::string fitRangeId_;
      int verbosity_;
      RooWorkspace workspace_;
      std::string outRootFileName_;
      std::string mbb_;
      std::string weight_;
      std::string data_;
      std::string signal_;
      std::string bkg_;
      float fitRangeMin_;
      float fitRangeMax_;
      TTree bkgOnlyFit_;
      float chi2BkgOnly_;
      float normChi2BkgOnly_;
      int ndfBkgOnly_;
    };

  }
}

#endif  // Analysis_BackgroundModel_FitContainer_h
