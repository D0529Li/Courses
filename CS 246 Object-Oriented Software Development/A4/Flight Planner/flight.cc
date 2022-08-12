#include <cmath>
#include <algorithm>
#include "flight.h"

using namespace std;

Flight::Flight() : Flight("", "", 0, 0, "") {}

Flight::Flight(std::string from, std::string to, int duration, int price, std::string name) :
	from{ from }, to{ to }, duration{ duration }, price{ price }, name{ name } {}

std::string Flight::getFrom()
{
	return from;
}

std::string Flight::getTo()
{
	return to;
}

int Flight::getDuration()
{
	return duration;
}

int Flight::getPrice()
{
	return price;
}

std::string Flight::getName()
{
	return name;
}

FlightPlan::FlightPlan() {}

FlightPlan::FlightPlan(const FlightPlan& plan)
{
	steps.assign(plan.steps.begin(), plan.steps.end());
}

FlightPlan::FlightPlan(const FlightPlan& plan, const std::shared_ptr<Flight>& newFlight) : FlightPlan(plan)
{
	steps.push_back(newFlight);
}

FlightPlan::FlightPlan(const std::shared_ptr<Flight>& newFlight) : FlightPlan()
{
	steps.push_back(newFlight);
}

std::vector<std::shared_ptr<Flight>> FlightPlan::getSteps()
{
	return steps;
}

int FlightPlan::getDuration()
{
	int totalDuration = 0;
	for (auto i : steps) {
		totalDuration += i->getDuration();
		totalDuration++;
	}
	--totalDuration;
	return totalDuration;
}

int FlightPlan::getPrice()
{
	int totalPrice = 0;
	for (auto i : steps) {
		totalPrice += i->getPrice();
	}
	return totalPrice;
}

int FlightPlan::getPain()
{
	int d = getDuration();
	int p = getPrice();
	return static_cast<int>(pow(1.2, d) * p);
}

// return true if there is a duplicate flight; false otherwise
bool FlightPlanner::checkFlightDuplicate(const std::string& from, const std::string& to, const std::string& name)
{
	for (auto flight : flights) {
		if (flight->getFrom() == from && flight->getTo() == to && flight->getName() == name) return true;
	}
	return false;
}

// return true if the airport exists; false otherwise.
bool FlightPlanner::checkAirportExists(const std::string& airportName)
{
	if (find(airports.begin(), airports.end(), airportName) == airports.end()) return false;
	return true;
}

void FlightPlanner::planTripHelper(vector<shared_ptr<FlightPlan>>& res, const std::string& to,
	const shared_ptr<FlightPlan>& currPlan, vector<string>& usedAirports)
{
	if (usedAirports[usedAirports.size() - 1] == to) {
		res.push_back(currPlan);
		return;
	}
	for (auto flight : flights) {
		if (flight->getFrom() == usedAirports[usedAirports.size() - 1] &&
			find(usedAirports.begin(), usedAirports.end(), flight->getTo()) == usedAirports.end())
		{
			shared_ptr<FlightPlan> newPlan;
			if (currPlan != nullptr)
			{
				newPlan = make_shared<FlightPlan>(*currPlan, flight);
			}
			else {
				newPlan = make_shared<FlightPlan>(flight);
			}
			usedAirports.push_back(flight->getTo());
			planTripHelper(res, to, newPlan, usedAirports);
			usedAirports.erase(usedAirports.end() - 1);
		}
	}
}

void FlightPlanner::addAirport(const std::string& name)
{
	airports.push_back(name);
}

void FlightPlanner::addFlight(const std::string& from, const std::string& to, int duration, int price, const std::string& name)
{
	if (checkFlightDuplicate(from, to, name) || !checkAirportExists(from) || !checkAirportExists(to)) throw FlightNameException();
	flights.emplace_back(make_shared<Flight>(from, to, duration, price, name));
	flights.emplace_back(make_shared<Flight>(to, from, duration, price, name));
}

vector<shared_ptr<FlightPlan>> FlightPlanner::planTrip(const std::string& from, const std::string& to)
{
	if (!checkAirportExists(from) || !checkAirportExists(to)) {
		throw FlightNameException();
	}
	vector<shared_ptr<FlightPlan>> res;
	vector<string> usedAirports;
	usedAirports.push_back(from);
	planTripHelper(res, to, nullptr, usedAirports);
	sort(res.begin(), res.end(), [](shared_ptr<FlightPlan>& a, shared_ptr<FlightPlan>& b) { return (a->getPain() < b->getPain()); });
	return res;
}
