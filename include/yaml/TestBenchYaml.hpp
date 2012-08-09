#ifndef TEST_BENCH_YAML_HPP
#define TEST_BENCH_YAML_HPP

#include <iostream>
#include <yaml-cpp/yaml.h>

namespace yaml
{
	struct TestBenchYaml
	{
		std::string moduleName;
		double timeResolution;
		double clockPeriod;
		int resetTime;
		double simulationTime;

		friend std::ostream& operator << ( std::ostream& os, const TestBenchYaml& obj)
		{
			os 
				 << "module_name     : " << obj.moduleName     << "\n"
				 << "time_resolution : " << obj.timeResolution << "\n"
				 << "clock_period    : " << obj.clockPeriod << "\n"
				 << "reset_time      : " << obj.resetTime      << "\n"
				 << "simulation_time : " << obj.simulationTime << "\n";

			return os;
		}

	};

	void operator >> ( const YAML::Node& node, TestBenchYaml& obj )
	{
		node["module_name"]     >> obj.moduleName;
		node["time_resolution"] >> obj.timeResolution;
		node["clock_period"]    >> obj.clockPeriod;
		node["reset_time"]      >> obj.resetTime;
		node["simulation_time"] >> obj.simulationTime;
	}

};

#endif

