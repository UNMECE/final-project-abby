#include <iostream>
#include <random>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>


int main(){

//setting the random number generator
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> timeRandom(50,120);

std::uniform_real_distribution<double> waterLevelRandom(20,100);
std::uniform_real_distribution<double> waterNeedRandom(50,100);
std::uniform_real_distribution<double> waterCapacityRandom(100,200);


//
std::ofstream MyFile("RandomValues.dat");			
MyFile.is_open();		//Opening pixels.dat file for writing into
std::ostringstream ss;

int time = timeRandom(gen);
ss <<"Max Simulation Time" <<std::endl;
ss <<time <<std::endl;

int regionWaterLevel;
int regionWaterNeed;
int regionWaterCapacity;
std::string RegionName;
ss<<"Random Values" <<std::endl;

int waterDifference=0;
int totalwaterLevel=0;
int totalWaterNeed=0;
std::cout<<"Current State of the Regions: " <<std::endl;
std::cout<<"------------------------------" <<std::endl;
for(int i = 0; i < 3;i++)
{
	regionWaterLevel = waterLevelRandom(gen);
	regionWaterNeed = waterNeedRandom(gen);
	regionWaterCapacity = waterCapacityRandom(gen);
	waterDifference = (regionWaterNeed - regionWaterLevel);
	std::uniform_real_distribution<double> waterLevelUpdate(waterDifference,100);
	if(i == 0)
	{
		RegionName = "North";
		while(waterDifference > 25)
		{

			regionWaterLevel = waterLevelUpdate(gen);
			waterDifference = (regionWaterNeed- regionWaterLevel);
		}	
	}
	else if(i == 1)
	{
		RegionName = "South";
		while(waterDifference > 25)
		{	
			regionWaterLevel = waterLevelUpdate(gen);
			waterDifference = (regionWaterNeed - regionWaterLevel);
		}

	}
	else if(i == 2)
	{
		RegionName = "East";
		waterDifference = (regionWaterNeed + totalWaterNeed) - totalwaterLevel;
		std::cout<<"Water Difference: " <<waterDifference <<std::endl;
		if(waterDifference > 0)
		{
			std::uniform_real_distribution<double> waterLevelFinal(0, 20);
			regionWaterLevel = waterDifference + waterLevelFinal(gen);
			while(regionWaterCapacity < regionWaterLevel)
			{
				regionWaterCapacity = waterCapacityRandom(gen);
			}
		}
	}
	else
	{
		RegionName = "Unknown";
	}
	totalwaterLevel += regionWaterLevel;
	totalWaterNeed += regionWaterNeed;
	std::cout <<"Region: "<<RegionName <<", Water Level: "<<regionWaterLevel <<", Water Need: " <<regionWaterNeed <<", Water Capacity: " <<regionWaterCapacity	<<std::endl;
	ss <<RegionName <<"," <<regionWaterLevel <<"," <<regionWaterNeed <<"," <<regionWaterCapacity <<std::endl; 
}
std::cout<<"------------------------------------------------------------" <<std::endl;
std::cout<<"Please write your solution in the StudentSolution.cpp file." <<std::endl;
std::cout<<"Your code must solve each region's water needs within the following simulation time: " <<time <<std::endl;
std::cout<<"When you have saved your code and ready to run the simulation, you may press Y to run." <<std::endl;

MyFile<<ss.str();				
MyFile.close();	



//Design idea for the student to compile and execute their code while the program is running
//students enter a value to move the code forward
	char answer;
 	std::cout<<"Press Y to test your solveProblems function." <<std::endl;
 	std::cin >> answer;
//when the student has completed their StudentSolution code, then they can press y to move to simulation
 	if(answer=='Y' || 'y')
 	{

//this would call the compiler to compile the StudentSolution 
 		std::string compile_command = "g++ StudentSolution.cpp AcequiaManager.cpp SimulatorMain.cpp -std=c++11 -o ready";
 		int compile_result = system(compile_command.c_str());
		if(compile_result != 0)
		{
			std::cerr << "Compilation failed!" <<std::endl;
			return 1;
		}
// //this would call to execute the file 
		std::string execute_command = "./ready";
		int execute_result = system(execute_command.c_str());
		if(execute_result != 0)
 		{
 		 	std::cerr <<"execution failed!" <<std::endl;
 			return 1;
 		}
//if compiled and executed successfully, the simulation will resume using the student solution
//then evalulate and display results.	
 	}

return 0;
}