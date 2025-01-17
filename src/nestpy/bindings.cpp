#include <pybind11/pybind11.h>
#include "NEST.hh"
#include "VDetector.hh"
#include "execNEST.hh"
#include "TestSpectra.hh"
#include "LUX_Run03.hh"
#include "DetectorExample_XENON10.hh"
#include "RandomGen.hh"
#include <pybind11/numpy.h>
#include <pybind11/stl_bind.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

namespace py = pybind11;

PYBIND11_MODULE(nestpy, m) {
  //	Binding for YieldResult struct
  RandomGen::rndm()->SetSeed( time(nullptr) );

  py::class_<NEST::YieldResult>(m, "YieldResult", py::dynamic_attr())
    .def(py::init<>())
	.def_readwrite("PhotonYield", &NEST::YieldResult::PhotonYield)
	.def_readwrite("ElectronYield", &NEST::YieldResult::ElectronYield)
	.def_readwrite("ExcitonRatio", &NEST::YieldResult::ExcitonRatio)
	.def_readwrite("Lindhard", &NEST::YieldResult::Lindhard)
	.def_readwrite("ElectricField", &NEST::YieldResult::ElectricField)
	.def_readwrite("DeltaT_Scint", &NEST::YieldResult::DeltaT_Scint);

  //	Binding for QuantaResult struct
  py::class_<NEST::QuantaResult>(m, "QuantaResult", py::dynamic_attr())
    .def(py::init<>())
	.def_readwrite("photons", &NEST::QuantaResult::photons)
	.def_readwrite("electrons", &NEST::QuantaResult::electrons)
	.def_readwrite("ions", &NEST::QuantaResult::ions)
	.def_readwrite("excitons", &NEST::QuantaResult::excitons);

  //	Binding for NESTresult struct
  py::class_<NEST::NESTresult>(m, "NESTresult", py::dynamic_attr())
    .def(py::init<>())
	.def_readwrite("yields", &NEST::NESTresult::yields)
	.def_readwrite("quanta", &NEST::NESTresult::quanta)
	.def_readwrite("photon_times", &NEST::NESTresult::photon_times);
 
  // Binding for Wvalue struct...
  py::class_<NEST::NESTcalc::Wvalue>(m, "Wvalue", py::dynamic_attr())
    .def(py::init<>())
        .def_readwrite("Wq_eV", &NEST::NESTcalc::Wvalue::Wq_eV)
        .def_readwrite("alpha", &NEST::NESTcalc::Wvalue::alpha);

  // Binding for the WIMP Spectrum Prep struct
  py::class_<TestSpectra::WIMP_spectrum_prep>(m, "WIMP_spectrum_prep", py::dynamic_attr())
    .def(py::init<>());
  //	Binding for the enumeration INTERACTION_TYPE
  py::enum_<NEST::INTERACTION_TYPE>(m, "INTERACTION_TYPE", py::arithmetic())
    .value("NR", NEST::INTERACTION_TYPE::NR)
    .value("WIMP", NEST::INTERACTION_TYPE::WIMP)
    .value("B8", NEST::INTERACTION_TYPE::B8)
    .value("DD", NEST::INTERACTION_TYPE::DD)
    .value("AmBe", NEST::INTERACTION_TYPE::AmBe)
    .value("Cf", NEST::INTERACTION_TYPE::Cf)
    .value("ion", NEST::INTERACTION_TYPE::ion)
    .value("gammaRay", NEST::INTERACTION_TYPE::gammaRay)
    .value("beta", NEST::INTERACTION_TYPE::beta)
    .value("CH3T", NEST::INTERACTION_TYPE::CH3T)
    .value("C14", NEST::INTERACTION_TYPE::C14)
    .value("Kr83m", NEST::INTERACTION_TYPE::Kr83m)
    .value("NoneType", NEST::INTERACTION_TYPE::NoneType)
    .export_values();


  py::enum_<NEST::S1CalculationMode>(m, "S1CalculationMode", py::arithmetic())
    .value("Full", NEST::S1CalculationMode::Full)
    .value("Parametric", NEST::S1CalculationMode::Parametric)
    .value("Hybrid", NEST::S1CalculationMode::Hybrid)
    .value("Waveform", NEST::S1CalculationMode::Waveform)
    .export_values();

  py::enum_<NEST::S2CalculationMode>(m, "S2CalculationMode", py::arithmetic())
    .value("Full", NEST::S2CalculationMode::Full)
    .value("Waveform", NEST::S2CalculationMode::Waveform)
    .value("WaveformWithEtrain", NEST::S2CalculationMode::WaveformWithEtrain)
    .export_values();


  //	Binding for the VDetector class
  py::class_<VDetector, std::unique_ptr<VDetector, py::nodelete>>(m, "VDetector")	//	py::nodelete added so that NESTcalc() deconstructor does
		  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	//	not delete instance of VDetector()
	.def(py::init<>())
	.def("Initialization", &VDetector::Initialization)
	.def("get_g1", &VDetector::get_g1)
	.def("get_sPEres", &VDetector::get_sPEres)
	.def("get_sPEthr", &VDetector::get_sPEthr)
	.def("get_sPEeff", &VDetector::get_sPEeff)
	.def("get_P_dphe", &VDetector::get_P_dphe)

	.def("get_coinWind", &VDetector::get_coinWind)
	.def("get_coinLevel", &VDetector::get_coinLevel)
	.def("get_numPMTs", &VDetector::get_numPMTs)

	.def("get_g1_gas", &VDetector::get_g1_gas)
	.def("get_s2Fano", &VDetector::get_s2Fano)
	.def("get_s2_thr", &VDetector::get_s2_thr)
	//.def("get_S2botTotRatio", &VDetector::get_S2botTotRatio)
	.def("get_E_gas", &VDetector::get_E_gas)
	.def("get_eLife_us", &VDetector::get_eLife_us)

	.def("get_inGas", &VDetector::get_inGas)
	.def("get_T_Kelvin", &VDetector::get_T_Kelvin)
	.def("get_p_bar", &VDetector::get_p_bar)

	.def("get_dtCntr", &VDetector::get_dtCntr)
	.def("get_dt_min", &VDetector::get_dt_min)
	.def("get_dt_max", &VDetector::get_dt_max)
	.def("get_radius", &VDetector::get_radius)
	.def("get_TopDrift", &VDetector::get_TopDrift)
	.def("get_anode", &VDetector::get_anode)
	.def("get_cathode", &VDetector::get_cathode)
	.def("get_gate", &VDetector::get_gate)

	.def("get_PosResExp", &VDetector::get_PosResExp)
	.def("get_PosResBase", &VDetector::get_PosResBase)

	.def("set_g1", &VDetector::set_g1)
	.def("set_sPEres", &VDetector::set_sPEres)
	.def("set_sPEthr", &VDetector::set_sPEthr)
	.def("set_sPEeff", &VDetector::set_sPEeff)
	.def("set_P_dphe", &VDetector::set_P_dphe)

	.def("set_coinWind", &VDetector::set_coinWind)
	.def("set_coinLevel", &VDetector::set_coinLevel)
	.def("set_numPMTs", &VDetector::set_numPMTs)

	.def("set_g1_gas", &VDetector::set_g1_gas)
	.def("set_s2Fano", &VDetector::set_s2Fano)
	.def("set_s2_thr", &VDetector::set_s2_thr)
	//.def("set_S2botTotRatio", &VDetector::set_S2botTotRatio)
	.def("set_E_gas", &VDetector::set_E_gas)
	.def("set_eLife_us", &VDetector::set_eLife_us)

	.def("set_inGas", &VDetector::set_inGas)
	.def("set_T_Kelvin", &VDetector::set_T_Kelvin)
	.def("set_p_bar", &VDetector::set_p_bar)

	.def("set_dtCntr", &VDetector::set_dtCntr)
	.def("set_dt_min", &VDetector::set_dt_min)
	.def("set_dt_max", &VDetector::set_dt_max)
	.def("set_radius", &VDetector::set_radius)
	.def("set_TopDrift", &VDetector::set_TopDrift)
	.def("set_anode", &VDetector::set_anode)
	.def("set_cathode", &VDetector::set_cathode)
	.def("set_gate", &VDetector::set_gate)

	.def("set_PosResExp", &VDetector::set_PosResExp)
	.def("set_PosResBase", &VDetector::set_PosResBase)

	.def("FitS1", &VDetector::FitS1)
	.def("FitS2", &VDetector::FitS1)
	.def("FitEF", &VDetector::FitEF)
	.def("FitTBA", &VDetector::FitTBA)
//    .def("FitS1", &VDetector::FitS1,
//         py::arg("xpos_mm") = 0.,
//         py::arg("ypos_mm") = 0.,
//         py::arg("zpos_mm") = 0.,
//         py::arg("LCE") = VDetector::LCE::unfold)
//	.def("FitS2", &VDetector::FitS2,
//	     py::arg("xpos_mm") = 0.,
//         py::arg("ypos_mm") = 0.,
//         py::arg("LCE") = VDetector::LCE::unfold)

	.def("OptTrans", &VDetector::OptTrans)
  	.def("SinglePEWaveForm", &VDetector::SinglePEWaveForm);
  
  //	Binding for example XENON10
  py::class_<DetectorExample_XENON10, VDetector, std::unique_ptr<DetectorExample_XENON10, py::nodelete>>(m, "DetectorExample_XENON10")
     .def(py::init<>())
	 .def("Initialization", &DetectorExample_XENON10::Initialization)
	 .def("FitTBA", &DetectorExample_XENON10::FitTBA)
	 .def("OptTrans", &DetectorExample_XENON10::OptTrans)
	 .def("SinglePEWaveForm", &DetectorExample_XENON10::SinglePEWaveForm);
//
 //	Binding for example LUX_Run03
 py::class_<DetectorExample_LUX_RUN03, VDetector, std::unique_ptr<DetectorExample_LUX_RUN03, py::nodelete>>(m, "LUX_Run03")
    .def(py::init<>())
	 .def("Initialization", &DetectorExample_LUX_RUN03::Initialization)
	 .def("FitTBA", &DetectorExample_LUX_RUN03::FitTBA)
	 .def("OptTrans", &DetectorExample_LUX_RUN03::OptTrans)
	 .def("SinglePEWaveForm", &DetectorExample_LUX_RUN03::SinglePEWaveForm);
  //py::class_<VDetector, std::unique_ptr<VDetector, py::nodelete>>(m, "VDetector")
  // Binding for the TestSpectra class
  py::class_<TestSpectra, std::unique_ptr<TestSpectra, py::nodelete>>(m, "TestSpectra")
     .def(py::init<>())
        .def_static("CH3T_spectrum", 
               &TestSpectra::CH3T_spectrum, 
               py::arg("xMin") = 0.,
               py::arg("xMax") = 18.6
             )
	
        .def_static("C14_spectrum",
               &TestSpectra::C14_spectrum,
               py::arg("xMin") = 0.,
               py::arg("xMax") = 156.
            )
        .def_static("B8_spectrum", 
               &TestSpectra::B8_spectrum,
               py::arg("xMin") = 0.,
               py::arg("xMax") = 4.
            )
        .def_static("AmBe_spectrum", 
               &TestSpectra::AmBe_spectrum,
               py::arg("xMin") = 0.,
               py::arg("xMax") = 200.
            )
        .def_static("Cf_spectrum", 
              &TestSpectra::Cf_spectrum,
              py::arg("xMin") = 0.,
              py::arg("xMax") = 200.
            )
        .def_static("DD_spectrum",
              &TestSpectra::DD_spectrum,
              py::arg("xMin") = 0.,
              py::arg("xMax") = 80.,
	      py::arg("expFall") =  10.,
	      py::arg("peakFrac") = 0.1,
	      py::arg("peakMu") = 60.,
	      py::arg("peakSig") = 25.
            )
        .def_static("ppSolar_spectrum", 
              &TestSpectra::ppSolar_spectrum,
              py::arg("xMin") = 0.,
              py::arg("xMax") = 250.
             )
        .def_static("atmNu_spectrum",
              &TestSpectra::atmNu_spectrum,
              py::arg("xMin") = 0.,
              py::arg("xMax") = 85.
            )
	.def_static("WIMP_prep_spectrum", 
	      &TestSpectra::WIMP_prep_spectrum,
	      py::arg("mass") = 50.,
	      py::arg("eStep") = 5.,
	      py::arg("day")=0.
	    )
	.def_static("WIMP_spectrum",
	      &TestSpectra::WIMP_spectrum,
	      py::arg("wprep"),
	      py::arg("mass") = 50.,
	      py::arg("day") = 0.
	    );
        
  //	Binding for the NESTcalc class
  py::class_<NEST::NESTcalc, std::unique_ptr<NEST::NESTcalc, py::nodelete>>(m, "NESTcalc")
    //.def(py::init<>())
	.def(py::init<VDetector*>())
    .def_readonly_static("default_NRYieldsParam", &NESTcalc::default_NRYieldsParam)
    .def_readonly_static("default_NRERWidthsParam", &NESTcalc::default_NRERWidthsParam)
//     .def_static("BinomFluct", &NEST::NESTcalc::BinomFluct)
	.def("FullCalculation", &NEST::NESTcalc::FullCalculation,
			"Perform the full yield calculation with smearings")
	.def("PhotonTime", &NEST::NESTcalc::PhotonTime)
	.def("AddPhotonTransportTime", &NEST::NESTcalc::AddPhotonTransportTime)
	.def("GetPhotonTimes", &NEST::NESTcalc::GetPhotonTimes)
    .def("GetYieldKr83m",
	     &NEST::NESTcalc::GetYieldKr83m,
	     py::arg("energy") = 41.5,
	     py::arg("density") = 2.9,
	     py::arg("drift_field") = 124,
	     py::arg("maxTimeSeparation") = 2000.,
	     py::arg("minTimeSeparation") = 0.0
	 )
    .def("GetYieldERWeighted", 
	     &NEST::NESTcalc::GetYieldERWeighted, 
	     py::arg("energy") = 5.2,
	     py::arg("density") = 2.9, 
	     py::arg("drift_field") = 124,
	     py::arg("nuisance_parameters") = std::vector<double>({ 11., 1.1, 0.0480, -0.0533, 12.6, 0.3, 2., 0.3, 2., 0.5, 1., 1.})
	)
    .def("GetYields",
	     &NEST::NESTcalc::GetYields,
	     py::arg("interaction") = NEST::INTERACTION_TYPE::NR,
	     py::arg("energy") = 100,
	     py::arg("density") = 2.9,
	     py::arg("drift_field") = 124,
	     py::arg("A") = 131.293,
	     py::arg("Z") = 54,
	     py::arg("nuisance_parameters") = std::vector<double>({ 11., 1.1, 0.0480, -0.0533, 12.6, 0.3, 2., 0.3, 2., 0.5, 1., 1.}), 
         py::arg("oldModelER") = false
	 )
    .def("GetQuanta", &NEST::NESTcalc::GetQuanta,
	    py::arg("yields"),
	    py::arg("density") = 2.9,
	    py::arg("free_parameters") = std::vector<double>({1.,1.,0.1,0.5,0.19,2.25, 0.0015, 0.0553, 0.205, 0.45, -0.2}), 
        py::arg("oldModelER") = false
     )   
	.def("GetS1", &NEST::NESTcalc::GetS1)
	.def("GetSpike", &NEST::NESTcalc::GetSpike)
	// Currently VDetector.FitTBA() requires we reinitialize the detector every time:
	.def("GetS2", &NEST::NESTcalc::GetS2)
	.def("CalculateG2", &NEST::NESTcalc::CalculateG2)
	.def("SetDriftVelocity", &NEST::NESTcalc::SetDriftVelocity)
	.def("SetDriftVelocity_NonUniform", &NEST::NESTcalc::SetDriftVelocity_NonUniform)
	.def("SetDensity", &NEST::NESTcalc::SetDensity)
	.def_static("GetDensity", &NEST::NESTcalc::GetDensity,
                py::arg("T") = 174.,
                py::arg("P") = 1.80,
                py::arg("inGas") = false,
                py::arg("evtNum") = 0,
                py::arg("molarMass") = 131.293
        )
        .def_static("WorkFunction", &NEST::NESTcalc::WorkFunction,
                py::arg("rho") = 2.89,
                py::arg("MolarMass") = 131.293,
                py::arg("OldW13eV") = true
        )

	// Currently VDetector.FitTBA() requires we reinitialize the detector every time:
	.def("xyResolution", &NEST::NESTcalc::xyResolution)
	.def("PhotonEnergy", &NEST::NESTcalc::PhotonEnergy)
	.def("CalcElectronLET", &NEST::NESTcalc::CalcElectronLET)
	.def("GetDetector", &NEST::NESTcalc::GetDetector);
   
  //	execNEST function
  m.def("execNEST", &execNEST);
  m.def("GetEnergyRes", &GetEnergyRes);
  m.def("GetBand", &GetBand);
  
}

