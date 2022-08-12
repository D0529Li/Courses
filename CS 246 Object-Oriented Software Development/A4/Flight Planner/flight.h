#include <vector>
#include <string>
#include <memory>

class FlightPlanner;
class FlightPlan;
class Flight;
class FlightNameException;

class FlightPlanner {
	// fields
	std::vector<std::string> airports; // all airports
	std::vector<std::shared_ptr<Flight>> flights; // all flights

	// private helper functions
	bool checkFlightDuplicate(const std::string& from, const std::string& to, const std::string& name);
	bool checkAirportExists(const std::string& airportName);
	void planTripHelper(std::vector<std::shared_ptr<FlightPlan>>& res, const std::string& to,
		const std::shared_ptr<FlightPlan>& currPlan, std::vector<std::string>& usedAirports);
public:
	void addAirport(const std::string& name);
	void addFlight(const std::string& from, const std::string& to, int duration, int price, const std::string& name);
	std::vector<std::shared_ptr<FlightPlan>> planTrip(const std::string& from, const std::string& to);
};

class FlightPlan {
	std::vector<std::shared_ptr<Flight>> steps;
public:
	FlightPlan();
	FlightPlan(const FlightPlan& plan);
	FlightPlan(const FlightPlan& plan, const std::shared_ptr<Flight>& newFlight);
	FlightPlan(const std::shared_ptr<Flight>& newFlight);
	std::vector<std::shared_ptr<Flight>> getSteps();
	int getDuration();
	int getPrice();
	int getPain();
};

class Flight {
	std::string from;
	std::string to;
	int duration;
	int price;
	std::string name;
public:
	Flight();
	Flight(std::string from, std::string to, int duration, int price, std::string name);
	std::string getFrom();
	std::string getTo();
	int getDuration();
	int getPrice();
	std::string getName();
};

class FlightNameException {};
