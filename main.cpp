#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>

// for convenience
using json = nlohmann::json;

constexpr double pi()
{
	return(M_PI);
}
double deg2rad(double x)
{
	return(x * pi() / 180);
}
double rad2deg(double x)
{
	return(x * 180 / pi());
}

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s)
{//has data
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos)
  {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos)
  {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}//has data

int main()
{//main
	uWS::Hub h;

	PID pid;
	//pid.Init(0.2, 0.004, 3.0);
	//pid.Init(0.16, 0.001, 2);
	pid.Init(0.11, 0.0003, .9);

	h.onMessage([&pid](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode)
	{
		// "42" at the start of the message means there's a websocket message event.
		// The 4 signifies a websocket message
		// The 2 signifies a websocket event
		if (length && length > 2 && data[0] == '4' && data[1] == '2')
		{//if
			auto s = hasData(std::string(data).substr(0, length));
			if (s != "")
			{//if 2
				auto j = json::parse(s);
				std::string event = j[0].get<std::string>();
				if (event == "telemetry")
				{//if 3
					// j[1] is the data JSON object
					double cte = std::stod(j[1]["cte"].get<std::string>());
					double speed = std::stod(j[1]["speed"].get<std::string>());
					double angle = std::stod(j[1]["steering_angle"].get<std::string>());

					pid.UpdateError(cte);
					double steer_value = pid.TotalError();
					// remember the steering value is [-1, 1].
					// Feel free to play around with the throttle and speed
					// Maybe use another PID controller to control the speed!
          
					// DEBUG
					//std::cout << "CTE: " << cte << " Steering Value: " << steer_value << std::endl;
					//std::cout << "speed: " << speed << std::endl;

					json msgJson;
					msgJson["steering_angle"] = steer_value;
					//msgJson["throttle"] = 0.3;
					msgJson["throttle"] = 0.45;
					auto msg = "42[\"steer\"," + msgJson.dump() + "]";
					std::cout << msg << std::endl;
					ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
				}// if 3
			}// if 2
			else
			{// else
				// Manual driving
				std::string msg = "42[\"manual\",{}]";
				ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
			}// else
		}// if
	});

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}//main
