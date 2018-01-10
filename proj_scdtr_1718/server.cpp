/**********************************************************\
* SCDTR - Projeto - stage 2 (Distributed Lighting Control) *
*                                                          *
* server.cpp                                               *
*                                                          *
* José Carlos Vieira - 90900                               *
* Pedro Carmo        - 90989                               *
*                                                          *
\**********************************************************/

// Compile:
// g++ server.cpp -o server -std=c++11 -lboost_system -lpthread

// Serial ports:
// ls -l /dev/serial*
// /dev/ttyUSB0

#include "i2c.cpp"
#include "Arduino.cpp"

//#define BOOST_ASIO_ENABLE_HANDLER_TRACKING
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <stdexcept>

#define SERVER_PORT 17000
#define SERIAL_PORT "/dev/ttyUSB0"
#define BAUD 115200
#define MAX_LUM 50

using boost::asio::ip::tcp;
using boost::system::error_code;

using namespace boost::asio;
using ip::tcp;

class session{
    public:
	session(io_service& io_service) : socket_(io_service){}

  	tcp::socket& socket(){
   	    return socket_;
  	}

	void print_client(std::string input){
	    const std::string& message = input;
	    async_write(socket_,buffer(message.c_str(),message.size()),boost::bind(&session::writeHandler,this,placeholders::error,placeholders::bytes_transferred));
        }

	void writeHandler(const boost::system::error_code& error,const size_t bytesTransferred){
	    if(error){
		delete this;
	    }
	}

  	void start(){
            async_read_until(socket_, buff_, '\n', boost::bind(&session::handle_read, this,_1));
	}

	void initial_print(){
	    print_client("\r\n\r\n---\r\nCopyright © 2017 - 2018 SCDTR\r\n\r\nUse 'h' to help.\r\nUse 'exit' to close session.\r\n\r\n");
    	    try {m_thread = std::thread(&session::thread_stream, this);} catch(...) {}
	    start();
	}

	void thread_stream(){
	    char cont = 0;
	    std::string s;

	    while(1){
		if(recebeu){recebeu = false;
		if(recv_desk>0 && recv_desk<MAX_LUM){
		    if(stream_l[recv_desk] && stream_d[recv_desk])
		        cont = 3;
		    else if(stream_l[recv_desk] && !stream_d[recv_desk])
		    	cont = 2;
		    else if(!stream_l[recv_desk] && stream_d[recv_desk])
	      	    	cont = 1;
		    else
			cont = 0;
		    if(cont > 0){
		        if(last_time_stamp[recv_desk].compare(aux3) != 0){
			    last_time_stamp[recv_desk] = aux3;
			    if(cont == 3)
				s = aux1 + aux2 + aux3 + "\n";
			    else if(cont == 2)
				s = aux1 + aux3 + "\n";
			    else if(cont == 1)
				s = aux2 + aux3 + "\n";
			    print_client(s);
		        }
		    }
		}
	    }}
	}

	int parse_str(std::string s){
	    size_t pos = 0;
	    char cont = 0;
    	    std::string token;
	    std::string delimiter = " ";
            bool c = false;
	    bool l = false;
	    std::string::size_type sz;

    	    while ((pos = s.find(delimiter))!= std::string::npos){
		cont++;
        	token = s.substr(0, pos);
        	if(cont == 1){
		    if(token=="c" || token=="d"){
			if(token=="c")
			    c = true;
		    }else
        	    	return 0;
		}else if(cont == 2){
		    if(token=="l" || token=="d"){
			if(token=="l")
			    l = true;
		    }else
			 return 0;
		}else if(cont > 2)
		    return 0;
        	s.erase(0, pos + delimiter.length());
    	    }
	    int i;
	    try{
	        i = std::stoi(s);
	    }catch(std::invalid_argument& e){
  		return 0;
	    }
	    if(i>=0 && i<MAX_LUM){
		if(l)
		    stream_l[i]=c;
		else
		    stream_d[i]=c;
	    }
	    return 0;
	}

  	void handle_read(const boost::system::error_code& ec){
	    if (!ec){
      		std::istream is(&buff_);
		std::getline(is, msg);

		if(msg == "exit"){
		    boost::system::error_code ignored_ec;
		    socket_.close(ignored_ec);
		}else if(!msg.empty()){
		    parse_str(msg);
		    DynamicCall(msg);
		    msg.erase();
		}else
		    str.erase();
		str+="\r\n";
		print_client(str);
		start();
	    }else{
               	delete this;
	    }
  	}

    private:
  	tcp::socket socket_;
	std::string msg;
	streambuf buff_;
	bool stream_l[MAX_LUM]={0};
	bool stream_d[MAX_LUM]={0};
	std::string last_time_stamp[MAX_LUM];
	std::thread m_thread;
};

class server{
    io_service& io_service_;
    tcp::acceptor acceptor_;
    posix::stream_descriptor input_;
    streambuf input_buffer_;

    public:
	server(io_service& io_service, short port) : io_service_(io_service),input_(io_service, ::dup(STDIN_FILENO)),input_buffer_(1024),
	    acceptor_(io_service, tcp::endpoint(tcp::v4(), port)) {
	    start_accept();
	}

    private:
	void start_accept() {
	    session* new_session = new session(io_service_);
            acceptor_.async_accept(new_session->socket(),boost::bind(&server::handle_accept, this,new_session, _1));
	}

	void handle_accept(session* new_session,const error_code& error) {
	    if (!error)
		new_session->initial_print();
	    else
		delete new_session;
	    start_accept();
	}
};

int main(void){
    try{
    	io_service io_service; // initialize services
    	server s(io_service, SERVER_PORT);

    	serial_port sp(io_service); // create io object
    	boost::system::error_code ec;

    	int aux = init_serial(SERIAL_PORT, BAUD);
    	//if(aux == -1)
    	//    return aux;

    	std::thread t(parse_I2C);

    	io_service.run();

    }catch (std::exception& e){
    	std::cerr << e.what() << std::endl;
    }
    return 0;
}
