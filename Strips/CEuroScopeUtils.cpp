#include "pch.h"
#include "CEuroScopeUtils.h"
#include "stdio.h"
#include "string"
#include "sstream"

#define MY_PLUGIN_NAME      "Anti-Faff Strips :P"
#define MY_PLUGIN_VERSION   "0.1"
#define MY_PLUGIN_DEVELOPER "AF, JF, KT"
#define MY_PLUGIN_COPYRIGHT "Free to be distributed as source code"

#define DEL "DEL"
#define GND "GND"
#define TWR "TWR"
#define APP "APP"
#define CTR "CTR"
#define FSS "FSS"


CEuroScopeUtils::CEuroScopeUtils(void)
    : CPlugIn(EuroScopePlugIn::COMPATIBILITY_CODE,
        MY_PLUGIN_NAME,
        MY_PLUGIN_VERSION,
        MY_PLUGIN_DEVELOPER,
        MY_PLUGIN_COPYRIGHT)
{
    // message on startup
    DisplayUserMessage("Anti-Faff Strips", "Version", MY_PLUGIN_VERSION, true, true, true, true, false);

    // gets the callsign of the user, converts from c_str to string
    std::string UserCallsign(this->ControllerMyself().GetCallsign());
    // extracts the first 4 chars for the ICAO code
    std::string UserICAO = UserCallsign.substr(0, 4);
    // get the facility (numbers defined in the EuroScopePlugIn.h)
    UserFacilityInt = GetUserFacilityInt();
    // convert to string for outputting
    std::string UserFacilityStr = UserFacilityConvert(UserFacilityInt);

    DisplayUserMessage("Anti-Faff Strips", "Callsign", UserICAO.c_str(), true, true, true, true, false);
    DisplayUserMessage("Anti-Faff Strips", "Position", UserFacilityStr.c_str(), true, true, true, true, false);
}

int CEuroScopeUtils::GetUserFacilityInt() {
    //Function to get the facility integer, as defined in "EuroScopePlugIn.h"
    int UserFacilityInt = this->ControllerMyself().GetFacility();
    return UserFacilityInt;
}

std::string CEuroScopeUtils::UserFacilityConvert(int UserFacility) {
    //Convert the integer to the #defined strings
    std::string UserFacilityStr = "Not Found";
    if (UserFacility == 1) {
        UserFacilityStr = FSS;
    }
    else if (UserFacility == 2) {
        UserFacilityStr = DEL;
    }
    else if (UserFacility == 3) {
        UserFacilityStr = GND;
    }
    else if (UserFacility == 4) {
        UserFacilityStr = TWR;
    }
    else if (UserFacility == 5) {
        UserFacilityStr = APP;
    }
    else if (UserFacility == 6) {
        UserFacilityStr = CTR;
    }
    return UserFacilityStr;
}

