/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT                                             */
/*    FILE: GenPath.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "GenPath.h"
#include "CompPath.h"
#include <cstdint>
#include<sstream>
#include<unistd.h>
#include"XYPoint.h"
#include"XYSegList.h"
#include<math.h>

using namespace std;


int visit_radius = 5;

//---------------------------------------------------------
// Constructor

GenPath::GenPath()
{
// m_got_all_points = false;
// m_sent_all_points = false;
// m_register_start = false;
}

//---------------------------------------------------------
// Destructor

GenPath::~GenPath()
{
}

void GenPath::setStart(XYPoint point)
{

// XYPoint point2(0,-40);

// m_start_point = point2;
// m_register_start = true; 

}

void GenPath::testComp()
{

  list<CompPath>::iterator l;
  for(l=m_list.begin(); l!=m_list.end();) {
    CompPath &lobj = *l;
    reportEvent("ID="+lobj.m_id+"X="+lobj.m_x+"Y="+lobj.m_y);

    double x_pt,y_pt;

    x_pt = atof(lobj.m_x.c_str());
    y_pt = atof(lobj.m_y.c_str());

    double hyp = hypot(x_pt-m_x_curr,y_pt-m_y_curr);

    if((hyp < visit_radius)) {// && (lobj.m_id!="firstpoint") && (lobj.m_id!="lastpoint")) {
      l = m_list.erase(l);
    }   
    // if(lobj.m_id!="firstpoint") {
    //   l = m_list.erase(l);
    // }
    else {
      ++l;
    }
  }
}

void GenPath::sendPoints()
{
  my_seglist.clear();
  double x,y;
  string label,color;

  my_seglist.set_param("vertex_size", "3");
  string update_str = "points = ";


  list<CompPath>::iterator l;
  for(l=m_list.begin(); l!=m_list.end(); l++) {
    CompPath &lobj = *l;

    x = atof(lobj.m_x.c_str());
    y = atof(lobj.m_y.c_str());
    
    XYPoint point(x,y);
    point.set_param("vertex_size", "3");
    point.set_label(lobj.m_id);

    if(lobj.m_id=="firstpoint") {
      my_seglist.add_vertex(m_curr_point);//.x(),m_start_point.y());
    }
    else if(lobj.m_id=="lastpoint") {
      my_seglist.add_vertex(m_curr_point);//.x(),m_start_point.y());

    }
    else {      
      my_seglist.insert_vertex(point.x(),point.y());
    }
  }

  update_str       += my_seglist.get_spec();

    // m_list.clear();


  Notify("CLASS_PATTERN",update_str);


  // m_sent_all_points = true;
    // m_list.clear();


}

//---------------------------------------------------------
// Procedure: OnNewMail

bool GenPath::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;

    string key = msg.GetKey();
    string sval  = msg.GetString(); 


    // if(key=="WPT_STAT") {
    //   string value = msg.GetString();
    //   reportEvent("WPT_STAT = "+value);
    // }

    if(key=="HANG_Y") {

      reportEvent("I GOT A HANG_Y");
    }

    if(key=="GENPATH_REGENERATE") {
      string value = msg.GetString();
      if(value =="true") {
        reportEvent("DONE !!!!!");

        sendPoints();

        Notify("GENPATH_REGENERATE","false");
      }
    }


  //Every new VISIT_POINT instantiates a CompPath object which is pushed to a list
    if(key=="VISIT_POINT"){

      string value = msg.GetString();      
      CompPath b(value);
      m_list.push_front(b);
      // if(value=="firstpoint") {
      //   Notify("GENPATH_REGENERATE","true");
      // }
      if(value=="lastpoint") {
        Notify("GENPATH_REGENERATE","true");
      }

    }

    if(key=="NODE_REPORT_LOCAL"){
      string value = msg.GetString();  
      string x_str,y_str,ans; 
      double x_now,y_now;

      x_str = tokStringParse(value, "X", ',', '=');
      y_str = tokStringParse(value, "Y", ',', '=');
      x_now = atof(x_str.c_str());
      y_now = atof(y_str.c_str());
      m_x_curr = x_now;
      m_y_curr = y_now;

      XYPoint point(x_now,y_now);
      m_curr_point = point;

      if(!m_register_start){
        setStart(point);
      }

    }


#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif

   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool GenPath::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool GenPath::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!

      testComp();

  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool GenPath::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());

  STRING_LIST::iterator p;
  for(p=sParams.begin(); p!=sParams.end(); p++) {
    string orig  = *p;
    string line  = *p;
    string param = tolower(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if(param == "foo") {
      handled = true;
    }
    else if(param == "bar") {
      handled = true;
    }

    if(!handled)
      reportUnhandledConfigWarning(orig);

  }
  
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void GenPath::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  Register("VISIT_POINT", 0);
  Register("LOITER_UPDATE",0);
  Register("NODE_REPORT_LOCAL",0);
  Register("GENPATH_REGENERATE",0);
  Register("WPT_STAT",0);
  Register("HANG_Y",0);
  Notify("PAUSE_TIME","false");

}


//------------------------------------------------------------
// Procedure: buildReport()

bool GenPath::buildReport() 
{
  m_msgs << "============================================ \n";
  m_msgs << "File:                                        \n";
  m_msgs << "============================================ \n";

m_msgs << "SIZE" << m_list.size() << endl;


  // ACTable actab(4);
  // actab << "Alpha | Bravo | Charlie | Delta";
  // actab.addHeaderLines();
  // actab << "one" << "two" << "three" << "four";
  // m_msgs << actab.getFormattedString();

  return(true);
}




