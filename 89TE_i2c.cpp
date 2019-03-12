#include <iostream>
#include <unistd.h>
#include <cstdint>
#include <wiringPiI2C.h>
#include <cerrno>

using namespace std;

const uint8_t dev_id = 0x70;
const uint8_t dev_addr = 0b000001100;
const uint8_t dev_comm = 0x0C;
const uint8_t dev_data = 0x00;

uint8_t getCRC(uint8_t bytes_to_send[6]){
	uint8_t byte_sum = 0x00;
	uint8_t crc;
	for(int i=0; i<6 ; i++){
		byte_sum += bytes_to_send[i];
	}
	crc = byte_sum;
	crc += (byte_sum/(0x0100));
	crc = 0xFF - crc;
	return crc;
}

int main(){
	int fd;
	int write_res;
	uint8_t sensor_data[8];
	int read_res;
	uint8_t crc;
	uint8_t out_buff[6]{dev_addr, dev_comm, dev_data, dev_data, 
		dev_data, dev_data};
	crc = getCRC(out_buff);
	cout<<"CRC: "<<(int)crc<<endl;
	uint8_t out_buff_full[7]{dev_addr, dev_comm, dev_data, dev_data, 
		dev_data, dev_data, 0xFF /*crc*/};
	if((fd=wiringPiI2CSetup(0x70)) < 0){
		cout<<"Error Occured in setting up Dev_ID: 0x70"<<endl;
		return 0;
	}
	else{
		cout<<"FD= "<<fd<<endl;
		cout<<"-------------Before Writing, Check Sensor Array---------------"<<endl;
		read_res = read(fd, sensor_data, 8);
		cout<<"1: "<<(int)sensor_data[0]<<endl;
		cout<<"2: "<<(int)sensor_data[1]<<endl;
		cout<<"3: "<<(int)sensor_data[2]<<endl;
		cout<<"4: "<<(int)sensor_data[3]<<endl;
		cout<<"5: "<<(int)sensor_data[4]<<endl;
		cout<<"6: "<<(int)sensor_data[5]<<endl;
		cout<<"7: "<<(int)sensor_data[6]<<endl;
		cout<<"8: "<<(int)sensor_data[7]<<endl;
		cout<<"-------------End Test Before Write---------------"<<endl;
		if((write_res = write(fd, out_buff_full, 7)) != 7){
			cout<<"There could be an issue writing to the file."<<endl;
			cout<<"Write returned: "<<write_res<<endl;
			return 0;
		}
		else{
			sleep(5);
			if((read_res = read(fd, sensor_data, 8)) != 8){
				cout<<"There could be an issue reading from the file."<<endl;
				cout<<"Read returned: "<<read_res<<endl;
				return 0;
			}
			else{
				cout<<"Returned Data {Address}: "<<(int)sensor_data[0]<<endl;
				cout<<"Returned Data {VOC}: "<<(int)sensor_data[1]<<endl;
				cout<<"Returned Data {CO2}: "<<(int)sensor_data[2]<<endl;
				cout<<"Returned Data {Raw Sensor Value[MSB]}: "<<(int)sensor_data[3]<<endl;
				cout<<"Returned Data {Raw Sensor Value}: "<<(int)sensor_data[4]<<endl;
				cout<<"Returned Data {Raw Sensor Value[LSB]}: "<<(int)sensor_data[5]<<endl;
				cout<<"Returned Data {Error Byte}: "<<(int)sensor_data[6]<<endl;
				cout<<"Returned Data {CRC}: "<<(int)sensor_data[7]<<endl;
			}
		close(fd);
		}
	}
	return 0;
}