void CEuroScopeUtils::OnFlightPlanFlightPlanDataUpdate(EuroScopePlugIn::CFlightPlan FlightPlan) {
    // get all relevent flight plan info
    //gets callsign
    const char* FPCallsign = FlightPlan.GetCallsign();
    //origin
    const char* FPOrigin = FlightPlan.GetFlightPlanData().GetOrigin();
    //dest
    const char* FPDest = FlightPlan.GetFlightPlanData().GetDestination();
    //type
    const char* FPType = FlightPlan.GetFlightPlanData().GetAircraftFPType();
    //actype
    const char* FPACType = FlightPlan.GetFlightPlanData().GetAircraftInfo();
    //FL
    int FPAltitude = FlightPlan.GetFlightPlanData().GetFinalAltitude();
    //route - this could be looooong
    const char* FPRouteFull = FlightPlan.GetFlightPlanData().GetRoute();
    //speed
    int FPAirspeed = FlightPlan.GetFlightPlanData().GetTrueAirspeed();
    //sq
    const char* FPAllocSq = FlightPlan.GetControllerAssignedData().GetSquawk();
    //voice (duplicated in OnFlightPlanControllerAssignedDataUpdate)
    char FPVoice = FlightPlan.GetFlightPlanData().GetCommunicationType();
    //edt
    const char* FPEtd = FlightPlan.GetFlightPlanData().GetEstimatedDepartureTime();
    //capabilities
    char FPCapabilities = FlightPlan.GetFlightPlanData().GetCapibilities();
    //alternate
    const char* FPAlternate = FlightPlan.GetFlightPlanData().GetAlternate();
    //arrival runway
    const char* FPArrRwy = FlightPlan.GetFlightPlanData().GetArrivalRwy();
    //departure runway
    const char* FPDepRwy = FlightPlan.GetFlightPlanData().GetDepartureRwy();
    //remarks
    const char* FPRmks = FlightPlan.GetFlightPlanData().GetRemarks();
    //SID
    const char* FPSid = FlightPlan.GetFlightPlanData().GetSidName();
    //STAR
    const char* FPStar = FlightPlan.GetFlightPlanData().GetStarName();
    
    // send as message (for the moment)
    //set up message buffer
    char buffer[1000];
    int msg;
    
    msg = snprintf(buffer, 1000, "%s, origin %s, dest %s, type %s, EDT %s, altitude %d ft, type %s, speed %d kts, route %s, squawk %s, voice %c, capabilities %c, alternate %s, arrRwy %s, deprwy %s, rmks %s, SID %s, STAR %s .",
        FPCallsign, FPOrigin, FPDest, FPType, FPEtd, FPAltitude, FPACType, FPAirspeed,
        FPRouteFull, FPAllocSq, FPVoice, FPCapabilities, FPAlternate, FPArrRwy, FPDepRwy, FPRmks, FPSid, FPStar);
    //makes controller message
    DisplayUserMessage("Anti-Faff Strips", "New Flight Plan Data Update", buffer, true, true, true, true, true);
}

void CEuroScopeUtils::OnFlightPlanControllerAssignedDataUpdate(EuroScopePlugIn::CFlightPlan FlightPlan,
    int DataType) {
    //gets callsign for reference
    const char* FPCallsign = FlightPlan.GetCallsign();
    //get all the relevant controller assigned data info
    //heading
    int FPHeading = FlightPlan.GetControllerAssignedData().GetAssignedHeading();
    //mach
    int FPMach = FlightPlan.GetControllerAssignedData().GetAssignedMach();
    //heading
    int FPRate = FlightPlan.GetControllerAssignedData().GetAssignedRate();
    //speed
    int FPSpeed = FlightPlan.GetControllerAssignedData().GetAssignedSpeed();
    //cleared altitude
    int FPClearedAlt = FlightPlan.GetControllerAssignedData().GetClearedAltitude();
    //communication type (duplicate of info in OnFlightPlanFlightPlanDataUpdate)
    char FPVoice = FlightPlan.GetControllerAssignedData().GetCommunicationType();
    //direct point name
    const char* FPDirPointName = FlightPlan.GetControllerAssignedData().GetDirectToPointName();
    //final altitude
    int FPFinalAlt = FlightPlan.GetControllerAssignedData().GetFinalAltitude();
    //scratchpad
    const char* FPScratchpad = FlightPlan.GetControllerAssignedData().GetScratchPadString();
    //squawk (I think this is squawking, not assigned)
    const char* FPSquawk = FlightPlan.GetControllerAssignedData().GetSquawk();
    
    // send as message (for the moment)
    //set up message buffer
    char buffer[1000];
    int msg;
    
    msg = snprintf(buffer, 200, "%s, heading %d, mach %d, rate %d, speed %d, alt %d, voice %c, direct %s, final alt %d, scratcpad %s, sqk %s", 
        FPCallsign, FPHeading, FPMach, FPRate, FPSpeed, FPClearedAlt, FPVoice, FPDirPointName, FPFinalAlt, FPScratchpad, FPSquawk);
    DisplayUserMessage("Anti-Faff Strips", "New Controller Assigned Data Update", buffer, true, true, true, true, true);
}

void CEuroScopeUtils::OnFlightPlanDisconnect(EuroScopePlugIn::CFlightPlan FlightPlan) {
    //gets callsign for reference
    const char* FPCallsign = FlightPlan.GetCallsign();

    //nothing to delete yet, since it hasn't been decided where to save the info
    //sends message to user in meantime
    DisplayUserMessage("Anti-Faff Strips", "Deleting Flight Plan", FPCallsign, true, true, true, true, true);
}