/**
 * generated by Voice CodeGen Version: R1_v1.3.0
 * generated on: Mon Nov 14 16:54:17 IST 2016
 */

#include <thread>

#include <CommonAPI/CommonAPI.hpp>
#include "v1/com/monojit/Triangle.hpp"
#include "v1/com/monojit/TriangleProxy.hpp"
#include <unistd.h>



std::shared_ptr<v1::com::monojit::TriangleProxy<>> TriangleClientInstance;
void recv_cb(uint32_t value)
{

	std::cout << "recv_cb:Height Attribute " << value << std::endl;

}

void recv_BroadcastEvent(std::string value)
{

	std::cout << "recv_BroadcastEvent : " << value << std::endl;
	
}

void ReceiveAreaName(const CommonAPI::CallStatus& Status, const std::string& Name)
{
	std::cout << "ReceiveAreaName " << Name << std::endl;
}

void SetWidthData(const CommonAPI::CallStatus& Status, const uint32_t& value)
{
	std::cout << "SetWidthData:  " << value << std::endl;
	
}

int main(int argc, char **argv) {
	CommonAPI::Runtime::setProperty("LogContext", "TriangleClient");
	CommonAPI::Runtime::setProperty("LibraryBase", "TriangleClient");

	/*
	 * create the runtime
	 */
	std::shared_ptr<CommonAPI::Runtime> runtime = CommonAPI::Runtime::get();
	if(!runtime)
	{
		
		return 1;
	}
	
	std::cout << "Created Runtime objects" << std::endl;
	std::string domain = "local";



	    // create the Triangle proxy
	    
	    	std::string connection = "TriangleClientInstance";
	    	std::string instance = "TriangleServer.TriangleServerInstance";
			TriangleClientInstance = runtime->buildProxy<v1::com::monojit::TriangleProxy>(domain, instance, connection);
	    	if(!TriangleClientInstance)
	    	{
				std::cout << "client is not created " << std::endl;
			}
			else
			{
				std::cout << "Client created successfully" << std::endl;


				std::cout << "Created Client Instance" << std::endl;

				while (!TriangleClientInstance->isAvailable())
				{
					sleep(1);
				}

				CommonAPI::CallStatus callStatus;
				//std::function<void(uint32_t)> fcb = recv_cb;
				//TriangleClientInstance->getHeightAttribute().getChangedEvent().subscribe(fcb);
				//std::cout << "Subscribing a method" << std::endl;
				std::string e;
				TriangleClientInstance->getHeightAttribute().getChangedEvent().subscribe(std::bind(recv_cb, std::placeholders::_1));
				TriangleClientInstance->getAreaResultEvent().subscribe(std::bind(recv_BroadcastEvent, std::placeholders::_1));
				std::function<void(const CommonAPI::CallStatus&, const std::string&)> CallBackArea = ReceiveAreaName;
				TriangleClientInstance->AreaAsync(std::bind(ReceiveAreaName, std::placeholders::_1, std::placeholders::_2));
				std::function<void(const CommonAPI::CallStatus&, const uint32_t &)> set_WidthFunc = SetWidthData;
				uint32_t value = 73;
				TriangleClientInstance->getWidthAttribute().setValueAsync(value, set_WidthFunc);

				//std::cout << "string value area is " << e << std::endl;
				
				while (true) {
					//std::cout << "Waiting for calls... (Abort with CTRL+C)" << std::endl;
					std::this_thread::sleep_for(std::chrono::seconds(1));

				}
			}
    return 0;
}