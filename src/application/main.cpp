
//#include <project.h>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/string.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/uuid/name_generator_md5.hpp>
#include <fstream>
#include <iostream>
#include <ios>
#include <iosfwd>
#include <string>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.


#include <core/AtlasEngine.h>

class Person {
private:
	std::string name;
	int age;
	double height;

	// Make Boost.Serialization a friend to access private members
	friend class boost::serialization::access;

	// Method to serialize and deserialize the object
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& name;
		ar& age;
		ar& height;
	}

public:
	// Constructors
	Person(const std::string& n = "", int a = 0, double h = 0.0)
		: name(n), age(a), height(h) {}

	// Display the Person's data
	void display() const {
		std::cout << "Name: " << name << ", Age: " << age
			<< ", Height: " << height << " meters" << std::endl;
	}
};
using namespace Atlas;

int main(int argc, char* argv[]) {

	// Serialize the object to a file
/*	Person p1("John Doe", 30, 1.75);
	{
		boost::uuids::uuid namespace_id = boost::uuids::string_generator()("my_namespace");
		boost::uuids::uuid uuid = boost::uuids::name_generator_md5(namespace_id)(&p1, sizeof(Person));

		std::ofstream ofs("C:\\Dev\\Techstorm-v5\\game\\data\\" + boost::uuids::to_string(uuid) + ".dat", std::ios_base::binary | std::ios_base::app);
		boost::archive::text_oarchive oa(ofs);
		oa.save_binary(&p1, sizeof(Person));  // Save the serialized data in binary format
	}

	// Serialize the object to a file
	Person p3("John roe", 260, 1.4435);
	{
		boost::uuids::uuid uuid = boost::uuids::random_generator()();

		std::ofstream ofs("C:\\Dev\\Techstorm-v5\\game\\data\\" + boost::uuids::to_string(uuid) + "person.dat", std::ios_base::binary | std::ios_base::app);
		boost::archive::text_oarchive oa(ofs);
		
		oa.save_binary(&p3, sizeof(Person));  // Save the serialized data in binary format
		//oa << p3;  // Serialize
	}

	// Deserialize the object from the file
	Person p2;
	{
	//	std::ifstream ifs("C:\\Dev\\Techstorm-v5\\game\\data\\person.dat");
//		boost::archive::text_iarchive ia(ifs);
	//	ia >> p2;  // Deserialize
	}*/

	//std::shared_ptr<PROJECT_TYPENAME> project = std::make_shared<PROJECT_TYPENAME>();

	//Atlas::Application::FrameManager& manager = Atlas::Application::FrameManager::Instance();
	//manager.userProject = project;

	//manager.launchThreads();
	


	EngineModulesInfo modulesInfo = EngineModulesInfo{ 
		new VulkanRenderer(new GLFWGameWindow("Vulkan", 800, 600, NULL, 60, "")),
		new GameThreader()
	};


	AtlasSettings settings{};

	AtlasEngine* atlas = new AtlasEngine(modulesInfo, settings);
	//

	atlas->init();

	atlas->run();

	return 0;
}
