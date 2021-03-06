/*****************************************************************/
/*    NAME: Henrik Schmidt                                       */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: FrontEstimate.h                                      */
/*    DATE: Aug. 2, 2008                                         */
/*                                                               */
/* This file is part of MOOS-IvP                                 */
/*                                                               */
/* MOOS-IvP is free software: you can redistribute it and/or     */
/* modify it under the terms of the GNU General Public License   */
/* as published by the Free Software Foundation, either version  */
/* 3 of the License, or (at your option) any later version.      */
/*                                                               */
/* MOOS-IvP is distributed in the hope that it will be useful,   */
/* but WITHOUT ANY WARRANTY; without even the implied warranty   */
/* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See  */
/* the GNU General Public License for more details.              */
/*                                                               */
/* You should have received a copy of the GNU General Public     */
/* License along with MOOS-IvP.  If not, see                     */
/* <http://www.gnu.org/licenses/>.                               */
/*****************************************************************/

#ifndef FRONTESTIMATE_HEADER
#define FRONTESTIMATE_HEADER
#include "MOOS/libMOOS/MOOSLib.h"
//#include "MOOSLIB/MOOSLib.h"
//#include "MOOSGenLib/MOOSGenLib.h"
//#include "MOOSUtilityLib/MOOSGeodesy.h"
#include "CSimAnneal2.h"
#include "CFrontSim2.h"
#include "Genetic.h"
#include <string>
#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"


class CFrontEstimate : public AppCastingMOOSApp
{
 public:
  CFrontEstimate();
  virtual ~CFrontEstimate();

  bool Iterate();
  bool OnNewMail(MOOSMSG_LIST &NewMail);
  bool OnConnectToServer();
  bool OnStartUp();
  bool buildReport();
 protected:

  CSimAnneal anneal;
  Genetic genetic;

  bool in_survey;
  bool completed;
  bool concurrent;
  bool adaptive;
  bool report_sent;
  bool new_anneal_report;
  double temp_fac;
  bool other_report_received;
  std::string other_report;
  bool final_report;
  bool first_report;
  // Front parameters
  double offset;
  double angle;
  double amplitude;
  double period;
  double wavelength;
  double alpha;
  double beta;
  double T_N;
  double T_S;
  double temperature;
  // David Front parameters
  double doffset;
  double dangle;
  double damplitude;
  double dperiod;
  double dwavelength;
  double dalpha;
  double dbeta;
  double dT_N;
  double dT_S;
  // Genetic Front parameters
  double goffset;
  double gangle;
  double gamplitude;
  double gperiod;
  double gwavelength;
  double galpha;
  double gbeta;
  double gT_N;
  double gT_S;
  // lower limits for annealer
  double min_offset;
  double min_angle;
  double min_amplitude;
  double min_period;
  double min_wavelength;
  double min_alpha;
  double min_beta;
  double min_T_N;
  double min_T_S;
  // upper limits for annealer
  double max_offset;
  double max_angle;
  double max_amplitude;
  double max_period;
  double max_wavelength;
  double max_alpha;
  double max_beta;
  double max_T_N;
  double max_T_S;
  double delta_t;

  double curr_time;
  double last_report;

  int num_param; 
  int cooling_steps;
  int num_meas;
  int anneal_step;

  std::string vname;

  std::string report_var;

  void postParameterReport();
  void postParameterReportDavid();
  void postParameterReportGenetic();
  void sendReportToOther();
};

#endif 




