// time_delays.h -- contains timeouts for commands and observations
// ******************all times are in milliseconds!********************

#ifndef _TIME_DELAYS_H
#define _TIME_DELAYS_H

#include "x34_enumapi.h"

unsigned int command_timeouts[NUM_command_types] = {
      60,   // test_sv33_valveCmdIn,
      60,   // test_sv03_sv_valveCmdIn,
      60    // test_sv31_sv_valveCmdIn,
};



unsigned int observation_timeouts[NUM_observation_types] = {
      1000, // test_mpre202p_pressureReading_pr02Crack,
      1000, // test_mpre202p_pressureReading_pressurizationRate_upperBound,
      1000, // test_mpre202p_pressureReading_pressurizationRate_lowerBound,
      1000, // test_mpre202p_pressureReading_boiloffRate_upperBound,
      1000, // test_mpre202p_pressureReading_boiloffRate_lowerBound,
      1000, // test_mpre202p_pressureReading_bleedRate_upperBound,
      1000, // test_mpre202p_pressureReading_bleedRate_lowerBound,
      1000, // test_mpre202p_pressureReading_ventingRate_upperBound,
      1000, // test_mpre202p_pressureReading_ventingRate_lowerBound,
      1000, // test_mpre212p_pressureReading_pr02Crack,
      1000, // test_mpre212p_pressureReading_pressurizationRate_upperBound,
      1000, // test_mpre212p_pressureReading_pressurizationRate_lowerBound,
      1000, // test_mpre212p_pressureReading_boiloffRate_upperBound,
      1000, // test_mpre212p_pressureReading_boiloffRate_lowerBound,
      1000, // test_mpre212p_pressureReading_bleedRate_upperBound,
      1000, // test_mpre212p_pressureReading_bleedRate_lowerBound,
      1000, // test_mpre212p_pressureReading_ventingRate_upperBound,
      1000, // test_mpre212p_pressureReading_ventingRate_lowerBound,
      1000, // test_mpre222p_pressureReading_pr02Crack,
      1000, // test_mpre222p_pressureReading_pressurizationRate_upperBound,
      1000, // test_mpre222p_pressureReading_pressurizationRate_lowerBound,
      1000, // test_mpre222p_pressureReading_boiloffRate_upperBound,
      1000, // test_mpre222p_pressureReading_boiloffRate_lowerBound,
      1000, // test_mpre222p_pressureReading_bleedRate_upperBound,
      1000, // test_mpre222p_pressureReading_bleedRate_lowerBound,
      1000, // test_mpre222p_pressureReading_ventingRate_upperBound,
      1000, // test_mpre222p_pressureReading_ventingRate_lowerBound,
      1000, // test_mpre103p_pressureReading_rg11_upperBound,
      1000, // test_mpre103p_pressureReading_rg11_lowerBound,
      1000, // test_mpre103p_pressureReading_rg01_upperBound,
      1000, // test_mpre103p_pressureReading_rg01_lowerBound,
      1000, // test_mpre102p_pressureReading_rg21_upperBound,
      1000, // test_mpre102p_pressureReading_rg21_lowerBound,
      1000, // test_mpre102p_pressureReading_rg02_upperBound,
      1000, // test_mpre102p_pressureReading_rg02_lowerBound,
      1000, // test_mpre101p_pressureReading_rg11_upperBound,
      1000, // test_mpre101p_pressureReading_rg11_lowerBound,
      1000, // test_mpre101p_pressureReading_rg01_upperBound,
      1000, // test_mpre101p_pressureReading_rg01_lowerBound,
      1000, // test_mrtd103t_temperatureReading_tankMixture_upperBound,
      1000, // test_mrtd103t_temperatureReading_tankMixture_lowerBound,
      1000, // test_mrtd103t_temperatureReading_gHe_upperBound,
      1000, // test_mrtd103t_temperatureReading_gHe_lowerBound,
      1000, // test_sv03_microSwitch,
      1000, // test_sv31_microSwitch,
      1000, // test_mpre104p_pressureReading_rg11_upperBound,
      1000, // test_mpre104p_pressureReading_rg11_lowerBound,
      1000, // test_mpre104p_pressureReading_rg01_upperBound,
      1000, // test_mpre104p_pressureReading_rg01_lowerBound,
      1000, // test_mrtd104t_temperatureReading_tankMixture_upperBound,
      1000, // test_mrtd104t_temperatureReading_tankMixture_lowerBound,
      1000, // test_mrtd104t_temperatureReading_gHe_upperBound,
      1000, // test_mrtd104t_temperatureReading_gHe_lowerBound,
      1000, // test_mrtd201t_temperatureReading_lO2_upperBound,
      1000, // test_mrtd201t_temperatureReading_lO2_lowerBound,
      1000, // test_mpre107p_pressureReading_rg21_upperBound,
      1000, // test_mpre107p_pressureReading_rg21_lowerBound,
      1000, // test_mpre107p_pressureReading_rg02_upperBound,
      1000, // test_mpre107p_pressureReading_rg02_lowerBound,
      1000, // test_mrtd203t_temperatureReading_tankMixture_upperBound,
      1000, // test_mrtd203t_temperatureReading_tankMixture_lowerBound,
      1000, // test_mrtd203t_temperatureReading_ambient_upperBound,
      1000, // test_mrtd203t_temperatureReading_ambient_lowerBound,
      1000, // test_mpre204p_pressureReading_bleedRate_upperBound,
      1000, // test_mpre204p_pressureReading_bleedRate_lowerBound,
      1000, // test_mrtd204t_temperatureReading_lO2_upperBound,
      1000, // test_mrtd204t_temperatureReading_lO2_lowerBound,
      1000, // test_mpre205p_pressureReading_bleedRate_upperBound,
      1000, // test_mpre205p_pressureReading_bleedRate_lowerBound,
      1000, // test_mrtd205t_temperatureReading_lO2_upperBound,
      1000, // test_mrtd205t_temperatureReading_lO2_lowerBound,
      1000, // test_pv03_positionIndicator_openIndicator,
      1000, // test_pv03_positionIndicator_closedIndicator,
      1000, // test_mpre201p_pressureReading_pr02Crack,
      1000, // test_mpre201p_pressureReading_pressurizationRate_upperBound,
      1000, // test_mpre201p_pressureReading_pressurizationRate_lowerBound,
      1000, // test_mpre201p_pressureReading_boiloffRate_upperBound,
      1000, // test_mpre201p_pressureReading_boiloffRate_lowerBound,
      1000, // test_mpre201p_pressureReading_bleedRate_upperBound,
      1000, // test_mpre201p_pressureReading_bleedRate_lowerBound,
      1000, // test_mpre201p_pressureReading_ventingRate_upperBound,
      1000, // test_mpre201p_pressureReading_ventingRate_lowerBound,
      1000  // test_aftLO2_lO2Level,
};

unsigned int get_command_timeout(unsigned int cmd)
{
	return command_timeouts[cmd];
}

unsigned int get_observation_timeout(unsigned int obs)
{
	return observation_timeouts[obs];
}



#endif

