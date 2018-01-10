/**********************************************************\
* SCDTR - Projeto - stage 2 (Distributed Lighting Control) *
*                                                          *
* Serial.cpp                                               *
*                                                          *
* José Carlos Vieira - 90900                               *
* Pedro Carmo        - 90989                               *
*                                                          *
\**********************************************************/

#include <iostream>
#include <string>
#include <sstream>
#include <chrono>

#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>

using namespace boost::asio;

//GLOBALS
io_service io;
serial_port sp(io);
steady_timer tim(io);
streambuf read_buf;
int counter = 0;

int init_serial(std::string port, int boud){
    boost::system::error_code ec;

    sp.open(port, ec); // connect to port
    if(ec){
        std::cout << "Error, wrong serial port! -> " << ec << std::endl;
        return -1;
    }

    sp.set_option(serial_port_base::baud_rate(boud),ec); // set baud rate
    if(ec){
        std::cout << "Error, wrong serial baud! -> " << ec << std::endl;
        return -1;
    }
}

void serial_write_handler(const boost::system::error_code &ec, size_t nbytes);

void serial_timer_handler(const boost::system::error_code &ec) {
    std::ostringstream os;
    os << "Counter = " << ++counter;
    async_write(sp, buffer(os.str()), serial_write_handler);
}

void serial_write_handler(const boost::system::error_code &ec, size_t nbytes) {
    tim.expires_from_now(std::chrono::seconds(5));
    tim.async_wait(serial_timer_handler);
}

void serial_read_handler(const boost::system::error_code &ec, size_t nbytes) {
    std::cerr << &read_buf;
    async_read_until(sp,read_buf,'\n',serial_read_handler);
}
