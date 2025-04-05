server:
	gcc src/serverController.cpp src/message/Message.cpp src/utils/Utils.cpp -o dist/serverController.out -lstdc++

serverRun:
	./dist/serverController.out



vehicleArduino:
	gcc src/vehiculoController_arduino.cpp src/message/Message.cpp src/utils/Utils.cpp  -o dist/vehiculoController_arduino.out -lstdc++ -pthread

vehicleArduinoRun:
	./dist/vehiculoController_arduino.out



vehicleRaspberry:
	gcc src/vehiculoController_raspberry.cpp src/message/Message.cpp src/utils/Utils.cpp  -o dist/vehiculoController_raspberry.out -lstdc++ -pthread -lpigpio -lrt

vehicleRaspberryRun:
	./dist/vehiculoController_raspberry.out